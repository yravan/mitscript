#!/bin/bash
# >>> ./run-tests.sh ../../reference/mitscript

ROOT=".."
INTERPRETER="$1"
TIMEOUT=300

TOTAL=0
COUNT=0

for filename in $ROOT/a2/public/bad*.mit; do
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

for filename in $ROOT/a2/public/good*.mit; do
    echo $filename
    timeout $TIMEOUT $INTERPRETER $filename > tmp.out
    CODE=$?
    if diff tmp.out $ROOT/a2/public/$(basename $filename).out; then
        COUNT=$((COUNT+1))
    else
        echo "Fail: $(basename $filename) (exit code $CODE)"
    fi
    TOTAL=$((TOTAL+1))
    rm -f tmp.out
done

echo "Passed $COUNT out of $TOTAL public tests"

PTOTAL=0
PCOUNT=0

for filename in $ROOT/a2/private/*.mit; do
    if [ ! -f "$filename" ]; then
        break
    fi
    echo $filename
    if test -f $filename.in; then
        timeout $TIMEOUT $INTERPRETER $filename < $filename.in > tmp1.out 2> tmp1.err
    else
        timeout $TIMEOUT $INTERPRETER $filename > tmp1.out 2> tmp1.err
    fi
    CODE=$?
    if [[ $(cat $ROOT/a2/private/$(basename $filename).out) =~ [A-Z][A-Za-z]+Exception ]]; then
        if [[ $(cat tmp1.err tmp1.out) = *"${BASH_REMATCH[0]}"* ]]; then
            PCOUNT=$((PCOUNT+1))
            COUNT=$((COUNT+1))
        else
						cat tmp1.out tmp1.err
            echo "Fail Exception: $(basename $filename) (exit code $CODE)"
        fi
    else
        if diff tmp1.out $ROOT/a2/private/$(basename $filename).out; then
            PCOUNT=$((PCOUNT+1))
            COUNT=$((COUNT+1))
        else
						cat tmp1.out tmp1.err
            echo "Fail: $(basename $filename) (exit code $CODE)"
        fi
    fi
    TOTAL=$((TOTAL+1))
    PTOTAL=$((PTOTAL+1))
    rm -f tmp1.out tmp1.err
done

echo "Passed $PCOUNT out of $PTOTAL private tests"
echo "Passed $COUNT out of $TOTAL tests"
