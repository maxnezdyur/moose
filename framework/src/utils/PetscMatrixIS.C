//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "PetscMatrixIS.h"

#ifdef LIBMESH_HAVE_PETSC

#include "MooseError.h"

#include "libmesh/dense_matrix.h"
#include "libmesh/dof_map.h"
#include "libmesh/int_range.h"
#include "libmesh/petsc_vector.h"
#include "libmesh/sparsity_pattern.h"

#include <algorithm>
#include <iostream>

namespace
{
/**
 * Whether \p dofs splits into consecutive runs of \p bs entries that each cover one whole global
 * block, that is whether every run starts at a multiple of \p bs and counts up from there.
 *
 * @param dofs The subdomain dofs in global numbering
 * @param bs The block size the mapping is to be built with
 * @return Whether the list can be addressed by block index
 */
bool
isBlockAligned(const std::vector<PetscInt> & dofs, const PetscInt bs)
{
  const auto block_size = static_cast<std::size_t>(bs);
  if (dofs.size() % block_size != 0)
    return false;

  for (const auto block : libMesh::make_range(dofs.size() / block_size))
  {
    const auto begin = block * block_size;
    if (dofs[begin] % bs != 0)
      return false;
    for (const auto i : libMesh::make_range(begin + 1, begin + block_size))
      if (dofs[i] != dofs[i - 1] + 1)
        return false;
  }

  return true;
}
}

PetscMatrixIS::PetscMatrixIS(const libMesh::Parallel::Communicator & comm_in)
  : libMesh::PetscMatrixBase<libMesh::Number>(comm_in)
{
}

PetscMatrixIS::PetscMatrixIS(Mat mat_in,
                             const libMesh::Parallel::Communicator & comm_in,
                             const bool destroy_on_exit)
  : libMesh::PetscMatrixBase<libMesh::Number>(mat_in, comm_in, destroy_on_exit)
{
#ifndef NDEBUG
  PetscBool is_matis = PETSC_FALSE;
  LibmeshPetscCall(PetscObjectTypeCompare((PetscObject)mat_in, MATIS, &is_matis));
  mooseAssert(is_matis, "The wrapped matrix must be of type MATIS");
#endif
}

PetscMatrixIS::~PetscMatrixIS() = default;

void
PetscMatrixIS::init(const libMesh::ParallelType libmesh_dbg_var(type))
{
  mooseAssert(_dof_map, "A DofMap must be attached before the matrix is initialized");
  mooseAssert(_sp, "A sparsity pattern must be attached before the matrix is initialized");

  // Adaptivity clears and re-inits the matrix on every reinit, so start from a clean Mat
  if (initialized())
    clear();

  _zeroed_rows.clear();

  const libMesh::numeric_index_type m_global = _dof_map->n_dofs();
  const libMesh::numeric_index_type m_local = _dof_map->n_local_dofs();
  mooseAssert(m_global == m_local || type != libMesh::SERIAL,
              "A serial matrix must own every row of the system");

  LibmeshPetscCall(MatCreate(comm().get(), &_mat));
  LibmeshPetscCall(MatSetSizes(_mat,
                               static_cast<PetscInt>(m_local),
                               static_cast<PetscInt>(m_local),
                               static_cast<PetscInt>(m_global),
                               static_cast<PetscInt>(m_global)));
  LibmeshPetscCall(MatSetType(_mat, MATIS));

  // This precedes the local-to-global mapping because -mat_is_allow_repeated and
  // -mat_is_localmat_type are consumed when that mapping builds the subdomain matrix
  LibmeshPetscCall(MatSetFromOptions(_mat));

  PetscBool is_matis = PETSC_FALSE;
  LibmeshPetscCall(PetscObjectTypeCompare((PetscObject)_mat, MATIS, &is_matis));
  if (!is_matis)
    mooseError("The system matrix type was changed away from MATIS, which disables the domain "
               "decomposition preconditioner. Remove -mat_type from the PETSc options.");

  // Registers this object as the Mat context. libMesh's SNES and KSP callbacks recover the
  // SparseMatrix through PetscMatrixBase::get_context and dereference it without a null check
  set_context();

  setSubdomainMapping();

  const std::vector<libMesh::numeric_index_type> & n_nz = _sp->get_n_nz();
  const std::vector<libMesh::numeric_index_type> & n_oz = _sp->get_n_oz();
  mooseAssert(n_nz.size() == m_local && n_oz.size() == m_local,
              "The sparsity pattern must hold one entry per locally owned row");

  // MATIS overestimates the subdomain preallocation from these global per-row counts. They stay one
  // count per scalar row when the mapping is blocked, because MATIS derives the block counts itself
  LibmeshPetscCall(
      MatISSetPreallocation(_mat,
                            0,
                            libMesh::numeric_petsc_cast(n_nz.empty() ? nullptr : n_nz.data()),
                            0,
                            libMesh::numeric_petsc_cast(n_oz.empty() ? nullptr : n_oz.data())));

  // MatSeqAIJSetPreallocation turns MAT_NEW_NONZERO_ALLOCATION_ERR on inside the call above, and
  // MatZeroRows drops a Dirichlet row's diagonal unless the pattern is kept; the next
  // assembly compacts the freed slack away and the one after it fails to re-insert that diagonal
  Mat local_mat;
  LibmeshPetscCall(MatISGetLocalMat(_mat, &local_mat));
  LibmeshPetscCall(MatSetOption(local_mat, MAT_KEEP_NONZERO_PATTERN, PETSC_TRUE));
  LibmeshPetscCall(MatSetOption(local_mat, MAT_NEW_NONZERO_ALLOCATION_ERR, PETSC_FALSE));
  LibmeshPetscCall(MatISRestoreLocalMat(_mat, &local_mat));

  buildSubdomainScatter();

  _is_initialized = true;
}

