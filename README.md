# SpinningTopSimulation

TODO:

### About

This is the third project created during "Physical simulations in a virtual
environment" course, specialty **CAD/CAM Systems Design** of degree in *Computer Science
and Information Systems* at *Warsaw University of Technology*.

TODO:

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
file from [this](https://glad.dav1d.de/) web page. `GLFW`, `GLM`, `ImGui`,`ImPlot` are
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