# InverseRB

!syntax description /UserObjects/InverseRB

## Overview

`InverseRB` evaluates a trained DEIM reduced-basis model at a new parameter sample. It solves the reduced nonlinear system with a Newton iteration: each iteration assembles the full-order residual and Jacobian, projects them onto the reduced basis of the supplied [!param](/UserObjects/InverseRB/mapping) (a [DEIMRBMapping.md]), and updates the reduced solution coordinates. The reduced residual is the affine sum over the mapping's residual components, so a per-tag [DEIMRBMapping.md] accounts for each Kernel and boundary-condition contribution separately; see [modules/stochastic_tools/examples/deim_rom.md].

Control the iteration with [!param](/UserObjects/InverseRB/max_iter), [!param](/UserObjects/InverseRB/tolerance), and [!param](/UserObjects/InverseRB/relaxation_factor). The loop stops when the reduced residual $L_2$ norm falls to or below [!param](/UserObjects/InverseRB/tolerance).

## Example Input File Syntax

!listing test/tests/rom/inverse_rb_multi_tag/main_rb.i block=UserObjects

## Syntax

!syntax parameters /UserObjects/InverseRB

!syntax inputs /UserObjects/InverseRB

!syntax children /UserObjects/InverseRB
