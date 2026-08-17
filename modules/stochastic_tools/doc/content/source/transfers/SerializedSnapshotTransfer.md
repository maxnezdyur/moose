# SerializedSnapshotTransfer

!syntax description /Transfers/SerializedSnapshotTransfer

## Overview

`SerializedSnapshotTransfer` moves the per-sample solution, residual, and Jacobian snapshots collected on a sampler sub-application into a [ParallelSolutionStorage.md] reporter on the main application, where a [DEIMRBMapping.md] is trained from them. Name the sub-application containers with [!param](/Transfers/SerializedSnapshotTransfer/solution_container) (a [SolutionContainer.md]), [!param](/Transfers/SerializedSnapshotTransfer/residual_container) (a [ResidualContainer.md]), and [!param](/Transfers/SerializedSnapshotTransfer/jacobian_container) (a [JacobianContainer.md]), and the destination with [!param](/Transfers/SerializedSnapshotTransfer/parallel_storage).

The solution and Jacobian are each transferred as a single stream, keyed `solution` and `jacobian`. The residual is transferred as one stream per tag held by the [ResidualContainer.md], each keyed `residual::<tag>`; a single-tag container instead produces one stream keyed `residual`. The [DEIMRBMapping.md] [!param](/VariableMappings/DEIMRBMapping/variables) list names these same keys.

When [!param](/Transfers/SerializedSnapshotTransfer/serialize_on_root) is enabled, each snapshot is gathered onto the root processor of its sub-application before transfer, which aligns with the sampler-reporter design but can imbalance the workload; when disabled, the samples are distributed across the sub-application ranks.

## Example Input File Syntax

!listing test/tests/rom/multi_tag_snapshots/multi_tag_main.i block=Transfers

## Syntax

!syntax parameters /Transfers/SerializedSnapshotTransfer

!syntax inputs /Transfers/SerializedSnapshotTransfer

!syntax children /Transfers/SerializedSnapshotTransfer
