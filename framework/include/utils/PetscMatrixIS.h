//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "libmesh/libmesh_config.h"

#ifdef LIBMESH_HAVE_PETSC

#include "libmesh/petsc_matrix_base.h"

#include <mutex>

/**
 * A system matrix stored as a PETSc MATIS, the globally unassembled format that PCBDDC and
 * KSPFETIDP operate on.
 *
 * The decomposition is non-overlapping with exactly one subdomain per MPI rank. The subdomain is
 * defined by the local-to-global mapping that init() builds from the attached DofMap: the dof range
 * the rank owns, unioned with the DofMap send list. The Mat is created as MATIS from the start
 * rather than converted from an assembled AIJ matrix.
 *
 * Element matrices are added with global indices and MATIS maps them into the subdomain matrix,
 * but this is not a drop-in replacement for libMesh::PetscMatrix: close() is overridden and
 * reconciles the diagonal of every zeroed row across the subdomains that share it. Operations that
 * read entries back out (a single entry, a row, a matrix norm) have no PETSc implementation for
 * MATIS and report an error instead of a wrong answer.
 *
 * All overridden virtual functions are documented in libMesh's sparse_matrix.h.
 */
class PetscMatrixIS : public libMesh::PetscMatrixBase<libMesh::Number>
{
public:
  /**
   * Constructor which leaves the matrix uninitialized.
   *
   * Attach a DofMap with attach_dof_map() and then call init() before using the matrix.
   *
   * @param comm_in The communicator of the system that owns this matrix
   */
  explicit PetscMatrixIS(const libMesh::Parallel::Communicator & comm_in);

  /**
   * Constructor which wraps an existing Mat.
   *
   * @param mat_in The matrix to wrap; it must already be of type MATIS and must already carry a
   *               local-to-global mapping
   * @param comm_in The communicator of the system that owns this matrix
   * @param destroy_on_exit Whether this object destroys \p mat_in when it is cleared
   */
  explicit PetscMatrixIS(Mat mat_in,
                         const libMesh::Parallel::Communicator & comm_in,
                         bool destroy_on_exit = false);

  /**
   * This class manages a C-style struct (Mat) manually, so we do not allow any automatic copy or
   * move functions to be generated.
   */
  PetscMatrixIS(PetscMatrixIS &&) = delete;
  PetscMatrixIS(const PetscMatrixIS &) = delete;
  PetscMatrixIS & operator=(const PetscMatrixIS &) = delete;
  PetscMatrixIS & operator=(PetscMatrixIS &&) = delete;
  virtual ~PetscMatrixIS();

  // The assignment operators declared above hide the base class ones, which assignment through the
  // SparseMatrix interface resolves to
  using libMesh::PetscMatrixBase<libMesh::Number>::operator=;

  /**
   * Build the MATIS matrix from the attached DofMap and sparsity pattern.
   *
   * This is safe to call repeatedly after clear(), which adaptivity does on every reinit.
   *
   * @param type The serial/parallel type of the matrix, used only to check in debug that a serial
   *             matrix owns every row of the system
   */
  virtual void init(libMesh::ParallelType type = libMesh::PARALLEL) override;

  /**
   * Not implemented: MATIS needs the local-to-global mapping that only the attached DofMap can
   * supply, so the matrix cannot be built from bare sizes. Use init(ParallelType) instead.
   */
  virtual void init(const libMesh::numeric_index_type m,
                    const libMesh::numeric_index_type n,
                    const libMesh::numeric_index_type m_l,
                    const libMesh::numeric_index_type n_l,
                    const libMesh::numeric_index_type nnz = 30,
                    const libMesh::numeric_index_type noz = 10,
                    const libMesh::numeric_index_type blocksize = 1) override;

  virtual void zero() override;

  virtual std::unique_ptr<libMesh::SparseMatrix<libMesh::Number>> zero_clone() const override;

  virtual std::unique_ptr<libMesh::SparseMatrix<libMesh::Number>> clone() const override;

  /**
   * Zero the given rows, leaving \p diag_value on the diagonal.
   *
   * \p diag_value reaches PETSc unchanged, and close() splits the diagonal of the zeroed rows
   * across the subdomains sharing them.
   *
   * No right hand side accompanies the call (the libMesh interface carries none) and none is
   * needed for the solution of the system to be unchanged: MOOSE writes the boundary equation of
   * a constrained dof into the residual itself, so the zeroed row with \p diag_value on the
   * diagonal already produces the correct Newton update for that dof.
   */
  virtual void zero_rows(std::vector<libMesh::numeric_index_type> & rows,
                         libMesh::Number diag_value = 0.0) override;

  virtual void set(const libMesh::numeric_index_type i,
                   const libMesh::numeric_index_type j,
                   const libMesh::Number value) override;

  virtual void add(const libMesh::numeric_index_type i,
                   const libMesh::numeric_index_type j,
                   const libMesh::Number value) override;

  virtual void add_matrix(const libMesh::DenseMatrix<libMesh::Number> & dm,
                          const std::vector<libMesh::numeric_index_type> & rows,
                          const std::vector<libMesh::numeric_index_type> & cols) override;

