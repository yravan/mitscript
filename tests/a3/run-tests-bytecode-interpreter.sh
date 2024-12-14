#!/bin/bash
# >>> ./run-tests-bytecode-interpreter.sh ../../reference/mitscript

ROOT=".."
INTERPRETER="$1"
TIMEOUT=10

TOTAL=0
COUNT=0

SKIP_TESTS=(
    "good14.mitbc"
)

for filename in $ROOT/a3/a2-mitbc/good*.mitbc; do
    basename_file=$(basename "$filename")
    if [[ " ${SKIP_TESTS[@]} " =~ " ${basename_file} " ]]; then
        # echo "Skipping: $basename_file"
        continue
    fi

    # echo $filename
    timeout $TIMEOUT $INTERPRETER -b $filename > tmp.out
    CODE=$?
    if diff tmp.out $ROOT/a3/a2-mitbc/$(basename $filename).out; then
        echo "Passed: $(basename $filename)"
        COUNT=$((COUNT+1))
    else
        echo "Fail: $(basename $filename) (exit code $CODE)"
    fi
    TOTAL=$((TOTAL+1))
    rm -f tmp.out
done

SKIP_TESTS=(
    "good4.mitbc"
    "good5.mitbc"
)

for filename in $ROOT/a3/a1-mitbc/good*.mitbc; do
    basename_file=$(basename "$filename")
    if [[ " ${SKIP_TESTS[@]} " =~ " ${basename_file} " ]]; then
        # echo "Skipping: $basename_file"
        continue
    fi

    # echo $filename
    timeout $TIMEOUT $INTERPRETER -b $filename > tmp.out
    CODE=$?
    if diff tmp.out $ROOT/a3/a1-mitbc/$(basename $filename).out; then
        # echo "Passed: $(basename $filename)"
        COUNT=$((COUNT+1))
    else
        echo "Fail: $(basename $filename) (exit code $CODE)"
    fi
    TOTAL=$((TOTAL+1))
    rm -f tmp.out
done


for filename in $ROOT/a3/public/mitbc/bad*.mitbc; do
    # echo $filename
    timeout $TIMEOUT $INTERPRETER -b $filename > tmp.out
    CODE=$?
    if [[ $CODE -eq 0 ]] && [[ $OUT != *"Error"* ]]; then
        echo "Fail: $(basename $filename) (exit code $CODE)"
    else
        # echo "Passed: $(basename $filename)"
        COUNT=$((COUNT + 1))
    fi

    TOTAL=$((TOTAL+1))
    rm -f tmp.out
done


echo "Passed $COUNT out of $TOTAL bytecode public tests"

