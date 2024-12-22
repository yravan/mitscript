#!/bin/bash
# set -x

# Function to handle Ctrl+C
ctrl_c() {
    echo "Ctrl+C pressed. Exiting..."
    exit 1
}

# Set up the trap
trap ctrl_c INT

# Initialize arrays
REFERENCE_DERBY_TEST_NAMES=()
REFERENCE_DERBY_TEST_TIMES=()

# Input data
input_data="
bignum.mit                        5.822 s
carsim.mit                        5.198 s
kmediods.mit                       4.44 s
life.mit                          3.398 s
textproc.mit                      5.098 s
treeproc.mit                       8.17 s
"

vm_raw_maps="
bignum.mit                        8.414 s
carsim.mit                        7.724 s
kmediods.mit                      5.406 s
life.mit                          4.224 s
textproc.mit                      8.686 s
treeproc.mit                       7.96 s
"

vm_raw_no_maps="
bignum.mit                        5.659 s
carsim.mit                        5.069 s
kmediods.mit                      4.041 s
life.mit                          2.846 s
textproc.mit                      6.563 s
treeproc.mit                      5.904 s
"

"----------------------------------------
Derby Speedup:

bignum.mit                          1.036
carsim.mit                          0.935
kmediods.mit                        1.065
life.mit                            1.156
textproc.mit                        0.736
treeproc.mit                        1.364

Mean Speedup:                       1.030
----------------------------------------"

vm_p3_well_written_unoptimized="
----------------------------------------
Derby Test Times:

bignum.mit                        4.468 s
carsim.mit                        3.112 s
kmediods.mit                      2.274 s
life.mit                          1.722 s
textproc.mit                       3.22 s
treeproc.mit                      3.432 s

----------------------------------------"

"----------------------------------------
Derby Speedup:

bignum.mit                          1.303
carsim.mit                          1.670
kmediods.mit                        1.953
life.mit                            1.973
textproc.mit                        1.583
treeproc.mit                        2.381

Mean Speedup:                       1.778
----------------------------------------"

vm_p4_base_unoptimized="
----------------------------------------
Derby Test Times:

bignum.mit                      timeout s
carsim.mit                        49.71 s
kmediods.mit                      32.85 s
life.mit                          27.18 s
textproc.mit                      73.18 s
treeproc.mit                      67.11 s

----------------------------------------"


# Parse the input and populate arrays
while IFS= read -r line; do
    # Skip empty lines
    [[ -z "$line" ]] && continue

    # Extract test name and time using awk
    name=$(echo "$line" | awk '{print $1}')
    time=$(echo "$line" | awk '{print $(NF-1)}')

    # Append to REFERENCE_DERBY_TEST_NAMES and REFERENCE_DERBY_TEST_TIMES
    REFERENCE_DERBY_TEST_NAMES+=("$name")
    REFERENCE_DERBY_TEST_TIMES+=("$time")
done <<< "$input_data"


ROOT=$(git rev-parse --show-toplevel)
INTERPRETER=$ROOT/cmake-build-grading/vm/mitscript
VALGRIND_INTERPRETER=$ROOT/cmake-build-mem-debug/interpreter/mitscript

TIMEOUT=90
DEFAULT_MEM=4
REFERENCE_BARE=false
REFERENCE_VM_BYTECODE=false
REFERENCE_VM=false

TEST_DIR_BASE=$ROOT/tests/a5/private
TEST_OUT=$TEST_DIR_BASE/tmp.out
TEST_ERR=$TEST_DIR_BASE/tmp.err

TEST_FILE_EXT=".mit"
INPUT_FILE_EXT=".input"
TARGET_FILE_EXT=".output"
MEMLIMIT_FILE_EXT=".memlimit"

VERBOSE_INPUT=true
VERBOSE_OUTPUT=true
VERBOSE_STATS=true
VERBOSE_ERROR=true

NUM_TIMES=5

RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