  virtual void add_matrix(const libMesh::DenseMatrix<libMesh::Number> & dm,
                          const std::vector<libMesh::numeric_index_type> & dof_indices) override;

  virtual void add(const libMesh::Number a,
                   const libMesh::SparseMatrix<libMesh::Number> & X) override;

  virtual void scale(const libMesh::Number scale) override;

  /**
   * Not implemented: PETSc provides no MatGetValues for MATIS because the matrix is stored
   * unassembled, so a global entry cannot be read back.
   */
  virtual libMesh::Number operator()(const libMesh::numeric_index_type i,
                                     const libMesh::numeric_index_type j) const override;

  /**
   * Not implemented: PETSc provides no MatGetRow for MATIS because the matrix is stored
   * unassembled, so a global row cannot be read back.
   */
  virtual void get_row(libMesh::numeric_index_type i,
                       std::vector<libMesh::numeric_index_type> & indices,
                       std::vector<libMesh::Number> & values) const override;

  /**
   * Not implemented: PETSc provides no MatNorm for MATIS because the matrix is stored unassembled,
   * so the sum over a global row is not available.
   */
  virtual libMesh::Real l1_norm() const override;

  /**
   * Not implemented: PETSc provides no MatNorm for MATIS because the matrix is stored unassembled,
   * so the sum over a global row is not available.
   */
  virtual libMesh::Real linfty_norm() const override;

  /**
   * Not implemented: MATIS is preallocated from the local-to-global mapping rather than from a
   * global nonzero pattern, so MatResetPreallocation has nothing to restore. Call clear() and
   * init() to rebuild the matrix.
   */
  virtual void restore_original_nonzero_pattern() override;

  virtual void print_personal(std::ostream & os = libMesh::out) const override;

  virtual void get_diagonal(libMesh::NumericVector<libMesh::Number> & dest) const override;

  virtual void get_transpose(libMesh::SparseMatrix<libMesh::Number> & dest) const override;

  virtual void close() override;

  /**
   * Release the cached subdomain scatter along with the Mat.
   *
   * The scatter and the subdomain multiplicity are built from the local-to-global mapping of the
   * Mat released here, so they cannot outlive it; init() rebuilds both.
   */
  virtual void clear() noexcept override;

private:
  /**
   * Build the local-to-global mapping that defines this rank's subdomain and attach it to the Mat.
   *
   * Called from init() before any insertion, because MATIS translates the global indices handed to
   * MatSetValues through this mapping.
   *
   * The mapping is given the block size of the attached DofMap so that PCBDDC classifies all of a
   * node's dofs as one entity rather than one component at a time.
   */
  void setSubdomainMapping();

  /**
   * Build the scatter between the global vector layout and this rank's subdomain vector, along
   * with the number of subdomains sharing each subdomain dof, and cache both. The work vectors of
   * reconcileZeroedDiagonals() are built here too, because their layouts are fixed by the same
   * mapping and close() runs several times per Jacobian assembly.
   *
   * Called from init() rather than from close(): everything built here depends only on the
   * local-to-global mapping, which is fixed until the matrix is cleared, and VecScatterCreate is a
   * collective that communicates to work the mapping out.
   */
  void buildSubdomainScatter();

  /**
   * Split the diagonal of every zeroed row evenly across the subdomains that share it.
   *
   * MatZeroRows on a MATIS zeroes the row in every subdomain that shares it, but the
   * diagonal MOOSE writes back afterwards only reaches the owning rank. Every other subdomain is
   * left holding an identically zero row, which PCBDDC cannot factor even though the assembled
   * global operator is correct. Writing v/count into all count subdomains, the owner included,
   * makes each subdomain block nonsingular while the assembled diagonal stays exactly v.
   *
   * That invariant also makes this idempotent: a second pass reads the same assembled v and writes
   * the same v/count. Assembly therefore does not need to know which close() is the last one.
   */
  void reconcileZeroedDiagonals();

  /// Serializes insertions, which MOOSE issues from several threads during Jacobian assembly
  std::mutex _petsc_matrix_mutex;

  /// The rows zeroed since the last zero() or init(), sorted and unique
  std::vector<libMesh::numeric_index_type> _zeroed_rows;

  /// Scatter between the global vector layout and this rank's subdomain vector, built by init()
  libMesh::WrappedPetsc<VecScatter> _to_subdomain;

  /// The number of subdomains sharing each subdomain dof, in subdomain ordering
  libMesh::WrappedPetsc<Vec> _multiplicity;

  /// Global-layout work vector of reconcileZeroedDiagonals(), built by buildSubdomainScatter()
  libMesh::WrappedPetsc<Vec> _work_global;

  /// Subdomain marker of the zeroed rows, rebuilt only when some rank's _zeroed_rows changed
  libMesh::WrappedPetsc<Vec> _zeroed_marker;

  /// Subdomain copy of the assembled diagonal, refreshed on every reconcileZeroedDiagonals()
  libMesh::WrappedPetsc<Vec> _subdomain_diagonal;

  /// Whether _zeroed_rows changed since _zeroed_marker was last rebuilt
  bool _zeroed_marker_stale = true;
};

#endif // LIBMESH_HAVE_PETSC
