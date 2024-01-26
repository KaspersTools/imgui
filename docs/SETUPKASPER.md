Install before trying to build
===

1. Install cmake && make
2. Install vulkan

How to setup 
===

1. Create a folder on somewhere on your computer and name it something
2. Open this folder in a terminal
3. Clone this repository
```bash
git clone --recursive git@github.com:KaspersTools/imgui.git
```

4. Clone the glfw repository !IMPORTANT to use this repo for using the custom title bar

```bash
git clone git@github.com:KaspersTools/glfw.git
```

5. Go into the imgui folder
```bash
cd imgui
```

6. Create a build folder
```bash
mkdir "build"
```

7. Open the build folder
```bash
cd build
```

8. Run cmake
```bash
cmake ..
```

9. Run make
```bash
make
```

10. Go into the bin folder
```bash
cd bin
```

11. Run the program
```bash
./KDB_ImGui
```