EXECUTE="timeout $TIMEOUT /usr/bin/time -v $INTERPRETER -s"
EXECUTE_VALGRIND_TIME="timeout $TIMEOUT /usr/bin/time -v valgrind --tool=callgrind $VALGRIND_INTERPRETER -s"
echo "----------------------------------------"
echo DEFAULT_MEM=$DEFAULT_MEM
echo TIMEOUT=$TIMEOUT
echo "----------------------------------------"

TOTAL=0
SUCCESS=0
FAILURE=0
TIME_ELAPSED=0

run_test() {
    filename=$1
    test_dir=$2
    mem_limit=${3:-$DEFAULT_MEM}
    PRINT_STATS=${4:-true}
    passed=false
    COUNT=$((COUNT+1))

    expected_memlimit=$TEST_DIR_BASE/$test_dir/$(basename $filename $TEST_FILE_EXT)$MEMLIMIT_FILE_EXT
    if test -f $expected_memlimit; then
        mem_limit="$(cat $expected_memlimit)"
    fi
    
    # Log info

    # Execute interpreter
    expected_input=$TEST_DIR_BASE/$test_dir/$(basename $filename $TEST_FILE_EXT)$INPUT_FILE_EXT
    EXECUTE="timeout $TIMEOUT /usr/bin/time -v $INTERPRETER"
    if $REFERENCE_BARE; then
        EXECUTE="timeout $TIMEOUT /usr/bin/time -v $INTERPRETER"
    fi
    if $REFERENCE_VM; then
        EXECUTE="timeout $TIMEOUT /usr/bin/time -v $INTERPRETER -s"
    fi
    if $REFERENCE_VM_BYTECODE; then
        EXECUTE="timeout $TIMEOUT /usr/bin/time -v $INTERPRETER -b"
        filename=$filename"bc"
        echo $filename
    fi


    if $REFERENCE_BARE || $REFERENCE_VM || $REFERENCE_VM_BYTECODE; then
       if test -f $expected_input; then
            $EXECUTE $filename < $expected_input > $TEST_OUT 2> $TEST_ERR
        else
            $EXECUTE $filename > $TEST_OUT 2> $TEST_ERR
        fi
    else
        if test -f $expected_input; then
            $EXECUTE -mem $mem_limit -s $filename < $expected_input > $TEST_OUT 2> $TEST_ERR
        else
            $EXECUTE -mem $mem_limit -s $filename > $TEST_OUT 2> $TEST_ERR
        fi
    fi

    CODE=$?

    if $REFERENCE_VM_BYTECODE; then
        filename="${filename/bc/}"
    fi


    # Save GNU time stats
    STATS=$(cat $TEST_ERR)
    MEM_USAGE_KB=$(echo "$STATS" | grep "Maximum resident set size" | awk '{print $6}')
    USER_TIME_S=$(echo "$STATS" | grep "User time (seconds)" | awk '{print $4}')
    MEM_LIMIT_KB="$((($mem_limit + 5) * 1000))"

    expected_output=$TEST_DIR_BASE/$test_dir/$(basename $filename $TEST_FILE_EXT)$TARGET_FILE_EXT

    # Expected output is exception
    if [[ $(cat $expected_output) =~ [A-Z][A-Za-z]+Exception|Error ]]; then
        # Test fails succesfully, output is exception
        if [[ $(cat $TEST_OUT $TEST_ERR) == *(Exception|Error)* ]]; then
            if [[ $USAGE -gt $LIMIT ]]; then
                echo -e "$(basename $filename): ${RED}Failed${NC} (Memory Usage $MEM_USAGE_KB > $MEM_LIMIT_KB)"
            else
                if $VERBOSE_OUTPUT; then
                    echo -e "$(basename $filename): ${GREEN}Passed${NC}"
                fi
                passed=true
            fi
        # Test runs succesfully, expected output does not match
        else
            echo -e "$(basename $filename): ${RED}Failed${NC} (exit code $CODE)"

            if $VERBOSE_ERROR; then
                echo "$(cat $TEST_OUT)"

                echo "    Diff:"
                diff -u "$expected_output" $TEST_OUT | sed 's/^/    /'
            fi
        fi

    
    # Expected output is valid
    else
        if [[ $CODE = 0 ]]; then
            # Test runs succesfully, expected output matches
            if diff -q $TEST_OUT $expected_output > /dev/null; then
                if [[ $MEM_USAGE_KB -gt $MEM_LIMIT_KB ]]; then
                    echo -e "$(basename $filename): ${RED}Failed${NC} (Memory Usage $MEM_USAGE_KB > $MEM_LIMIT_KB)"
                else
                    if $VERBOSE_OUTPUT && $PRINT_STATS; then
                        echo -e "$(basename $filename): ${GREEN}Passed${NC} (Memory Usage $MEM_USAGE_KB < $MEM_LIMIT_KB)"
                    fi
                    passed=true
                fi

            # Test runs succesfully, expected output does not match
            else
                echo -e "$(basename $filename): ${RED}Failed${NC} (exit code $CODE)"

                if $VERBOSE_ERROR; then
                    echo "$(cat $TEST_OUT)"

                    echo "    Diff:"
                    diff -u "$expected_output" $TEST_OUT | sed 's/^/    /'
                fi
            fi

        # Test does not run succesfully
        else
            echo -e "$(basename $filename): ${RED}Failed${NC} (exit code $CODE)"
            if $VERBOSE_ERROR; then
                echo "$(cat $TEST_OUT)"
            fi
        fi
    fi

    if $VERBOSE_STATS && $PRINT_STATS; then
        echo "Mem Usage: $MEM_USAGE_KB kb"
        echo "User Time: $USER_TIME_S s"
        echo ""
    fi

    if [ "$passed" = true ]; then
        return 0
    else
        return 1
    fi
}


