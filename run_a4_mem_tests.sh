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

TIMEOUT=90
DEFAULT_MEM_MIN_LIMIT=4
DEFAULT_MEM_MID_LIMIT=6
DEFAULT_MEM_NO_LIMIT=100

echo "----------------------------------------"
echo DEFAULT_MEM_MIN_LIMIT=$DEFAULT_MEM_MIN_LIMIT
echo DEFAULT_MEM_NO_LIMIT=$DEFAULT_MEM_NO_LIMIT
echo TIMEOUT=$TIMEOUT

# ========================================================================
# Interpreter Source Mode: a4 personal tests
# ========================================================================
echo "----------------------------------------"
echo "Running interpreter source mode: a4 personal tests..."

TOTAL=0
COUNT=0

TEST_DIR=./tests/a4/personal
for filename in $TEST_DIR/test*.mit; do

    # Capture memory usage with no limit
    mem_usage_kb_no_limit=$(timeout $TIMEOUT /usr/bin/time -v $INTERPRETER -mem $DEFAULT_MEM_NO_LIMIT -s "$filename" 2>&1 | grep "Maximum resident set size" | awk '{print $6}')

    # Capture memory usage with minimum limit
    mem_usage_kb_min_limit=$(timeout $TIMEOUT /usr/bin/time -v $INTERPRETER -mem $DEFAULT_MEM_MIN_LIMIT -s "$filename" 2>&1 | grep "Maximum resident set size" | awk '{print $6}')

    # Capture memory usage with minimum limit
    mem_usage_kb_mid_limit=$(timeout $TIMEOUT /usr/bin/time -v $INTERPRETER -mem $DEFAULT_MEM_MID_LIMIT -s "$filename" 2>&1 | grep "Maximum resident set size" | awk '{print $6}')

    printf "Mem usage: %s\n" "$filename"
    printf "No Limit:  --mem = %4d: %6s KB\n" "$DEFAULT_MEM_NO_LIMIT" "$mem_usage_kb_no_limit"
    printf "Mid Limit: --mem = %4d: %6s KB\n" "$DEFAULT_MEM_MID_LIMIT" "$mem_usage_kb_mid_limit"
    printf "Min Limit: --mem = %4d: %6s KB\n" "$DEFAULT_MEM_MIN_LIMIT" "$mem_usage_kb_min_limit"
    echo ""
done

rm -rf tmp.out
