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

1. run this into your terminal:

```sh
$ mkdir build; cd build; cmake ..;
```

2. build with `make`.

```sh
$ make install
```

## Run

```sh
$ ./src/main
```

## TODO:
* provide build rules/ jobs