void
PetscMatrixIS::setSubdomainMapping()
{
  const libMesh::dof_id_type first_dof = _dof_map->first_dof();
  const libMesh::dof_id_type end_dof = _dof_map->end_dof();
  const std::vector<libMesh::dof_id_type> & send_list = _dof_map->get_send_list();

  // The subdomain must contain every dof this rank inserts into, because MatSetValues on a MATIS
  // masks out global indices the mapping does not cover. That set is the owned dof range plus the
  // send list, which the DofMap builds from the same coupling functors that generate the sparsity
  // pattern and which already carries the constraint masters. The sparsity pattern columns are not
  // unioned in as well: PetscMatrixBase reports need_full_sparsity_pattern() == false, so the
  // DofMap keeps only the n_nz and n_oz counts and discards the column graph.
  std::vector<PetscInt> subdomain_dofs;
  subdomain_dofs.reserve((end_dof - first_dof) + send_list.size());
  for (const auto dof : libMesh::make_range(first_dof, end_dof))
    subdomain_dofs.push_back(libMesh::cast_int<PetscInt>(dof));
  // The send list is sorted and unique but may repeat dofs this rank owns
  for (const auto dof : send_list)
    if (dof < first_dof || dof >= end_dof)
      subdomain_dofs.push_back(libMesh::cast_int<PetscInt>(dof));

  // PCBDDC decides whether a dof is a corner, an edge, or a face dof from the set of subdomains
  // sharing it. A mapping of block size one hands it every component of a node separately, and for
  // a system of PDEs the coarse space that classification produces is far too weak; a mapping of
  // block size bs presents the bs dofs of a node as a single entity. PETSc's own three dimensional
  // elasticity example builds its MATIS the same way, see src/ksp/ksp/tutorials/ex71.c
  const auto dof_map_bs = libMesh::cast_int<PetscInt>(_dof_map->block_size());

  // libMesh numbers a blocked DofMap a node at a time, so the owned range and the send list both
  // come out block aligned. Indexing a list that is not by block would silently reorder the
  // subdomain, so fall back to scalar indices instead. Every rank has to reach the same answer,
  // because MatSetBlockSize and the mapping it feeds are collective
  bool block_aligned = dof_map_bs == 1 || isBlockAligned(subdomain_dofs, dof_map_bs);
  mooseAssert(block_aligned,
              "A DofMap reporting a block size above one must number its dofs in blocks");
  comm().min(block_aligned);

  const PetscInt bs = block_aligned ? dof_map_bs : 1;

  std::vector<PetscInt> block_dofs;
  if (bs > 1)
  {
    const auto block_size = static_cast<std::size_t>(bs);
    block_dofs.reserve(subdomain_dofs.size() / block_size);
    for (const auto block : libMesh::make_range(subdomain_dofs.size() / block_size))
      block_dofs.push_back(subdomain_dofs[block * block_size] / bs);

    // MATIS sizes its local matrix and its preallocation from the layout block size as well as from
    // the mapping, so the Mat has to carry it too
    LibmeshPetscCall(MatSetBlockSize(_mat, bs));
  }

  // A mapping of block size bs is indexed by global block id, one entry per block rather than one
  // per dof
  const std::vector<PetscInt> & mapping_dofs = bs > 1 ? block_dofs : subdomain_dofs;

  ISLocalToGlobalMapping mapping;
  LibmeshPetscCall(ISLocalToGlobalMappingCreate(comm().get(),
                                                bs,
                                                libMesh::cast_int<PetscInt>(mapping_dofs.size()),
                                                mapping_dofs.data(),
                                                PETSC_COPY_VALUES,
                                                &mapping));

  // This must precede the first insertion, which is why it belongs to init()
  LibmeshPetscCall(MatSetLocalToGlobalMapping(_mat, mapping, mapping));

  // The Mat holds its own reference now, so dropping ours leaves clear() with nothing beyond the
  // Mat itself to release
  LibmeshPetscCall(ISLocalToGlobalMappingDestroy(&mapping));
}

