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
PARSER_BASE=interpreter_${name}/mitscript-parser-${name}

# Public/private tests
echo "Running a1 public tests..."
PARSER=../cmake-build-grading/${PARSER_BASE}
cd tests/ && ./a1/run-tests.sh public ${PARSER} && cd ..
echo "----------------------------------------"

# Personal tests
echo "Running a1 personal tests..."
ROOT="."
PARSER=./cmake-build-grading/${PARSER_BASE}


NAMES=("joseph" "yajvan" "rom")

for name in "${NAMES[@]}"; do
    TESTS=./tests/a1/personal_${name}
    
    TOTAL=0
    COUNT=0

    for filename in "$TESTS"/test*.mit; do
        OUT=$($PARSER $filename)
        CODE=$?
        if [[ $CODE -ne 0 ]] || [[ $OUT = *"Error"* ]]; then
            echo "  Fail: $filename (exit code $CODE)"
        else
            # echo "Passed: $(basename $filename)"
            COUNT=$((COUNT + 1))
        fi
        TOTAL=$((TOTAL + 1))
    done

    echo "Passed $COUNT out of $TOTAL personal_${name} tests"
done
