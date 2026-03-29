#!/bin/bash

# Directory for results
mkdir -p valgrind_results

# List of tests: each line = arguments passed to ./pipex
tests=(
  'infile.txt "cat lol" wc example'
  'infile.txt lol wc example'
  'infile.txt cat wc outfile.txt'
  # add more tests here
  # !!! for " " and ' ' = SEGFAULT for valgrind
)

echo
echo "===== Starting Valgrind tests ====="

# Array to store summaries
declare -a summary

# Loop over each test
for i in "${!tests[@]}"; do
    test_cmd=${tests[$i]}
    echo "=== Test $i : $test_cmd ==="

    # Full log file
    log_file="valgrind_results/test_$i.log"

    # Convert string to array to preserve quoted arguments
    # IFS=' ' read -r -a args <<< "$test_cmd"

    # Run Valgrind
    valgrind --track-origins=yes --show-leak-kinds=all --leak-check=full \
    ./pipex $test_cmd &> "$log_file"

    # Extract only leak-related lines
    leaks=$(grep -E "definitely lost|indirectly lost|possibly lost|still reachable" "$log_file")

    # Store in summary array
    summary[$i]="Test $i: $test_cmd
$leaks
--------------------------------"

done

echo
echo "===== Memory Leak Summary ====="
for s in "${summary[@]}"; do
    echo -e "$s"
done

echo "All tests completed. Full logs are saved in valgrind_results/"