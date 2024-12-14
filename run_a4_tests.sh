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

TIMEOUT=10
DEFAULT_MEM=100

echo "----------------------------------------"
echo DEFAULT_MEM=$DEFAULT_MEM
echo TIMEOUT=$TIMEOUT

# ========================================================================
# Interpreter Source Mode: a3 bytecode tests
# ========================================================================
echo "----------------------------------------"
echo "Running interpreter bytecode mode: a3 bytecode tests..."

TOTAL=0
COUNT=0
SKIP_TESTS=("good4.mitbc" "good5.mitbc")
TEST_DIR=./tests/a3/a1-mitbc
for filename in $TEST_DIR/good*.mitbc; do
    basename_file=$(basename "$filename")
    if [[ " ${SKIP_TESTS[@]} " =~ " ${basename_file} " ]]; then
        # echo "Skipping: $basename_file"
        continue
    fi

    # echo $filename
    timeout $TIMEOUT $INTERPRETER -mem $DEFAULT_MEM -b $filename > tmp.out
    CODE=$?

    expected_output="$TEST_DIR/$(basename $filename).out"
    if diff -q tmp.out $expected_output > /dev/null; then
        COUNT=$((COUNT+1))
        # echo "Pass: $(basename $filename) (exit code $CODE)"
    else
        echo "Fail: $(basename $filename) (exit code $CODE)"
        # echo "    Diff:"
        # diff -u "$expected_output" tmp.out | sed 's/^/    /'
    fi
    TOTAL=$((TOTAL+1))
    rm -f tmp.out
done
echo "Passed $(printf '%2d' $COUNT) out of $(printf '%2d' $TOTAL) a1-mitbc bytecode public tests"


TOTAL=0
COUNT=0
SKIP_TESTS=("good14.mitbc") # Function equality
TEST_DIR=./tests/a3/a2-mitbc
for filename in $TEST_DIR/good*.mitbc; do
    basename_file=$(basename "$filename")
    if [[ " ${SKIP_TESTS[@]} " =~ " ${basename_file} " ]]; then
        # echo "Skipping: $basename_file"
        continue
    fi

    # echo $filename
    timeout $TIMEOUT $INTERPRETER -mem $DEFAULT_MEM -b $filename > tmp.out
    CODE=$?

    expected_output="$TEST_DIR/$(basename $filename).out"
    if diff -q tmp.out $expected_output > /dev/null; then
        COUNT=$((COUNT+1))
        # echo "Pass: $(basename $filename) (exit code $CODE)"
    else
        echo "Fail: $(basename $filename) (exit code $CODE)"
        # echo "    Diff:"
        # diff -u "$expected_output" tmp.out | sed 's/^/    /'
    fi
    TOTAL=$((TOTAL+1))
    rm -f tmp.out
done
echo "Passed $(printf '%2d' $COUNT) out of $(printf '%2d' $TOTAL) a2-mitbc bytecode public tests"


TOTAL=0
COUNT=0
TEST_DIR=./tests/a3/public/mitbc
for filename in $TEST_DIR/bad*.mitbc; do
    # echo $filename
    timeout $TIMEOUT $INTERPRETER -mem $DEFAULT_MEM -b $filename > tmp.out
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
echo "Passed $(printf '%2d' $COUNT) out of $(printf '%2d' $TOTAL) a3/public/mitbc bytecode public tests"

TOTAL=0
COUNT=0
TEST_DIR=./tests/a3/personal_mitbc
for filename in $TEST_DIR/bad*.mitbc; do
    # echo $filename
    timeout $TIMEOUT $INTERPRETER -mem $DEFAULT_MEM -b $filename > tmp.out
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
echo "Passed $(printf '%2d' $COUNT) out of $(printf '%2d' $TOTAL) a3/personal_mitbc bytecode personal tests"

# ========================================================================
# Interpreter Source Mode: a2 public + personal tests
# ========================================================================
echo "----------------------------------------"
echo "Running interpreter source mode: a2 public + personal tests..."
PUBLIC_TOTAL=0
PUBLIC_COUNT=0

SKIP_TESTS=(
    "good14.mit" # Function equality
)

TEST_DIR=./tests/a2/public
for filename in $TEST_DIR/good*.mit; do
    basename_file=$(basename "$filename")
    if [[ " ${SKIP_TESTS[@]} " =~ " ${basename_file} " ]]; then
        # echo "Skipping: $basename_file"
        continue
    fi

    # echo $filename
    $(timeout $TIMEOUT $INTERPRETER -mem $DEFAULT_MEM -s $filename > tmp.out)
    CODE=$?

    expected_output="$TEST_DIR/$(basename $filename).out"
    if diff -q tmp.out $expected_output > /dev/null; then
        PUBLIC_COUNT=$((PUBLIC_COUNT+1))
        # echo "Pass: $(basename $filename) (exit code $CODE)"
    else
        echo "Fail: $(basename $filename) (exit code $CODE)"
        # echo "    Diff:"
        # diff -u "$expected_output" tmp.out | sed 's/^/    /'
    fi
    PUBLIC_TOTAL=$((PUBLIC_TOTAL+1))
    rm -f tmp.out tmp.err
