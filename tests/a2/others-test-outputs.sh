#!/bin/bash
# >>> ./run-tests.sh ../../reference/mitscript

ROOT=".."
INTERPRETER="$2"
PERSON="$1"
TIMEOUT=300

TOTAL=0
COUNT=0

for filename in $ROOT/a2/$PERSON/good*.mit; do
    echo $filename
    timeout $TIMEOUT $INTERPRETER $filename > tmp.out
    CODE=$?
    if diff tmp.out $ROOT/a2/$PERSON/$(basename $filename).out; then
        COUNT=$((COUNT+1))
    else
        echo "Fail: $(basename $filename) (exit code $CODE)"
    fi
    TOTAL=$((TOTAL+1))
    rm -f tmp.out
done

for filename in $ROOT/a2/$PERSON/bad*.mit; do
    echo $filename
    OUT=$(timeout $TIMEOUT $INTERPRETER $filename 2>&1)
    CODE=$?
    if [[ $CODE -ne 0 ]]; then
        COUNT=$((COUNT+1))
    else
        echo "Fail: $(basename $filename) (exit code $CODE) (OUT: $OUT)"
    fi
    TOTAL=$((TOTAL+1))
done

echo "Passed $COUNT out of $TOTAL others tests"

