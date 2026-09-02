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
#include "libmesh/petsc_matrix.h"

#include <functional>
#include <memory>
#include <mutex>
#include <set>

namespace libMesh
{
class MeshBase;
}

/**
 * A system matrix stored as a PETSc MATIS, the globally unassembled format that PCBDDC and
 * KSPFETIDP operate on.
 *
 * The decomposition is non-overlapping with exactly one subdomain per MPI rank. The subdomain is
 * defined by the local-to-global mapping that init() builds from the attached DofMap and the mesh:
 * the dofs of the elements the rank owns, unioned with the dofs of the elements the DofMap's
 * coupling functors pair them with. The Mat is created as MATIS from the start rather than
 * converted from an assembled AIJ matrix.
 *
 * Element matrices are added with global indices and MATIS maps them into the subdomain matrix,
 * but this is not a drop-in replacement for libMesh::PetscMatrix: close() is overridden and
 * reconciles the diagonal of every zeroed row across the subdomains that share it. Operations that
 * read entries back out (a single entry, a row, a matrix norm) have no PETSc implementation for
 * MATIS and report an error instead of a wrong answer; assembled() provides a copy that supports
 * them.
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
   * @param mesh The mesh of that system, whose local elements define this rank's subdomain
   */
  PetscMatrixIS(const libMesh::Parallel::Communicator & comm_in, const libMesh::MeshBase & mesh);

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
   * across the subdomains sharing them (see reconcileSubdomainDiagonals()).
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
   * An assembled (AIJ) copy of the closed matrix, for reading entries back out: the sum over the
   * subdomains that MATIS itself never forms. Objects such as the node-face contact constraints
   * read entries of the closed Jacobian while they assemble their own, and this is the matrix
   * they are handed.
   *
   * Collective, and a full copy of the matrix: call it once per assembly, at a point every rank
   * reaches, rather than per entry.
   */
  std::unique_ptr<libMesh::PetscMatrix<libMesh::Number>> assembled() const;

  /**
   * Register a function that adds to the subdomain the dofs this rank assembles into through
   * couplings no coupling functor announces: the geometric search couplings of node-face
   * contact, which MOOSE writes straight into the matrix.
   */
  void setExtraCouplingProvider(std::function<void(std::set<libMesh::dof_id_type> &)> provider)
  {
    _extra_coupling_provider = std::move(provider);
  }

  /**
   * Register a function to run after the subdomain has been remapped in place (see
   * rebuildIfSubdomainChanged()). A preconditioner that caches per-subdomain data on the Mat, as
   * PCBDDC does, has to be reset there.
   */
  void setSubdomainRemapCallback(std::function<void()> callback)
  {
    _on_subdomain_remap = std::move(callback);
  }

  /**
   * Release the cached subdomain scatter along with the Mat.
   *
   * The scatter and the subdomain multiplicity are built from the local-to-global mapping of the
   * Mat released here, so they cannot outlive it; init() rebuilds both.
   */
  virtual void clear() noexcept override;

