# Uncomment if using make
# make -C vm; exit

# CMake build process
: ${CMAKE_DIR:=cmake-build-grading}

# Check for --debug flag
if [[ "$1" == "--debug" ]]; then
    BUILD_TYPE="Debug"
    CMAKE_DIR=cmake-build-debug
else
    # Check for --debug flag
    if [[ "$1" == "--mem-debug" ]]; then
        BUILD_TYPE="MemDebug"
        CMAKE_DIR=cmake-build-mem-debug
    else
        BUILD_TYPE="Release"
    fi
fi

cmake -D CMAKE_BUILD_TYPE=$BUILD_TYPE -S . -B $CMAKE_DIR
cmake --build $CMAKE_DIR

# create symlinks, so that executables are accessible in the source tree
rm -f vm/mitscriptc vm/mitscript
BUILD_DIR=$(realpath $CMAKE_DIR)
ln -s $BUILD_DIR/vm/mitscriptc vm/mitscriptc
ln -s $BUILD_DIR/vm/mitscriptbc vm/mitscript
