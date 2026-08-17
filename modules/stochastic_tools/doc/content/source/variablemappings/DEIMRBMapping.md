# DEIMRBMapping

!syntax description /VariableMappings/DEIMRBMapping

## Overview

`DEIMRBMapping` builds the reduced operators for a Discrete Empirical Interpolation Method (DEIM) reduced-basis model. It combines a Proper Orthogonal Decomposition (POD) basis of the solution field (see [PODMapping.md]) with a DEIM approximation [!citep](chaturantabut2010nonlinear) of the parameter-dependent residual and Jacobian, so that both are reconstructed from a small set of sampled entries. The method and the training workflow are described in [modules/stochastic_tools/examples/deim_rom.md].

The [!param](/VariableMappings/DEIMRBMapping/variables) parameter lists the snapshot components the mapping is built from:

- exactly one `solution` component,
- one or more residual components — either the single token `residual`, or one `residual::<tag>` entry per residual tag produced by [AutoResidualTagAction.md],
- exactly one `jacobian` component.

The residual is split across tags so each Kernel and boundary-condition object contributes its own DEIM component; this replaces the earlier fixed `{solution, residual, jacobian}` triple. Supply one [!param](/VariableMappings/DEIMRBMapping/num_modes_to_compute) entry per listed component. A residual tag whose snapshots vanish at convergence — for example a preset [DirichletBC.md] — has an empty basis and is dropped during training, so at least one residual component must be non-degenerate.

The reduced residual is the affine sum over the retained residual components,

\begin{equation}
\hat{\boldsymbol{r}}(\boldsymbol{\mu}) = \sum_{c} \left(\boldsymbol{\Phi}^T \boldsymbol{U}_c\right)\left(\boldsymbol{P}_c^T \boldsymbol{U}_c\right)^{-1} \boldsymbol{r}_c(\boldsymbol{\mu})\big|_{\boldsymbol{P}_c},
\end{equation}

where $\boldsymbol{U}_c$ is the POD basis of residual component $c$, $\boldsymbol{P}_c$ selects its DEIM interpolation rows, $\boldsymbol{\Phi}$ is the solution basis, and $\boldsymbol{r}_c|_{\boldsymbol{P}_c}$ are the residual entries evaluated at the interpolation points. [InverseRB.md] evaluates this sum at each Newton iteration when the model is queried at a new sample.

## Example Input File Syntax

!listing test/tests/rom/deim_multi_tag_train/train.i block=VariableMappings

## Syntax

!syntax parameters /VariableMappings/DEIMRBMapping

!syntax inputs /VariableMappings/DEIMRBMapping

!syntax children /VariableMappings/DEIMRBMapping
