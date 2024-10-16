#!/bin/bash
# >>> ./run-tests.sh ../../reference/mitscript

ROOT=".."
INTERPRETER="$1"
TIMEOUT=300

TOTAL=0
COUNT=0

for filename in $ROOT/a2/personal/test*.mit; do
    echo $filename
    timeout $TIMEOUT $INTERPRETER $filename > tmp.out
    timeout $TIMEOUT $INTERPRETER $filename > $ROOT/a2/personal/$(basename $filename).out
    CODE=$?
    if diff tmp.out $ROOT/a2/personal/$(basename $filename).out; then
        COUNT=$((COUNT+1))
    else
        echo "Fail: $(basename $filename) (exit code $CODE)"
    fi
    TOTAL=$((TOTAL+1))
    rm -f tmp.out
done

echo "Passed $COUNT out of $TOTAL personal tests"