void
PetscMatrixIS::buildSubdomainScatter()
{
  ISLocalToGlobalMapping mapping;
  LibmeshPetscCall(MatGetLocalToGlobalMapping(_mat, &mapping, nullptr));

  PetscInt n_subdomain_dofs;
  LibmeshPetscCall(ISLocalToGlobalMappingGetSize(mapping, &n_subdomain_dofs));

  // A blocked mapping expands to one index per dof here, which is the scalar subdomain ordering
  // that MatSetValues and the subdomain matrix use
  const PetscInt * subdomain_dofs;
  LibmeshPetscCall(ISLocalToGlobalMappingGetIndices(mapping, &subdomain_dofs));

  // PETSC_COPY_VALUES rather than PETSC_USE_POINTER, because the indices go back to the mapping
  // below while the IS is still in use
  libMesh::WrappedPetsc<IS> subdomain_is;
  LibmeshPetscCall(ISCreateGeneral(
      comm().get(), n_subdomain_dofs, subdomain_dofs, PETSC_COPY_VALUES, subdomain_is.get()));

  LibmeshPetscCall(ISLocalToGlobalMappingRestoreIndices(mapping, &subdomain_dofs));

  libMesh::WrappedPetsc<Vec> global;
  LibmeshPetscCall(MatCreateVecs(_mat, nullptr, global.get()));

  libMesh::WrappedPetsc<Vec> subdomain;
  LibmeshPetscCall(VecCreateSeq(PETSC_COMM_SELF, n_subdomain_dofs, subdomain.get()));

  LibmeshPetscCall(VecScatterCreate(global, subdomain_is, subdomain, nullptr, _to_subdomain.get()));

  // Summing ones over the subdomains gives each dof the number of subdomains sharing it
  LibmeshPetscCall(VecDuplicate(subdomain, _multiplicity.get()));
  LibmeshPetscCall(VecSet(subdomain, 1.0));
  LibmeshPetscCall(VecSet(global, 0.0));
  LibmeshPetscCall(VecScatterBegin(_to_subdomain, subdomain, global, ADD_VALUES, SCATTER_REVERSE));
  LibmeshPetscCall(VecScatterEnd(_to_subdomain, subdomain, global, ADD_VALUES, SCATTER_REVERSE));
  LibmeshPetscCall(
      VecScatterBegin(_to_subdomain, global, _multiplicity, INSERT_VALUES, SCATTER_FORWARD));
  LibmeshPetscCall(
      VecScatterEnd(_to_subdomain, global, _multiplicity, INSERT_VALUES, SCATTER_FORWARD));
}

void
PetscMatrixIS::clear() noexcept
{
  _to_subdomain.reset_to_zero();
  _multiplicity.reset_to_zero();
  _zeroed_rows.clear();

  libMesh::PetscMatrixBase<libMesh::Number>::clear();
}

void
PetscMatrixIS::init(const libMesh::numeric_index_type,
                    const libMesh::numeric_index_type,
                    const libMesh::numeric_index_type,
                    const libMesh::numeric_index_type,
                    const libMesh::numeric_index_type,
                    const libMesh::numeric_index_type,
                    const libMesh::numeric_index_type)
{
  mooseError("A PetscMatrixIS cannot be initialized from sizes alone. MATIS is defined by the "
             "local-to-global mapping of the subdomain, which is built from the attached DofMap; "
             "attach a DofMap and call init(ParallelType) instead.");
}

