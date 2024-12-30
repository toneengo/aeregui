WIP.
texgui is a minimal C++ library, which can use textures to render GUI elements for OpenGL games.
inspired by: nanogui, imgui

# Build
## Dependencies
- Ninja (optional)
## Windows
### CMake (Ninja)
1. Install vcpkg.
2. Set the environment VCPKG_ROOT to the path of the vcpkg installation.
```
$ setx VCPKG_ROOT C:\<path>\vcpkg\
```
3. Add the path of the vcpkg installation to your PATH.
```
$ setx PATH "%PATH%;C:\<path>\vcpkg\"
```
4. Install all dependencies.
```
$ vcpkg install glm
```
5. Clone this repository.
```
$ git clone https://github.com/toneengo/texgui.git
$ cd .\texgui\
```
6. Generate the build files. You may use another generator.
```
$ mkdir build
$ cmake -S . -B build -G "Ninja"
$ cmake --build build
```
7. Copy the output file (build\texgui.lib) into your project, and link it.
## Linux (CMake)
1. Install the dependencies with your package manager.
### Arch Linux:
```
sudo pacman -S glm
```
2. Clone this repository.
```
$ git clone https://github.com/toneengo/texgui.git
$ cd .\texgui\
```
3. Generate the build files. You may use another generator.
```
$ mkdir build
$ cmake -S . -B build -G "Ninja"
$ cmake --build build
```
4. Copy the output file (build\texgui.lib) into your project, and link it.
