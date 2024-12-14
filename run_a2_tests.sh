#!/bin/bash
# set -x

valid_flags=(--joseph --rom --yajvan)
error_exit() {
    echo "Error: Invalid flag. Use one of the following:"
    printf '%s\n' "${valid_flags[@]}"
    exit 1
}

# Check that there is exactly 1 argument
if [ "$#" -ne 1 ]; then
    error_exit
fi

flag="$1"
name="${flag/--/}"
INTERPRETER_BASE=interpreter_${name}/mitscript-${name}

# Public/private tests
echo "Running a2 public tests..."
INTERPRETER=../../cmake-build-grading/${INTERPRETER_BASE}
cd tests/a2 && ./run-tests.sh ${INTERPRETER} && cd ../..
echo "----------------------------------------"

# Personal tests
echo "Running a2 personal tests..."
ROOT="."
INTERPRETER=./cmake-build-grading/${INTERPRETER_BASE}
TIMEOUT=60

NAMES=("joseph" "yajvan" "rom")

for name in "${NAMES[@]}"; do
    TESTS=./tests/a2/personal_${name}
    TEST_DIR=$ROOT/tests/a2/personal_${name}


    TOTAL=0
    COUNT=0

    for filename in $TEST_DIR/test*.mit; do
        # echo $filename
        timeout $TIMEOUT $INTERPRETER $filename > tmp.out
        CODE=$?

        # expected_output="$TEST_DIR/$(basename $filename.mit).out"
        expected_output="$TEST_DIR/$(basename "$filename" .mit).out"
        if diff tmp.out "$expected_output"; then
            COUNT=$((COUNT+1))
        else
            echo "Fail: $(basename $filename) (exit code $CODE)"
        fi
        TOTAL=$((TOTAL+1))
        rm -f tmp.out
    done

    echo "Passed $COUNT out of $TOTAL personal_${name} tests"
done