run_valgrind_time_test() {
    filename=$1
    test_dir=$2
    mem_limit=${3:-$DEFAULT_MEM}
    passed=false
    COUNT=$((COUNT+1))

    expected_memlimit=$TEST_DIR_BASE/$test_dir/$(basename $filename $TEST_FILE_EXT)$MEMLIMIT_FILE_EXT
    if test -f $expected_memlimit; then
        mem_limit="$(cat $expected_memlimit)"
    fi

    # Log info
    if $VERBOSE_INPUT; then
        echo "Running $filename with --opt=all -mem=$mem_limit"
    fi

    # Execute interpreter
    expected_input=$TEST_DIR_BASE/$test_dir/$(basename $filename $TEST_FILE_EXT)$INPUT_FILE_EXT
    if test -f $expected_input; then
        $EXECUTE_VALGRIND_TIME -mem $mem_limit --opt=all $filename < $expected_input
    else
        $EXECUTE_VALGRIND_TIME -mem $mem_limit --opt=all $filename 
    fi
    CODE=$?

    # Save GNU time stats
    STATS=$(cat $TEST_ERR)
    MEM_USAGE_KB=$(echo "$STATS" | grep "Maximum resident set size" | awk '{print $6}')
    USER_TIME_S=$(echo "$STATS" | grep "User time (seconds)" | awk '{print $4}')
    MEM_LIMIT_KB="$((($mem_limit + 5) * 1000))"

    expected_output=$TEST_DIR_BASE/$test_dir/$(basename $filename $TEST_FILE_EXT)$TARGET_FILE_EXT

    # Expected output is exception
    if [[ $(cat $expected_output) =~ [A-Z][A-Za-z]+Exception|Error ]]; then
        # Test fails succesfully, output is exception
        if [[ $(cat $TEST_OUT $TEST_ERR) == *(Exception|Error)* ]]; then
            if [[ $USAGE -gt $LIMIT ]]; then
                echo -e "$(basename $filename): ${RED}Failed${NC} (Memory Usage $MEM_USAGE_KB > $MEM_LIMIT_KB)"
            else
                if $VERBOSE_OUTPUT; then
                    echo -e "$(basename $filename): ${GREEN}Passed${NC}"
                fi
                passed=true
            fi
        # Test runs succesfully, expected output does not match
        else
            echo -e "$(basename $filename): ${RED}Failed${NC} (exit code $CODE)"

            if $VERBOSE_ERROR; then
                echo "$(cat $TEST_OUT)"

                echo "    Diff:"
                diff -u "$expected_output" $TEST_OUT | sed 's/^/    /'
            fi
        fi

    
    # Expected output is valid
    else
        if [[ $CODE = 0 ]]; then
            # Test runs succesfully, expected output matches
            if diff -q $TEST_OUT $expected_output > /dev/null; then
                if [[ $MEM_USAGE_KB -gt $MEM_LIMIT_KB ]]; then
                    echo -e "$(basename $filename): ${RED}Failed${NC} (Memory Usage $MEM_USAGE_KB > $MEM_LIMIT_KB)"
                else
                    if $VERBOSE_OUTPUT; then
                        echo -e "$(basename $filename): ${GREEN}Passed${NC} (Memory Usage $MEM_USAGE_KB < $MEM_LIMIT_KB)"
                    fi
                    passed=true
                fi

            # Test runs succesfully, expected output does not match
            else
                echo -e "$(basename $filename): ${RED}Failed${NC} (exit code $CODE)"

                if $VERBOSE_ERROR; then
                    echo "$(cat $TEST_OUT)"

                    echo "    Diff:"
                    diff -u "$expected_output" $TEST_OUT | sed 's/^/    /'
                fi
            fi

        # Test does not run succesfully
        else
            echo -e "$(basename $filename): ${RED}Failed${NC} (exit code $CODE)"
            if $VERBOSE_ERROR; then
                echo "$(cat $TEST_OUT)"
            fi
        fi
    fi

    if $VERBOSE_STATS; then
        echo "Mem Usage: $MEM_USAGE_KB kb"
        echo "User Time: $USER_TIME_S s"
        echo ""
    fi

    if [ "$passed" = true ]; then
        return 0
    else
        return 1
    fi
}

