# 3D MBB Beam with a PDE Filter

In this example is included to show that TROUT works in 3D too. Only new material not covered in the previous
example will be covered here [2D Topology Optimization with Radial Average Filter](examples/2d_mbb.md), [2D Topology Optimization with PDE Filter and Boundary Penalty](examples/2d_mbb_pde.md), [3D Topology Optimization with PDE Filter](examples/3d_mbb.md).

In this example, the mesh block is 3D.

!listing test/tests/materials/ComplianceSensitivity/3d_mbb.i
         block=Mesh id=mesh_var_block
         caption=MBB `Mesh` block

The results below show the final design for the 3D MBB problem.
!media examples/images/3d_mbb.mp4 style=width:100%; controls=False autoplay=True loop=True



