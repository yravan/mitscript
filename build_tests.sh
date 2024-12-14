# To build and run,
# /src run ./build_tests.sh && ./vm-tests

cmake -D CMAKE_BUILD_TYPE=Release -D BUILD_TESTS=ON -S . -B cmake-build-test
cmake --build cmake-build-test

rm -f vm-tests
BUILD_DIR=$(realpath cmake-build-test)
ln -s $BUILD_DIR/test/vm/vm-tests vm-tests