repeat_tests() {
    RUNNING_MEMORY=0
    RUNNING_TIME=0
    # echo "Running $1 $NUM_TIMES times"
    for ((i = 1; i <= NUM_TIMES; i++)); do
        run_test "$1" "$2" "$3" false
        RUNNING_MEMORY=$((RUNNING_MEMORY + MEM_USAGE_KB)) # Integer addition
        RUNNING_TIME=$(awk "BEGIN {print $RUNNING_TIME + $USER_TIME_S}") # Floating-point addition
        running_time=$(awk "BEGIN {print $running_time + $USER_TIME_S}")
    done

    # Calculate averages
    AVG_MEMORY=$((RUNNING_MEMORY / NUM_TIMES))
    AVG_TIME=$(awk "BEGIN {print $RUNNING_TIME / $NUM_TIMES}")

    # Output results
    if [[ $AVG_MEMORY -gt $MEM_LIMIT_KB ]]; then
        echo -e "$(basename $1): ${RED}Failed${NC} (Average Memory Usage $AVG_MEMORY KB > $MEM_LIMIT_KB KB)"
    else
        if $VERBOSE_OUTPUT; then
            echo -e "$(basename $1): ${GREEN}Passed${NC} (Average Memory Usage $AVG_MEMORY KB < $MEM_LIMIT_KB KB)"
        fi
        passed=true
    fi

    echo "Avg Mem Usage: $AVG_MEMORY KB"
    echo "Avg User Time: $AVG_TIME s"
    echo ""
    return $?
}


