# ResidualContainer

!syntax description /Reporters/ResidualContainer

## Overview

`ResidualContainer` collects the distributed residual vector into a snapshot container, appending one snapshot each time it executes. These snapshots are the DEIM training data for the residual components of a [DEIMRBMapping.md]; see [modules/stochastic_tools/examples/deim_rom.md].

Select which residual tags to collect with [!param](/Reporters/ResidualContainer/tag_names): the container writes one snapshot stream per tag. When neither [!param](/Reporters/ResidualContainer/tag_names) nor [!param](/Reporters/ResidualContainer/tag_name) is set, it collects every residual vector tag stored by the nonlinear system. The scalar [!param](/Reporters/ResidualContainer/tag_name) collects a single tag as one stream and is mutually exclusive with [!param](/Reporters/ResidualContainer/tag_names). [AutoResidualTagAction.md] sets [!param](/Reporters/ResidualContainer/tag_names) automatically, one entry per tagged Kernel and boundary condition.

## Example Input File Syntax

The example below uses the scalar [!param](/Reporters/ResidualContainer/tag_name) form to collect a single tag; [AutoResidualTagAction.md] builds the multi-tag form programmatically.

!listing test/tests/reporters/res_jac_storage/sub.i block=Reporters

## Syntax

!syntax parameters /Reporters/ResidualContainer

!syntax inputs /Reporters/ResidualContainer

!syntax children /Reporters/ResidualContainer