void
PetscMatrixIS::zero()
{
  mooseAssert(initialized(), "The matrix must be initialized before it can be zeroed");

  _zeroed_rows.clear();

  // Unlike an AIJ matrix, a rank that owns no rows can still hold a nonempty subdomain matrix, so
  // this is not guarded on the local size
  LibmeshPetscCall(MatZeroEntries(_mat));
}

std::unique_ptr<libMesh::SparseMatrix<libMesh::Number>>
PetscMatrixIS::zero_clone() const
{
  if (!closed())
    mooseError("A PetscMatrixIS must be closed before it can be cloned");

  Mat copy;
  LibmeshPetscCall(MatDuplicate(_mat, MAT_DO_NOT_COPY_VALUES, &copy));

  auto ret = std::make_unique<PetscMatrixIS>(copy, comm());
  ret->set_destroy_mat_on_exit(true);

  return ret;
}

std::unique_ptr<libMesh::SparseMatrix<libMesh::Number>>
PetscMatrixIS::clone() const
{
  if (!closed())
    mooseError("A PetscMatrixIS must be closed before it can be cloned");

  Mat copy;
  LibmeshPetscCall(MatDuplicate(_mat, MAT_COPY_VALUES, &copy));

  auto ret = std::make_unique<PetscMatrixIS>(copy, comm());
  ret->set_destroy_mat_on_exit(true);

  return ret;
}

void
PetscMatrixIS::zero_rows(std::vector<libMesh::numeric_index_type> & rows,
                         libMesh::Number diag_value)
{
  mooseAssert(initialized(), "The matrix must be initialized before rows can be zeroed");

  // MatZeroRows on a MATIS broadcasts the zeroing to every subdomain sharing the row, so
  // each subdomain matrix loses the column of a constrained dof along with its row. No right hand
  // side is handed over to be fixed up because the libMesh interface carries none; see the header
  // for why the solution of the system is unchanged without one
  if (!rows.empty())
    LibmeshPetscCall(MatZeroRows(_mat,
                                 libMesh::cast_int<PetscInt>(rows.size()),
                                 libMesh::numeric_petsc_cast(rows.data()),
                                 libMesh::PS(diag_value),
                                 nullptr,
                                 nullptr));
  else
    LibmeshPetscCall(MatZeroRows(_mat, 0, nullptr, libMesh::PS(diag_value), nullptr, nullptr));

  _zeroed_rows.insert(_zeroed_rows.end(), rows.begin(), rows.end());
  std::sort(_zeroed_rows.begin(), _zeroed_rows.end());
  _zeroed_rows.erase(std::unique(_zeroed_rows.begin(), _zeroed_rows.end()), _zeroed_rows.end());
}

void
PetscMatrixIS::set(const libMesh::numeric_index_type i,
                   const libMesh::numeric_index_type j,
                   const libMesh::Number value)
{
  mooseAssert(initialized(), "The matrix must be initialized before values can be set");

  PetscInt i_val = libMesh::cast_int<PetscInt>(i), j_val = libMesh::cast_int<PetscInt>(j);
  const PetscScalar petsc_value = libMesh::PS(value);

  std::scoped_lock lock(_petsc_matrix_mutex);
  LibmeshPetscCall(MatSetValues(_mat, 1, &i_val, 1, &j_val, &petsc_value, INSERT_VALUES));
}

void
PetscMatrixIS::add(const libMesh::numeric_index_type i,
                   const libMesh::numeric_index_type j,
                   const libMesh::Number value)
{
  mooseAssert(initialized(), "The matrix must be initialized before values can be added");

  PetscInt i_val = libMesh::cast_int<PetscInt>(i), j_val = libMesh::cast_int<PetscInt>(j);
  const PetscScalar petsc_value = libMesh::PS(value);

  std::scoped_lock lock(_petsc_matrix_mutex);
  LibmeshPetscCall(MatSetValues(_mat, 1, &i_val, 1, &j_val, &petsc_value, ADD_VALUES));
}

