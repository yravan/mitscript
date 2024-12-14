#!/bin/bash
# set -x

# Function to handle Ctrl+C
ctrl_c() {
    echo "Ctrl+C pressed. Exiting..."
    exit 1
}

# Set up the trap
trap ctrl_c INT

COMPILER=./cmake-build-grading/vm/mitscriptc
INTERPRETER=./cmake-build-grading/vm/mitscript

TIMEOUT=60
DEFAULT_MEM=4

echo "----------------------------------------"
echo DEFAULT_MEM=$DEFAULT_MEM
echo TIMEOUT=$TIMEOUT

echo "----------------------------------------"


TOTAL=0
COUNT=0

TEST_DIR=./tests/a5/public
for filename in $TEST_DIR/*.mit; do
    echo "RUNNING $(basename $filename)"
    rm -f tmp.out tmp.err
    
    LIMIT="$((($DEFAULT_MEM + 10) * 1000))"
    echo "Memory Limit: $LIMIT kb"
    echo " "

    # Run test
    timeout $TIMEOUT /usr/bin/time -v $INTERPRETER -mem $DEFAULT_MEM -s $filename > tmp.out 2> tmp.err
    CODE=$?

    # Save GNU time stats
    STATS=$(cat tmp.err)

    MEM_USAGE_KB=$(echo "$STATS" | grep "Maximum resident set size" | awk '{print $6}')
    echo "Mem Usage: $MEM_USAGE_KB kb"

    # Expected output is exception
    filename_output=$TEST_DIR/$(basename "$filename").out

    if diff -q tmp.out $filename_output > /dev/null; then
        if [[ $MEM_USAGE_KB -gt $LIMIT ]]; then
            echo "Fail: $(basename $filename) ($MEM_USAGE_KB > $LIMIT)"
        else
            COUNT=$((COUNT+1))
            echo "PASS: $(basename $filename) (exit code $CODE)"
        fi

    else
        echo "Fail: $(basename $filename) (exit code $CODE)"

        echo "    Diff:"
        diff -u "$filename_output" tmp.out | sed 's/^/    /'
    fi

    TOTAL=$((TOTAL+1))
    echo "-----------"
done

echo "Passed $COUNT out of $TOTAL public tests"
echo "----------------------------------------"

