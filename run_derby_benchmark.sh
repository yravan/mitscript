#!/bin/bash
# set -x

# Function to handle Ctrl+C
ctrl_c() {
    echo "Ctrl+C pressed. Exiting..."
    exit 1
}

# Set up the trap
trap ctrl_c INT

COMPILER=./cmake-build-grading/interpreter/mitscriptc
INTERPRETER=./cmake-build-grading/interpreter/mitscript

TIMEOUT=300
DEFAULT_MEM=10

echo "----------------------------------------"
echo DEFAULT_MEM=$DEFAULT_MEM
echo TIMEOUT=$TIMEOUT

echo "----------------------------------------"

TEST_DIR=./tests/derby/public
for filename in $TEST_DIR/*.mit; do
    echo "RUNNING $(basename $filename)"
    rm -f tmp.out tmp.err
    
    # Load memlimit
    filename_memlimit=$TEST_DIR/$(basename "$filename" .mit).memlimit
    if test -f $filename_memlimit; then
        MEMLIMIT="$(cat $filename_memlimit)"
    else
        MEMLIMIT=$DEFAULT_MEM
    fi
    LIMIT="$((($MEMLIMIT + 10) * 1000))"
    echo "Memory Limit: $LIMIT kb"
    echo " "

    # ==========================================================================================
    # Unoptimized
    # ==========================================================================================
    
    # Run test
    filename_input=$TEST_DIR/$(basename "$filename" .mit).input
    if test -f $filename_input; then
        timeout $TIMEOUT /usr/bin/time -v $INTERPRETER -mem $MEMLIMIT -s $filename < $filename_input > tmp.out 2> tmp.err
    else
        timeout $TIMEOUT /usr/bin/time -v $INTERPRETER -mem $MEMLIMIT -s $filename > tmp.out 2> tmp.err
    fi
    CODE=$?

    # Save GNU time stats
    UNOPTIMIZED_STATS=$(cat tmp.err)

    MEM_USAGE_KB=$(echo "$UNOPTIMIZED_STATS" | grep "Maximum resident set size" | awk '{print $6}')
    USER_TIME_S=$(echo "$UNOPTIMIZED_STATS" | grep "User time (seconds)" | awk '{print $4}')
    echo "[unoptimized] Mem Usage: $MEM_USAGE_KB kb"
    echo "[unoptimized] User Time: $USER_TIME_S s"
    echo ""

    # ==========================================================================================
    # Optimized
    # ==========================================================================================

    # Run test
    filename_input=$TEST_DIR/$(basename "$filename" .mit).input
    if test -f $filename_input; then
        timeout $TIMEOUT /usr/bin/time -v $INTERPRETER -mem $MEMLIMIT -s --opt=all $filename < $filename_input > tmp.out 2> tmp.err
    else
        timeout $TIMEOUT /usr/bin/time -v $INTERPRETER -mem $MEMLIMIT -s --opt=all $filename > tmp.out 2> tmp.err
    fi
    CODE=$?

    # Save GNU time stats
    OPTIMIZED_STATS=$(cat tmp.err)

    MEM_USAGE_KB=$(echo "$OPTIMIZED_STATS" | grep "Maximum resident set size" | awk '{print $6}')
    USER_TIME_S=$(echo "$OPTIMIZED_STATS" | grep "User time (seconds)" | awk '{print $4}')
    echo "[optimized] Mem Usage: $MEM_USAGE_KB kb"
    echo "[optimized] User Time: $USER_TIME_S s"
    echo ""

    # ==========================================================================================
    # Correctness (optimized)
    # ==========================================================================================

    # Expected output is exception
    filename_output=$TEST_DIR/$(basename "$filename" .mit).output
    if [[ $(cat $filename_output) =~ [A-Z][A-Za-z]+Exception ]]; then
        # Used to be *"${BASH_REMATCH[0]}"*, but this was relaxed
        if [[ $(cat tmp.err tmp.out) == *"Exception"* ]]; then
            if [[ $USAGE -gt $LIMIT ]]; then
                echo "Fail [Memory Usage]: $(basename $filename) ($MEM_USAGE_KB > $LIMIT)"
            else
                COUNT=$((COUNT+1))
            fi
        else
            echo "Fail Exception: $(basename $filename) (exit code $CODE)"
        fi

    # Expected output is valid
    else
        if [[ $CODE = 0 ]]; then

            if diff -q tmp.out $filename_output > /dev/null; then
                if [[ $MEM_USAGE_KB -gt $LIMIT ]]; then
                    echo "Fail [Memory Usage]: $(basename $filename) ($MEM_USAGE_KB > $LIMIT)"
                else
                    COUNT=$((COUNT+1))
                    echo "Pass: $(basename $filename) (exit code $CODE)"
                fi

            else
                echo "Fail: $(basename $filename) (exit code $CODE)"
                echo "$(cat tmp.out)"
                echo "$(cat tmp.err)"

                filename_error_out="$TEST_DIR/$(basename "$filename" .mit).out"
                cat tmp.out > "$filename_error_out"

                filename_error_error="$TEST_DIR/$(basename "$filename" .mit).err"
                cat tmp.err > "$filename_error_error"

                echo "    Diff:"
                diff -u "$filename_output" tmp.out | sed 's/^/    /'
            fi

        else
            filename_error_out="$TEST_DIR/results/$(basename "$filename" .mit).out"
            cat tmp.out > "$filename_error_out"

            filename_error_error="$TEST_DIR/results/$(basename "$filename" .mit).err"
            cat tmp.err > "$filename_error_error"

            echo "Fail: $(basename $filename) (exit code $CODE)"
        fi
    fi

    TOTAL=$((TOTAL+1))
    echo "-----------"
done