void
PetscMatrixIS::add_matrix(const libMesh::DenseMatrix<libMesh::Number> & dm,
                          const std::vector<libMesh::numeric_index_type> & rows,
                          const std::vector<libMesh::numeric_index_type> & cols)
{
  mooseAssert(initialized(), "The matrix must be initialized before values can be added");
  mooseAssert(rows.size() == dm.m() && cols.size() == dm.n(),
              "The index vectors must match the shape of the element matrix");

  std::scoped_lock lock(_petsc_matrix_mutex);
  LibmeshPetscCall(MatSetValues(_mat,
                                libMesh::cast_int<PetscInt>(dm.m()),
                                libMesh::numeric_petsc_cast(rows.data()),
                                libMesh::cast_int<PetscInt>(dm.n()),
                                libMesh::numeric_petsc_cast(cols.data()),
                                libMesh::pPS(dm.get_values().data()),
                                ADD_VALUES));
}

void
PetscMatrixIS::add_matrix(const libMesh::DenseMatrix<libMesh::Number> & dm,
                          const std::vector<libMesh::numeric_index_type> & dof_indices)
{
  add_matrix(dm, dof_indices, dof_indices);
}

void
PetscMatrixIS::add(const libMesh::Number a, const libMesh::SparseMatrix<libMesh::Number> & X)
{
  mooseAssert(initialized(), "The matrix must be initialized before another can be added");
  mooseAssert(m() == X.m() && n() == X.n(),
              "The matrix being added must have the same global shape");

  const auto * const petsc_X =
      libMesh::cast_ptr<const libMesh::PetscMatrixBase<libMesh::Number> *>(&X);
  mooseAssert(petsc_X->closed(), "The matrix being added must be closed");

  LibmeshPetscCall(MatAXPY(_mat, libMesh::PS(a), petsc_X->mat(), DIFFERENT_NONZERO_PATTERN));
}

void
PetscMatrixIS::scale(const libMesh::Number scale)
{
  mooseAssert(closed(), "The matrix must be closed before it can be scaled");

  LibmeshPetscCall(MatScale(_mat, libMesh::PS(scale)));
}

libMesh::Number
PetscMatrixIS::operator()(const libMesh::numeric_index_type,
                          const libMesh::numeric_index_type) const
{
  mooseError("PETSc provides no MatGetValues for MATIS because a PetscMatrixIS is stored in "
             "globally unassembled form, so a single global entry cannot be read back.");
}

void
PetscMatrixIS::get_row(libMesh::numeric_index_type,
                       std::vector<libMesh::numeric_index_type> &,
                       std::vector<libMesh::Number> &) const
{
  mooseError("PETSc provides no MatGetRow for MATIS because a PetscMatrixIS is stored in globally "
             "unassembled form, so a global row cannot be read back.");
}

libMesh::Real
PetscMatrixIS::l1_norm() const
{
  mooseError("PETSc provides no MatNorm for MATIS because a PetscMatrixIS is stored in globally "
             "unassembled form, so the sum over a global row is not available.");
}

libMesh::Real
PetscMatrixIS::linfty_norm() const
{
  mooseError("PETSc provides no MatNorm for MATIS because a PetscMatrixIS is stored in globally "
             "unassembled form, so the sum over a global row is not available.");
}

void
PetscMatrixIS::restore_original_nonzero_pattern()
{
  mooseError("A PetscMatrixIS is preallocated through its local-to-global mapping rather than "
             "from a global nonzero pattern, so MatResetPreallocation has nothing to restore. "
             "Call clear() and then init() to rebuild the matrix.");
}

void
PetscMatrixIS::print_personal(std::ostream & os) const
{
  mooseAssert(initialized(), "The matrix must be initialized before it can be printed");

  if (os.rdbuf() != std::cout.rdbuf())
    mooseError("A PetscMatrixIS can only be printed to standard output. Printing to another "
               "stream requires reading global rows back out of the matrix, which MATIS does not "
               "support.");

  LibmeshPetscCall(MatView(_mat, nullptr));
}

void
PetscMatrixIS::get_diagonal(libMesh::NumericVector<libMesh::Number> & dest) const
{
  auto & petsc_dest = libMesh::cast_ref<libMesh::PetscVector<libMesh::Number> &>(dest);

  LibmeshPetscCall(MatGetDiagonal(_mat, petsc_dest.vec()));
}

void
PetscMatrixIS::close()
{
  libMesh::PetscMatrixBase<libMesh::Number>::close();

  reconcileZeroedDiagonals();
}

