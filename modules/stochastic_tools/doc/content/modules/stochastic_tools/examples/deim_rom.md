# DEIM Reduced-Order Models

This page describes the Discrete Empirical Interpolation Method (DEIM) reduced-order model in the [stochastic tools module](modules/stochastic_tools/index.md), and how to train and evaluate one. The pipeline is wired automatically by the [ROM](syntax/ROM/index.md) block ([ROMSetupAction.md]); this page explains what that pipeline computes.

## Background

A reduced-basis model approximates the high-dimensional solution $\boldsymbol{u}(\boldsymbol{\mu}) \in \mathbb{R}^N$ at parameter sample $\boldsymbol{\mu}$ in a low-dimensional space,

\begin{equation}
\boldsymbol{u}(\boldsymbol{\mu}) \approx \boldsymbol{\Phi}\, \boldsymbol{c}(\boldsymbol{\mu}),
\end{equation}

where the columns of $\boldsymbol{\Phi} \in \mathbb{R}^{N\times k}$ are a Proper Orthogonal Decomposition (POD) basis of solution snapshots (see [PODMapping.md]) and $\boldsymbol{c} \in \mathbb{R}^{k}$ are the reduced coordinates. Projecting the governing residual onto this basis gives a $k$-dimensional nonlinear system for $\boldsymbol{c}$. That projection, $\boldsymbol{\Phi}^T \boldsymbol{r}$, is cheap to solve but still requires assembling the full residual $\boldsymbol{r} \in \mathbb{R}^N$ at every iteration, which defeats the reduction for nonlinear or non-affine problems.

DEIM [!citep](chaturantabut2010nonlinear) removes that cost. It builds a POD basis $\boldsymbol{U}$ of residual snapshots and a selection operator $\boldsymbol{P}$ that picks a few interpolation rows, so the residual is reconstructed from only those entries,

\begin{equation}
\boldsymbol{r}(\boldsymbol{\mu}) \approx \boldsymbol{U}\left(\boldsymbol{P}^T\boldsymbol{U}\right)^{-1}\boldsymbol{P}^T\boldsymbol{r}(\boldsymbol{\mu}).
\end{equation}

The interpolation rows are chosen by the QDEIM point-selection algorithm, a pivoted-QR variant of the original greedy DEIM selection. The Jacobian is reduced the same way and projected onto the solution basis.

## Per-tag residual DEIM id=sec:multi-tag

At a converged solve the total residual is zero, $\boldsymbol{r} = \sum_t \boldsymbol{r}_t \approx \boldsymbol{0}$, so snapshots of the *total* residual carry no information and are useless for DEIM. The individual per-object contributions $\boldsymbol{r}_t$ — one per Kernel and per boundary condition — are not zero; they only cancel in the sum. Capturing the residual per tag therefore gives meaningful training data.

[AutoResidualTagAction.md] creates one residual vector tag per Kernel and boundary condition, and [ResidualContainer.md] snapshots each tag separately. [SerializedSnapshotTransfer.md] stores each tag's stream under the key `residual::<tag>`, and [DEIMRBMapping.md] builds a separate POD/DEIM basis per tag. The reduced residual is the affine sum over the retained tags,

\begin{equation}
\hat{\boldsymbol{r}}(\boldsymbol{\mu}) = \sum_{t} \left(\boldsymbol{\Phi}^T \boldsymbol{U}_t\right)\left(\boldsymbol{P}_t^T \boldsymbol{U}_t\right)^{-1} \boldsymbol{r}_t(\boldsymbol{\mu})\big|_{\boldsymbol{P}_t},
\end{equation}

which [InverseRB.md] evaluates at each Newton iteration.

## Parameterized Dirichlet boundary conditions id=sec:preset

A parameterized boundary value $g(\boldsymbol{\mu})$ is reproduced by the reduced model only when its boundary condition is *weakly* enforced. Set [!param](/BCs/DirichletBC/preset) `= false` on a parameterized [DirichletBC.md] so its residual $u - g(\boldsymbol{\mu})$ is nonzero at convergence and is captured by DEIM.

A strongly enforced (preset) Dirichlet condition writes its value directly into the solution vector and contributes an identically-zero residual at convergence. Its per-tag snapshots vanish, its POD basis is empty, and [DEIMRBMapping.md] drops the tag during training. This is the correct behavior — a zero residual has nothing to interpolate — but it means the parameter dependence of a preset boundary value is not represented in the reduced residual. Make such a boundary condition weakly enforced when its parameter must drive the reduced model.

## Training and evaluating a model

The [ROM](syntax/ROM/index.md) block builds the full training pipeline from one block:

!listing test/tests/rom/rom_action_equivalence/create_mapping.i block=ROM

The equivalent object-by-object pipeline — the sampler sub-application, the `[AutoResidualTag]` block ([AutoResidualTagAction.md]), the snapshot containers, [SerializedSnapshotTransfer.md], [ParallelSolutionStorage.md], [MappingReporter.md], and [DEIMRBMapping.md] — is shown in the training input:

!listing test/tests/rom/deim_multi_tag_train/train.i

The trained mapping is written to disk by [MappingOutput.md] and evaluated at new parameter samples with [InverseRB.md].
