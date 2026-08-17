# ROMSetupAction

!syntax description /ROM/ROMSetupAction

## Overview

The `[ROM]` block trains a DEIM reduced-order model from a single input block. Point it at a full-order-model input with [!param](/ROM/input), name the perturbed sub-application parameters with [!param](/ROM/parameters) (in the [!param](/ROM/sampler) column order), and the action builds the whole training pipeline. The design follows [the ParameterStudy syntax](syntax/ParameterStudy/index.md), which builds a sampling study from one block. The reduced-model theory is described in [modules/stochastic_tools/examples/deim_rom.md].

Set [!param](/ROM/num_modes) with exactly three entries — the number of POD modes for the solution, for each residual component, and for the Jacobian. The residual entry is applied to every residual component, so it is independent of the number of Kernel and boundary-condition objects. If unset, one mode is computed per component.

## Generated Objects

[tab:rom_objects] lists the objects the `[ROM]` block constructs. They reproduce, programmatically, the object-by-object pipeline in [modules/stochastic_tools/examples/deim_rom.md].

!table id=tab:rom_objects caption=Objects created by the `[ROM]` block.
| Base | Type | Purpose |
| - | - | - |
| Action | [StochasticToolsAction.md] | driver, created only if no `[StochasticTools]` block is present |
| [MultiApp](MultiApps/index.md) | [SamplerFullSolveMultiApp.md] | runs the full-order model per sample in `batch-reset` mode |
| [Control](Controls/index.md) | [MultiAppSamplerControl.md] | applies the sampled [!param](/ROM/parameters) on the sub-application command line |
| [Transfer](Transfers/index.md) | [SerializedSnapshotTransfer.md] | serializes the solution, residual, and Jacobian snapshots to the main app |
| [Transfer](Transfers/index.md) | [MultiAppReporterTransfer.md] | copies the Jacobian sparsity indices from the first sub-application |
| [Reporter](Reporters/index.md) | [ParallelSolutionStorage.md] | stores the transferred snapshots |
| [Reporter](Reporters/index.md) | [MappingReporter.md] | builds the mapping from the stored snapshots |
| [Reporter](Reporters/index.md) | [JacobianContainer.md] | holds the transferred Jacobian sparsity indices |
| [VariableMapping](VariableMappings/index.md) | [DEIMRBMapping.md] | the trained DEIM reduced-basis mapping |
| [Output](syntax/Outputs/index.md) | [MappingOutput.md] | writes the trained mapping to disk |

## Residual Tagging and Parameterized Boundary Conditions

With [!param](/ROM/auto_tag_residual) at its default `true`, the action enables the sub-application `[AutoResidualTag]` block (through an `AutoResidualTag/enable=true` command-line argument), so [AutoResidualTagAction.md] creates one residual tag per Kernel and boundary condition and each becomes its own DEIM component. With `auto_tag_residual = false`, the residual is treated as a single component and the sub-application must define its own snapshot containers.

A parameterized [DirichletBC.md] is reproduced by the reduced model only when it is weakly enforced. Set [!param](/BCs/DirichletBC/preset) `= false` on that boundary condition so its parameter-dependent residual $u - g(\mu)$ is captured by DEIM. A strongly enforced (preset) Dirichlet condition contributes an identically-zero residual at convergence: its per-tag basis is empty and is dropped from the reduced residual, so its parameter dependence would otherwise be lost. This contract is discussed further in [modules/stochastic_tools/examples/deim_rom.md#sec:preset].

## Example Input File Syntax

!listing test/tests/rom/rom_action_equivalence/create_mapping.i block=ROM

The equivalent hand-built pipeline is in `main_rb.i` in the same test directory.

!syntax parameters /ROM/ROMSetupAction
