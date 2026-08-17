# JacobianContainer

!syntax description /Reporters/JacobianContainer

## Overview

`JacobianContainer` collects the row-concatenated nonzero entries of a tagged Jacobian matrix into a snapshot container, appending one snapshot each time it executes. It also records the sparse `(row, column)` index map in the reporter named by [!param](/Reporters/JacobianContainer/jac_indices_reporter_name), which the reduced model uses to align every snapshot to the same sparsity pattern. Select the matrix tag with [!param](/Reporters/JacobianContainer/tag_name). These snapshots are the DEIM training data for the Jacobian component of a [DEIMRBMapping.md]; see [modules/stochastic_tools/examples/deim_rom.md].

The tagged matrix is only populated after the nonlinear solve, so [!param](/Reporters/JacobianContainer/execute_on) must not include `NONLINEAR`.

## Example Input File Syntax

!listing test/tests/reporters/res_jac_storage/sub.i block=Reporters

## Syntax

!syntax parameters /Reporters/JacobianContainer

!syntax inputs /Reporters/JacobianContainer

!syntax children /Reporters/JacobianContainer
