# ADLineHeatSinkRayKernel

!syntax description /RayKernels/ADLineHeatSinkRayKernel

## Description

`ADLineHeatSinkRayKernel` is a line-integrated Newton-cooling sink for a temperature variable, applied along a ray defined by a [RepeatableRayStudy.md]. Mesh independence is achieved by construction: the ray tracer integrates per-element-segment as it crosses the mesh, and each sub-segment receives its own Gauss-Legendre quadrature rule, ensuring no element on the line is skipped.

The continuous residual contribution over a polyline $\Gamma$ is

!equation
R_j = \int_\Gamma h(x, t) \cdot (T(x) - T_{sink}(x, t)) \cdot \psi_j(x)~d\ell\,,

where $h$ is the heat transfer coefficient, $T_{sink}$ is the sink temperature, $T$ is the temperature variable, and $\psi_j$ are the test functions. For each element $K$ the ray crosses, the framework lays a Gauss rule on the sub-segment $\Gamma \cap K$ and reinitializes shape functions at those quadrature points. Automatic differentiation provides the Jacobian.

## Heat Transfer Coefficient and Sink Temperature

The heat transfer coefficient [!param](/RayKernels/ADLineHeatSinkRayKernel/h) and sink temperature [!param](/RayKernels/ADLineHeatSinkRayKernel/T_sink) are required `FunctionName` parameters and may depend on space and time. For a constant value, wrap a scalar in a [ConstantFunction.md].

## Ray Definition

The polyline geometry is supplied by a [RepeatableRayStudy.md] user object, specified via the inherited [!param](/RayKernels/ADLineHeatSinkRayKernel/study) parameter. The study takes [!param](/UserObjects/RepeatableRayStudy/start_points) and [!param](/UserObjects/RepeatableRayStudy/end_points) as a sequence of chord endpoints, defining the line(s) along which the sink is applied. The kernel automatically associates itself with rays created by the study; execution scheduling is handled by the framework.

## Example Input File Syntax

!listing modules/heat_transfer/test/tests/raykernels/line_heat_sink/line_heat_sink.i block=UserObjects RayKernels


!syntax parameters /RayKernels/ADLineHeatSinkRayKernel

!syntax inputs /RayKernels/ADLineHeatSinkRayKernel

!syntax children /RayKernels/ADLineHeatSinkRayKernel
