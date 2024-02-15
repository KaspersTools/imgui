How to build:

1.  clone the repo
```bash
git clone git@github.com:KaspersTools/imgui.git --recursive 
```

2. go into folder
```bash
cd imgui
```

3. make build folder or open with CLion
```bash
mkdir build && cd build
```

4. run cmake
```bash
cmake .. -DHBUI_BUILD_EXAMPLE:BOOL=ON -DHBUI_GLFW:BOOL=ON -DHBUI_WITH_DEBUG_WINDOW:BOOL=ON
```

5. build with example
```bash
cmake --build . --target ImVk-example -j 8
```

6. run
```bash
./bin/ImVk-example
```