done
echo "Passed $PUBLIC_COUNT out of $PUBLIC_TOTAL public tests"


NAMES=("joseph" "yajvan" "rom")
for name in "${NAMES[@]}"; do
    TEST_DIR=./tests/a2/personal_${name}

    TOTAL=0
    COUNT=0

    for filename in $TEST_DIR/test*.mit; do
        # echo $filename
        timeout $TIMEOUT $INTERPRETER -mem $DEFAULT_MEM -s $filename > tmp.out
        CODE=$?

        expected_output="$TEST_DIR/$(basename "$filename" .mit).out"
    
        if diff -q tmp.out $expected_output > /dev/null; then
            COUNT=$((COUNT+1))
            # echo "Pass: $(basename $filename) (exit code $CODE)"
        else
            echo "Fail: $(basename $filename) (exit code $CODE)"
            # echo "    Diff:"
            # diff -u "$expected_output" tmp.out | sed 's/^/    /'
        fi
        TOTAL=$((TOTAL+1))
        rm -f tmp.out
    done

    echo "Passed $COUNT out of $TOTAL personal_${name} tests"
done


TOTAL=0
COUNT=0
TEST_DIR=/src/tests/a3/personal
for filename in $TEST_DIR/test*.mit; do
    # echo $filename
    timeout $TIMEOUT $INTERPRETER -mem $DEFAULT_MEM -s $filename > tmp.out
    CODE=$?

    expected_output="$TEST_DIR/$(basename "$filename" .mit).out"

    if diff -q tmp.out $expected_output > /dev/null; then
        COUNT=$((COUNT+1))
        # echo "Pass: $(basename $filename) (exit code $CODE)"
    else
        echo "Fail: $(basename $filename) (exit code $CODE)"
        # echo "    Diff:"
        # diff -u "$expected_output" tmp.out | sed 's/^/    /'
    fi
    TOTAL=$((TOTAL+1))
    rm -f tmp.out
done

echo "Passed $COUNT out of $TOTAL group personal tests"

# ========================================================================
# Interpreter Source Mode: a4 garbage tests
# ========================================================================
echo "----------------------------------------"
echo "Running interpreter source mode: a4 garbage tests..."

TOTAL=0
COUNT=0

TEST_DIR=./tests/a4/public
for filename in $TEST_DIR/garbagetest*.mit; do
    # echo $filename
    timeout $TIMEOUT $INTERPRETER -mem $DEFAULT_MEM -s $filename > tmp.out
    CODE=$?

    expected_output="$TEST_DIR/$(basename "$filename").out"

    if diff -q tmp.out $expected_output > /dev/null; then
        COUNT=$((COUNT+1))
        # echo "Pass: $(basename $filename) (exit code $CODE)"
    else
        echo "Fail: $(basename $filename) (exit code $CODE)"
        # echo "    Diff:"
        # diff -u "$expected_output" tmp.out | sed 's/^/    /'
    fi
    TOTAL=$((TOTAL+1))
    rm -f tmp.out
done

echo "Passed $COUNT out of $TOTAL public tests"
echo "----------------------------------------"

# ========================================================================
# Interpreter Source Mode: a4 personal tests
# ========================================================================
echo "----------------------------------------"
echo "Running interpreter source mode: a4 personal tests..."

TOTAL=0
COUNT=0

TEST_DIR=./tests/a4/personal
for filename in $TEST_DIR/test*.mit; do
    # echo $filename
    timeout $TIMEOUT $INTERPRETER -mem $DEFAULT_MEM -s $filename > tmp.out
    CODE=$?

    # Change here to remove the `.mit` extension and use `.out`
    expected_output="$TEST_DIR/$(basename "$filename" .mit).out"

    if diff -q tmp.out $expected_output > /dev/null; then
        COUNT=$((COUNT+1))
        # echo "Pass: $(basename $filename) (exit code $CODE)"
    else
        echo "Fail: $(basename $filename) (exit code $CODE)"
        # echo "    Diff:"
        # diff -u "$expected_output" tmp.out | sed 's/^/    /'
    fi
    TOTAL=$((TOTAL+1))
    rm -f tmp.out
done

echo "Passed $COUNT out of $TOTAL personal tests"
echo "----------------------------------------"