void
PetscMatrixIS::reconcileZeroedDiagonals()
{
  // A rank carrying no Dirichlet boundary of its own still holds subdomain copies of rows another
  // rank zeroed, so this decision has to be taken across the whole communicator or the collective
  // calls below deadlock
  bool any_zeroed_rows = !_zeroed_rows.empty();
  comm().max(any_zeroed_rows);
  if (!any_zeroed_rows)
    return;

  mooseAssert(_to_subdomain && _multiplicity,
              "init() must have built the subdomain scatter before diagonals are reconciled");

  PetscInt n_subdomain_dofs;
  LibmeshPetscCall(VecGetLocalSize(_multiplicity, &n_subdomain_dofs));

  libMesh::WrappedPetsc<Vec> global;
  LibmeshPetscCall(MatCreateVecs(_mat, nullptr, global.get()));

  // Marking the rows globally and reading them back is how a subdomain learns about rows that a
  // different rank owns and zeroed, which never appear in its own record
  libMesh::WrappedPetsc<Vec> zeroed;
  LibmeshPetscCall(VecDuplicate(_multiplicity, zeroed.get()));
  LibmeshPetscCall(VecSet(global, 0.0));
  for (const auto row : _zeroed_rows)
    LibmeshPetscCall(VecSetValue(global, libMesh::cast_int<PetscInt>(row), 1.0, INSERT_VALUES));
  LibmeshPetscCall(VecAssemblyBegin(global));
  LibmeshPetscCall(VecAssemblyEnd(global));
  LibmeshPetscCall(VecScatterBegin(_to_subdomain, global, zeroed, INSERT_VALUES, SCATTER_FORWARD));
  LibmeshPetscCall(VecScatterEnd(_to_subdomain, global, zeroed, INSERT_VALUES, SCATTER_FORWARD));

  // MatGetDiagonal on a MATIS sums the subdomain diagonals, so this is the assembled value that
  // has to be preserved
  libMesh::WrappedPetsc<Vec> diagonal;
  LibmeshPetscCall(VecDuplicate(_multiplicity, diagonal.get()));
  LibmeshPetscCall(MatGetDiagonal(_mat, global));
  LibmeshPetscCall(
      VecScatterBegin(_to_subdomain, global, diagonal, INSERT_VALUES, SCATTER_FORWARD));
  LibmeshPetscCall(VecScatterEnd(_to_subdomain, global, diagonal, INSERT_VALUES, SCATTER_FORWARD));

  const PetscScalar *zeroed_values, *multiplicity_values, *diagonal_values;
  LibmeshPetscCall(VecGetArrayRead(zeroed, &zeroed_values));
  LibmeshPetscCall(VecGetArrayRead(_multiplicity, &multiplicity_values));
  LibmeshPetscCall(VecGetArrayRead(diagonal, &diagonal_values));

  Mat local_mat;
  LibmeshPetscCall(MatISGetLocalMat(_mat, &local_mat));
  for (const auto i : libMesh::make_range(n_subdomain_dofs))
    if (zeroed_values[i] != 0.)
      LibmeshPetscCall(
          MatSetValue(local_mat, i, i, diagonal_values[i] / multiplicity_values[i], INSERT_VALUES));
  LibmeshPetscCall(MatAssemblyBegin(local_mat, MAT_FINAL_ASSEMBLY));
  LibmeshPetscCall(MatAssemblyEnd(local_mat, MAT_FINAL_ASSEMBLY));
  LibmeshPetscCall(MatISRestoreLocalMat(_mat, &local_mat));

  LibmeshPetscCall(VecRestoreArrayRead(diagonal, &diagonal_values));
  LibmeshPetscCall(VecRestoreArrayRead(_multiplicity, &multiplicity_values));
  LibmeshPetscCall(VecRestoreArrayRead(zeroed, &zeroed_values));
}

void
PetscMatrixIS::get_transpose(libMesh::SparseMatrix<libMesh::Number> & dest) const
{
  auto & petsc_dest = libMesh::cast_ref<PetscMatrixIS &>(dest);

  // If we are not reusing the matrix then dest has to be cleared, otherwise we leak its Mat
  if (&petsc_dest != this)
    dest.clear();

  if (&petsc_dest == this)
    LibmeshPetscCall(MatTranspose(_mat, MAT_INPLACE_MATRIX, &petsc_dest._mat));
  else
    LibmeshPetscCall(MatTranspose(_mat, MAT_INITIAL_MATRIX, &petsc_dest._mat));

  petsc_dest._is_initialized = true;
  petsc_dest.close();
}

#endif // LIBMESH_HAVE_PETSC
