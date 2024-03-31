# Sample VSCode Raylib project

This repository provides a basic structure to kickstart your development journey with raylib using Visual Studio Code. Get started with raylib, a simple and easy-to-use library for creating 2D and 3D games, and expand your project from this foundation. Happy coding!

Feel free to submit improvement PRs.

## Extensions used

* twxs.cmake
* josetr.cmake-language-support-vscode
* ms-vscode.cmake-tools
* llvm-vs-code-extensions.vscode-clangd
* xaver.clang-format
* vscode-icons-team.vscode-icons
* ms-vscode.makefile-tools


## Build instructions
This project uses clang as a compiler. If you wish to modify that behaviour, look into the root CMakeLists.txt.

For windows, you need llvm-mingw installed and added to your PATH.
For downloads see: https://github.com/mstorsjo/llvm-mingw/releases

Unzip and add the `/bin` directory to your PATH.

### Steps:
1. run this into your terminal:

Linux:
```sh
$ mkdir build; cd build; cmake ..;
```

Windows:
```sh
$ mkdir build
$ cd build
$ cmake -G "MinGW Makefiles" -DCMAKE_C_COMPILER=clang.exe ..
```

2. build

From the `build` directory:
```sh
$ cmake --build .
```

## Run

Linux:
```sh
$ ./src/main
```

Windows:
```sh
$ ./src/main.exe
```

## TODO:
* provide build rules/ jobs