private:
  /**
   * The dofs of this rank's subdomain in global numbering: the owned range first, then the dofs
   * of other ranks that the local elements and the DofMap's coupling functors reach, in increasing
   * order.
   */
  std::vector<PetscInt> subdomainDofs() const;

  /**
   * Attach the subdomain mapping to the Mat, preallocate the subdomain matrix from the sparsity
   * pattern, build the scatter and seed the diagonal. Everything past the creation of the Mat in
   * init(), so that a remapping can run it again on the same Mat.
   */
  void setupSubdomain();

  /**
   * Remap the Mat in place if the set of dofs this rank assembles into moved since the mapping
   * was built, which happens as a mortar pairing slides and the DofMap's coupling functors report
   * new partners. The Mat the solver holds stays the same object; PETSc replaces the subdomain
   * matrix behind it and raises its nonzero state at the next assembly, so the preconditioner
   * rebuilds its topology.
   *
   * Collective: one rank's change remaps every rank. Called from zero(), which opens every
   * Jacobian assembly right after the residual evaluation that brought the pairing up to date.
   */
  void rebuildIfSubdomainChanged();

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
   * reconcileSubdomainDiagonals() are built here too, because their layouts are fixed by the same
   * mapping and close() runs several times per Jacobian assembly.
   *
   * Called from init() rather than from close(): everything built here depends only on the
   * local-to-global mapping, which is fixed until the matrix is cleared, and VecScatterCreate is a
   * collective that communicates to work the mapping out.
   */
  void buildSubdomainScatter();

  /**
   * Whether the global dof \p dof lies inside this rank's subdomain, that is inside the
   * local-to-global mapping of the Mat.
   */
  bool inSubdomain(PetscInt dof) const;

  /**
   * Error if the global dof \p dof lies outside this rank's subdomain.
   *
   * MatSetValues on a MATIS silently masks out global indices the local-to-global mapping does
   * not cover, which would drop the entry without a trace. Failing here instead makes an
   * insufficient coupling radius diagnosable: couplings such as those of Constraint objects reach
   * the subdomain only through the DofMap's coupling functors, and this is the check that keeps
   * them honest. The subdomain is rebuilt before every Jacobian assembly
   * (rebuildIfSubdomainChanged()), so a failure here means a coupling that no coupling functor
   * announced.
   */
  void checkSubdomainCoverage(libMesh::numeric_index_type dof) const;

  /**
   * Error if any entry of \p dofs lies outside this rank's subdomain.
   */
  void checkSubdomainCoverage(const std::vector<libMesh::numeric_index_type> & dofs) const;

  /**
   * Split the diagonal of every zeroed or diagonal-deficient row evenly across the subdomains
   * that share it.
   *
   * Two situations leave a subdomain holding a row whose diagonal is zero while the assembled
   * global operator is correct, and PCBDDC cannot factor either one:
   *
   * - MatZeroRows on a MATIS zeroes the row in every subdomain that shares it, but the diagonal
   *   MOOSE writes back afterwards only reaches the owning rank.
   * - A dof that entered the subdomain through algebraic ghosting alone (for example the primary
   *   side of a contact pair) has no local element support, so nothing writes its local diagonal
   *   unless the coupling that pulled it in is active.
   *
   * Writing v/count into all count subdomains, the owner included, makes each subdomain block
   * nonsingular while the assembled diagonal stays exactly v.
   *
   * That invariant also makes this idempotent: a second pass reads the same assembled v and writes
   * the same v/count. Assembly therefore does not need to know which close() is the last one.
   */
  void reconcileSubdomainDiagonals();

  /// Serializes insertions, which MOOSE issues from several threads during Jacobian assembly
  std::mutex _petsc_matrix_mutex;

  /// The rows zeroed since the last zero() or init(), sorted and unique
  std::vector<libMesh::numeric_index_type> _zeroed_rows;

  /// Scatter between the global vector layout and this rank's subdomain vector, built by init()
  libMesh::WrappedPetsc<VecScatter> _to_subdomain;

  /// The number of subdomains sharing each subdomain dof, in subdomain ordering
  libMesh::WrappedPetsc<Vec> _multiplicity;

  /// Global-layout work vector of reconcileSubdomainDiagonals(), built by buildSubdomainScatter()
  libMesh::WrappedPetsc<Vec> _work_global;

  /// Subdomain marker of the zeroed rows, rebuilt only when some rank's _zeroed_rows changed
  libMesh::WrappedPetsc<Vec> _zeroed_marker;

  /// Subdomain copy of the assembled diagonal, refreshed on every reconcileSubdomainDiagonals()
  libMesh::WrappedPetsc<Vec> _subdomain_diagonal;

  /// Work vector holding this subdomain's local diagonal, then the shared deficiency marker
  libMesh::WrappedPetsc<Vec> _local_diagonal;

  /// Whether _zeroed_rows changed since _zeroed_marker was last rebuilt
  bool _zeroed_marker_stale = true;

  /// The mesh whose local elements define the subdomain; null for a wrapped Mat, whose mapping
  /// is already attached
  const libMesh::MeshBase * const _mesh = nullptr;

  /// Runs after every in-place remapping of the subdomain
  std::function<void()> _on_subdomain_remap;

  /// Adds the dofs of couplings that no coupling functor announces, see setExtraCouplingProvider()
  std::function<void(std::set<libMesh::dof_id_type> &)> _extra_coupling_provider;
};

#endif // LIBMESH_HAVE_PETSC
