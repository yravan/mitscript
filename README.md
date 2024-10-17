# MITScript Project

## Overview

This project involves the implementation of a lexer, parser, and interpreter for the MITScript language, based on a provided formal grammar. The project is split into two phases: the creation of a parser and the development of an interpreter.

### Phase I: Parser
The parser takes as input a MITScript program and outputs an Abstract Syntax Tree (AST) if the program is syntactically valid.

### Phase II: Interpreter
The interpreter executes the MITScript program based on the semantic rules provided, producing the appropriate output.

## Language Specification

The detailed specification of the MITScript language is included in the [`specification.pdf`](./specification.pdf) file located in the root of this repository. This document contains the formal grammar, syntax, and semantics of MITScript, which are essential for understanding and developing the lexer, parser, and interpreter for the project.

### Key Highlights of the MITScript Language:

1. **Grammar**: 
   - The MITScript language is defined using a formal context-free grammar, which outlines the structure and allowable constructs.
   - The core components include statements such as `Assignment`, `IfStatement`, `WhileLoop`, `Return`, and expressions like `Boolean`, `Arithmetic`, and `Record`.

2. **Syntax**:
   - MITScript supports basic control flow constructs (`if`, `while`), global declarations, and function definitions.
   - Expressions include arithmetic, boolean logic, and record structures.

3. **Semantics**:
   - The document describes how each construct behaves when executed. This includes the evaluation of expressions, assignment of variables, and function calls.

## Setup

You can either manually install dependencies (recommended), or use a Docker
container.

### Manually install dependencies

Install the following dependencies on your local system.

For **archlinux**:

```sh
$ pacman -S --needed base-devel git cmake antlr4 antlr4-runtime
```

For **debian**-based systems:

```sh
$ apt install build-essential git cmake antlr4 libantlr4-runtime-dev flex bison
# If using Ubuntu 24.04 or similar, there is a version incompatibility with the
# provided antlr4. See the last three lines of the Dockerfile for a method of
# dealing with this (you'll want to uninstall antlr4 as well).
```

In both cases, other potentially useful tools in debugging are `time`, `gdb` and
`valgrind`.

### Docker

You'll first need to install docker (`pacman -S --needed docker docker-buildx` in
archlinux, and `apt install docker.io docker-buildx` in debian-based
distributions).

To start a Docker container, run the following commands in this directory:

```sh
$ docker buildx build -t mit-61120 .
$ docker run -it \
    --cap-add=SYS_PTRACE --security-opt seccomp=unconfined \
    --name=61120 --mount type=bind,source=${PWD},target=/src \
     mit-61120 bash
```

This will start the container, map the current repository in it, and finally pull
up a shell inside the container.
Once the shell starts, just write commands like you were using a regular Linux
terminal.
The `/src` directory in the container is mapped to this directory on your
machine.

```sh
root@221b746bd366:/# cd /src
root@221b746bd366:/src#
```

The Docker container stops automatically when your shell exits.
You can restart it from where you left off, with the command:

```sh
$ docker start -ai 61120
root@221b746bd366:/# cd /src
```

If you want to customize your Docker image, edit the Dockerfile and build by
running the command below.
Make sure you specify the same tag for the build and execution (`mit-61120` in
the above).

```sh
# inside your project root
docker build . -t <image tag>
```

### Updating

To pull updates from this repository, go to your own repository, and run:

```sh
# do this every time the skeleton repository is updated
git pull origin master
```

## Build and Run

### Build

The `build.sh` script will be used for building when grading, so you should make
sure it can build your code.
A `build.sh` is provided for you if you're planning to use CMake - otherwise, you
should modify it to fit your needs.

You can also use the script to build the code locally, or you can run the
commands directly as described below.

#### CMake

If you'd like to learn more about CMake, you can read the first few chapters of
the [tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/index.html).
If you have more questions, ask on Piazza!

To avoid starting every build from scratch, you can run the CMake build commands
directly.

```sh
# This is called the configuration step and it prepares the build system inside cmake-build-grading.
# Rerun only if you deleted the build tree (the cmake-build-* directory), or if you want to change CMake options (-D etc.)
cmake -D CMAKE_BUILD_TYPE=Release -S . -B cmake-build-grading

# Run this in all other scenarios. It will rerun the configuration step, and then perform the incremental build
cmake --build cmake-build-grading

# clean compiled files
cmake --build cmake-build-grading --target clean

# only build a specific target (e.g. interpreter):
cmake --build cmake-build-grading --target mitscript

# clean and build
cmake --build cmake-build-grading --target mitscript --clean-first
```

This will put the executables inside `cmake-build-grading/interpreter/*`.
You can use the symlinks that `build.sh` creates, but you don't have to.

##### Further Customization

CMake has a lot of customization options. E.g. to compile without optimization
and with debugging symbols, do the following:

```sh
# we can keep multiple builds around with different options by
# storing them in separate directories.
cmake -D CMAKE_BUILD_TYPE=Debug -S . -B cmake-build-debug
cmake --build cmake-build-debug
```

If you want to add compile options or flags, use:

- [`target_compile_options`](https://cmake.org/cmake/help/latest/command/target_compile_options.html)
- [`target_compile_definitions`](https://cmake.org/cmake/help/latest/command/target_compile_definitions.html)
- etc.

Make sure you're looking at documentation for your version of CMake (`cmake
--version`).

If you want to run with the Ninja backend for better compilation speed,
first install it using one of:

```sh
$ pacman -S ninja
$ apt install ninja-build
```

Then, specify it with the `-G` option during the configuration step;

```sh
# build step stays the same, just make sure Ninja is installed.
cmake -D CMAKE_BUILD_TYPE=Debug -G Ninja -S . -B cmake-build-debug
```

##### Building Tests

By default, tests won't build.
To build them, add `-D BUILD_TESTS=ON` (before the `-S` flag) to the configure step:

```sh
cmake -D CMAKE_BUILD_TYPE=Debug -D BUILD_TESTS=ON -S . -B cmake-build-debug
```

#### Make

Uncomment line 2 of `build.sh`, and you're all set. To run directly,
just run `make` inside the `interpreter` directory.

Unit tests are not currently supported when using make.

#### IDEs (CLion, Visual Studio Code, etc.)

These will usually work with CMake right out of the box.

### Run

Whether your parser is inside `interpreter/` or `cmake-build-*/interpreter`, invoke it as such:

```sh
[path/to/executable]/mitscript-parser [path/to/input/file]
```

#### Running Tests

To run the provided tests, please see the `tests/[phase]/run-tests.sh` scripts. The top of each script contains instructions for running the tests.

#### Unit tests

To run parser unit tests:

- Create a test file (like `test/interpreter/example-test.cpp`)
- Add the test file to the correct target in `CMakeLists.txt`
- Build the tests (described above)
- Run the tests:

```sh
cmake-build-debug/test/interpreter/parser-tests [flags]
```
