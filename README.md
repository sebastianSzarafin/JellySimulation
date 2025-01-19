# JellySimulation

![image.png](assets%2Fimage.png)

### About

This is the third project created during "Physical simulations in a virtual
environment" course, specialty **CAD/CAM Systems Design** of degree in *Computer Science
and Information Systems* at *Warsaw University of Technology*.

The project is a simulation of 64 mass points arranged in 4x4x4 cube (***jelly***) and connected
to each other by springs. Jelly's corner points are connected to the corners of the ***control frame***
with special springs of zero rest length. Such approach forces jelly to follow control frame
which can be freely moved and rotated by the user.

Jelly is inside ***collision frame*** that it can't get out of. The control frame can be freely
moved inside and outside collision frame.

Jelly is rendered as ***6 Bezier patches*** based on control points which are outer jelly's points.
Additionally, a simple rubber duck model is inserted in the jelly. Model is deformed using
***free form deformation (FFD)***

$$ F(u, v, w) = \sum_{i}^3\sum_{j}^3\sum_{k}^3 P^{ijk} B_i^3(u)B_j^3(v)B_k^3(w), $$

where $ (u,v,w) \in [0, 1]^3 $, $ P^{ijk} $ - jelly's control points, $ B_{ijk}^3 $ - Bernstein basis polynomials.

Shading is computed based on this definition of FFD transformation.

### Usage demo



### Build
The project is written in C++ using CMake as a build system and OpenGL for rendering.
In order to build it few development tools are needed:
- gcc and g++
- make
- cmake
- git

All code dependecies are managed within project. To download all external modules run inside git repository:
```
git submodule update --init --recursive
```

### Architecture

```
src/
|-- SymBase/
|   |-- ...
|-- SymSandbox
|   |-- ...
|-- main.cc
```

Project files (apart from external libraries) are located in *src* directory which is
further divided onto 2 subdirectories:
- *SymBase* - contains base of the application independent of simulation itself
  (core class definitions, renderer api, etc.), largely inspired by [espert-core](https://github.com/engineer-boys/espert-core)
  project.
- *SymSandbox* - contains simulation logic and uses *SymBase* as static library.

### External Libraries

All used libraries are stored in `external` directory. `GLAD` is used as a generated
file from [this](https://glad.dav1d.de/) web page. `GLFW`, `GLM`, `ImGui`,`ImPlot`, `tinyobjloader` are
used as submodules. Currently, *SymBase* only supports loading simple textures, and for
this purpose`stb` is used as single header library.

| Library                                            | Licence                                                                  |
|----------------------------------------------------|--------------------------------------------------------------------------|
| [GLAD](https://github.com/Dav1dde/glad)            | [MIT](https://github.com/Dav1dde/glad?tab=License-1-ov-file#readme)      |
| [GLFW](https://github.com/glfw/glfw)               | [Zlib](https://github.com/glfw/glfw?tab=Zlib-1-ov-file#readme)           |
| [GLM](https://github.com/g-truc/glm)               | [MIT](https://github.com/g-truc/glm?tab=License-1-ov-file)               |
| [ImGui](https://github.com/ocornut/imgui)          | [MIT](https://github.com/ocornut/imgui?tab=MIT-1-ov-file#readme)         |
| [ImPlot](https://github.com/epezent/implot)        | [MIT](https://github.com/epezent/implot?tab=MIT-1-ov-file)               |
| [Stb](https://github.com/nothings/stb/tree/master) | [MIT](https://github.com/nothings/stb/tree/master?tab=License-1-ov-file) |
| [tinyobjloader]()                                  || 