time_tests() {
    test_dir=$1
    VERBOSE_INPUT=${2:-false}
    VERBOSE_STATS=${3:-false}
    VERBOSE_OUTPUT=${4:-false}
    VERBOSE_ERROR=${5:-false}
    echo "----------------------------------------"
    echo "Running timed tests in $TEST_DIR_BASE/$test_dir"
    echo ""

    total=0
    success=0
    failure=0
    total_time=0

    for filename in $TEST_DIR_BASE/$test_dir/*$TEST_FILE_EXT; do
        total=$((total+1))

        start_time=$(date +%s.%N)
        repeat_tests $filename $test_dir
        result=$?
        end_time=$(date +%s.%N)

        elapsed_time=$(awk "BEGIN {print $end_time - $start_time}")
        echo "$filename Result: $result (Elapsed Time: ${elapsed_time}s)"

        if [[ $result -eq 0 ]]; then
            success=$((success+1))
            total_time=$(awk "BEGIN {print $total_time + $elapsed_time}")
        else
            failure=$((failure+1))
            # total_time=$(awk "BEGIN {print $total_time + 11}")
        fi
    done

    echo ""
    echo "Passed $success out of $total tests in ${total_time}s"

    TOTAL=$((TOTAL+total))
    SUCCESS=$((SUCCESS+success))
    FAILURE=$((FAILURE+failure))
    TIME_ELAPSED=$(awk "BEGIN {print $TIME_ELAPSED + $total_time}")
}


run_tests() {
    test_dir=$1
    VERBOSE_INPUT=${2:-false}
    VERBOSE_STATS=${3:-false}
    VERBOSE_OUTPUT=${4:-true}
    VERBOSE_ERROR=${5:-true}
    echo "----------------------------------------"
    echo "Running tests in $TEST_DIR_BASE/$test_dir"
    echo ""
    tests=()
    times=()

    running_time=0
    total=0
    success=0
    failure=0
    for filename in $TEST_DIR_BASE/$test_dir/*$TEST_FILE_EXT; do
        total=$((total+1))
        if [[ $test_dir == "derby" ]]; then
            repeat_tests $filename $test_dir
            tests+=($filename)
            times+=($AVG_TIME)
        else
            run_test $filename $test_dir
        fi
        result=$?

        if [[ $result -eq 0 ]]; then
            success=$((success+1))
        else
            failure=$((failure+1))
        fi
    done

    echo ""
    echo "Passed $success out of $total tests"
    if [[ "$total" -ne 0 && "$NUM_TIMES" -ne 0 ]]; then
        echo "Average time per test: $(awk -v running_time="$running_time" -v total="$total" -v num_tests="$NUM_TIMES" \
        'BEGIN {print running_time / (total * num_tests)}')s"
    else
        echo "Error: Cannot calculate average time. running_time=$running_time, total=$total, NUM_TESTS=$NUM_TIMES"
    fi

    echo ""

    if [[ $test_dir == "derby" ]]; then
        echo "----------------------------------------"
        echo "Derby Test Times:"
        echo ""
        for ((i = 0; i < ${#tests[@]}; i++)); do
            elem1="${tests[$i]}"
            elem2="${times[$i]}"
            printf "%-30s %10s\n" "$(basename "$elem1")" "$elem2 s"
        done
        echo ""
        echo "----------------------------------------"
    fi

    TOTAL=$((TOTAL+total))
    SUCCESS=$((SUCCESS+success))
    FAILURE=$((FAILURE+failure))
}
run_valgrind-timing-tests() {
    test_dir=$1
    VERBOSE_INPUT=${2:-false}
    VERBOSE_STATS=${3:-false}
    VERBOSE_OUTPUT=${4:-true}
    VERBOSE_ERROR=${5:-true}
    echo "----------------------------------------"
    echo "Running tests in $TEST_DIR_BASE/$test_dir"
    echo ""

    total=0
    success=0
    failure=0
    for filename in $TEST_DIR_BASE/$test_dir/*$TEST_FILE_EXT; do
        total=$((total+1))
        run_valgrind_time_test $filename $test_dir
        result=$?

        if [[ $result -eq 0 ]]; then
            success=$((success+1))
        else
            failure=$((failure+1))
        fi
    done

    echo ""
    echo "Passed $success out of $total tests"




    TOTAL=$((TOTAL+total))
    SUCCESS=$((SUCCESS+success))
    FAILURE=$((FAILURE+failure))
}

flag=$1

if [[ $flag == "--reference" ]]; then
    INTERPRETER=$ROOT/cmake-build-grading/interpreter/mitscript-interpreter
    REFERENCE_BARE=true
    flag="--derby"
    NUM_TESTS=$2
fi

if [[ $flag == "--reference-vm" ]]; then
    INTERPRETER=$ROOT/cmake-build-grading/vm/mitscript
    REFERENCE_VM=true
    flag="--derby"
    NUM_TESTS=$2
fi

if [[ $flag == "--reference-vm-bytecode" ]]; then
    INTERPRETER=$ROOT/cmake-build-grading/vm/mitscript
    REFERENCE_VM_BYTECODE=true
    flag="--derby"
    NUM_TESTS=$2
fi

if [[ $flag == "--time" ]]; then
    time_tests "good"
    time_tests "private"
    time_tests "unit"
    time_tests "other"
    time_tests "staff_good"
    # time_tests "gc"

elif [[ $flag == "--valgrind-time" ]]; then
    start_time=$(date +%s.%N)
    run_valgrind-timing-tests "derby"
    end_time=$(date +%s.%N)
    TIME_ELAPSED=$(awk "BEGIN {print $end_time - $start_time}")


elif [[ $flag == "--gc" ]]; then
    start_time=$(date +%s.%N)
    run_tests "gc"
    end_time=$(date +%s.%N)
    TIME_ELAPSED=$(awk "BEGIN {print $end_time - $start_time}")

elif [[ $flag == "--bignum" ]]; then
    start_time=$(date +%s.%N)
    run_test "$TEST_DIR_BASE/derby/bignum.mit" "derby"
    end_time=$(date +%s.%N)
    TIME_ELAPSED=$(awk "BEGIN {print $end_time - $start_time}")

elif [[ $flag == "--derby" ]]; then
    NUM_TIMES=$2
    start_time=$(date +%s.%N)
    run_tests "derby" true true;
    end_time=$(date +%s.%N)
    TIME_ELAPSED=$(awk "BEGIN {print $end_time - $start_time}")
elif [[ $flag == "--all" ]]; then
    start_time=$(date +%s.%N)
    run_tests "bad"
    run_tests "good"

    run_tests "private"
    run_tests "unit"
    run_tests "other"

    run_tests "staff_bad" 
    run_tests "staff_good"

    run_tests "gc"
    run_tests "derby" true true;
    end_time=$(date +%s.%N)
    TIME_ELAPSED=$(awk "BEGIN {print $end_time - $start_time}")
else
    run_tests "bad"
    run_tests "good"

    run_tests "private"
    run_tests "unit"
    run_tests "other"

    run_tests "staff_bad" 
    run_tests "staff_good"
fi

echo ""
echo "----------------------------------------"
echo "Summary of all tests in $TEST_DIR_BASE"

echo "Passed $SUCCESS out of $TOTAL tests"
echo "Failed $FAILURE out of $TOTAL tests"
echo "Total time elapsed: ${TIME_ELAPSED}s"
echo "----------------------------------------"
echo ""

SPEEDUPS=()

if [[ ${#tests[@]} -gt 0 ]]; then
    echo "----------------------------------------"
    echo "Derby Speedup:"
    echo ""

    # Compute speedups
    for ((i = 0; i < ${#tests[@]}; i++)); do
        elem1="${times[$i]}"                    # Current test time
        elem2="${REFERENCE_DERBY_TEST_TIMES[$i]}"  # Reference time

        # Calculate speedup
        speedup=$(awk "BEGIN {print $elem2 / $elem1}")
        SPEEDUPS+=("$speedup")

        # Print speedup
        printf "%-30s %10.3f\n" "$(basename "${tests[$i]}")" "$speedup"
    done

    # Calculate geometric mean of speedups
    product=1
    for speedup in "${SPEEDUPS[@]}"; do
        product=$(awk "BEGIN {print $product * $speedup}")
    done
    geometric_mean=$(awk "BEGIN {print $product ^ (1 / ${#SPEEDUPS[@]})}")

    echo ""
    printf "%-30s %10.3f\n" "Mean Speedup:" "$geometric_mean"
    echo "----------------------------------------"
fi

