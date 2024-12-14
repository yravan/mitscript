#!/bin/bash
# set -x

COMPILER=../../cmake-build-grading/vm/mitscriptc
INTERPRETER=../../cmake-build-grading/vm/mitscriptbc

echo "Running interpreter bytecode mode: a1/a2/a3 public tests..."
cd tests/a3 && ./run-tests-bytecode-interpreter.sh ${INTERPRETER} && cd ../..
echo "----------------------------------------"

# echo "Running interpreter source mode: a2 public + personal tests..."
# COMPILER=./cmake-build-grading/vm/mitscriptc
# INTERPRETER=./cmake-build-grading/vm/mitscript

# TIMEOUT=90
# TEST_DIR=./tests/a2/public
# RESULTS=out.txt

# PUBLIC_TOTAL=0
# PUBLIC_COUNT=0

# SKIP_TESTS=(
#     "good14.mit" # Function equality
# )

# for filename in $TEST_DIR/good*.mit; do
#     basename_file=$(basename "$filename")
#     if [[ " ${SKIP_TESTS[@]} " =~ " ${basename_file} " ]]; then
#         # echo "Skipping: $basename_file"
#         continue
#     fi

#     # echo $filename
#     $(timeout $TIMEOUT $INTERPRETER -s $filename > tmp.out)
#     CODE=$?

#     expected_output="$TEST_DIR/$(basename $filename).out"
#     if diff -q tmp.out $expected_output > /dev/null; then
#         PUBLIC_COUNT=$((PUBLIC_COUNT+1))
#         # echo "Pass: $(basename $filename) (exit code $CODE)"
#     else
#         echo "Fail: $(basename $filename) (exit code $CODE)"
#         # echo "    Diff:"
#         # diff -u "$expected_output" tmp.out | sed 's/^/    /'
#     fi
#     PUBLIC_TOTAL=$((PUBLIC_TOTAL+1))
#     rm -f tmp.out tmp.err
# done
# echo "Passed $PUBLIC_COUNT out of $PUBLIC_TOTAL public tests"


# NAMES=("joseph" "yajvan" "rom")
# for name in "${NAMES[@]}"; do
#     TESTS=./tests/a2/personal_${name}
#     TEST_DIR=./tests/a2/personal_${name}

#     TOTAL=0
#     COUNT=0

#     for filename in $TEST_DIR/test*.mit; do
#         # echo $filename
#         timeout $TIMEOUT $INTERPRETER -s $filename > tmp.out
#         CODE=$?

#         expected_output="$TEST_DIR/$(basename "$filename" .mit).out"
    
#         if diff -q tmp.out $expected_output > /dev/null; then
#             COUNT=$((COUNT+1))
#             # echo "Pass: $(basename $filename) (exit code $CODE)"
#         else
#             echo "Fail: $(basename $filename) (exit code $CODE)"
#             # echo "    Diff:"
#             # diff -u "$expected_output" tmp.out | sed 's/^/    /'
#         fi
#         TOTAL=$((TOTAL+1))
#         rm -f tmp.out
#     done

#     echo "Passed $COUNT out of $TOTAL personal_${name} tests"
# done


# TOTAL=0
# COUNT=0
# TEST_DIR=/src/tests/a3/personal
# for filename in $TEST_DIR/test*.mit; do
#     # echo $filename
#     timeout $TIMEOUT $INTERPRETER -s $filename > tmp.out
#     CODE=$?

#     expected_output="$TEST_DIR/$(basename "$filename" .mit).out"

#     if diff -q tmp.out $expected_output > /dev/null; then
#         COUNT=$((COUNT+1))
#         # echo "Pass: $(basename $filename) (exit code $CODE)"
#     else
#         echo "Fail: $(basename $filename) (exit code $CODE)"
#         # echo "    Diff:"
#         # diff -u "$expected_output" tmp.out | sed 's/^/    /'
#     fi
#     TOTAL=$((TOTAL+1))
#     rm -f tmp.out
# done

# echo "Passed $COUNT out of $TOTAL group personal tests"

# # # echo "Done" >> $RESULTS
