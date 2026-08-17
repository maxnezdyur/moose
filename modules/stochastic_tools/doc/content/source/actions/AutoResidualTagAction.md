# AutoResidualTagAction

!syntax description /AutoResidualTag/AutoResidualTagAction

## Overview

The `[AutoResidualTag]` block prepares a full-order model for DEIM residual snapshotting. It creates one residual vector tag per Kernel and boundary condition, routes each object's residual into its own tag, and adds the [SolutionContainer.md], [ResidualContainer.md], and [JacobianContainer.md] snapshot reporters (all with `execute_on = TIMESTEP_END`). [ROMSetupAction.md] enables this block automatically on its sampler sub-application; add it by hand only when building a training pipeline object by object.

Each residual tag is named `residual_tag_<name>`, where `<name>` is the input-block name of the Kernel or boundary condition, so residual-object names must be unique. A single Jacobian matrix tag, `jacobian_tag`, collects the full Jacobian. The block requires no framework changes: it attaches the tags through each object's `extra_vector_tags` and `extra_matrix_tags` construction parameters. Tagging runs before the variables are added, so each variable reserves storage for the new tags.

Set [!param](/AutoResidualTag/enable) `= false` to keep the block in the input while disabling tagging; [ROMSetupAction.md] toggles it through this parameter on the command line.

## Example Input File Syntax

The block takes no required parameters; the empty block enables tagging with the default [!param](/AutoResidualTag/enable) `= true`.

!listing test/tests/rom/residual_tags_nonzero/residual_tags_nonzero.i block=AutoResidualTag

## Syntax

!syntax parameters /AutoResidualTag/AutoResidualTagAction
