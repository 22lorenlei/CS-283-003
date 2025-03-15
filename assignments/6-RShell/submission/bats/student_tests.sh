#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Testing if ls works for executing remote commands" {
    run ./dsh -s -i 0.0.0.0 & server_pid=$!
    sleep 0.5
    run ./dsh -c -i 127.0.0.1 <<EOF
    ls
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    combinedOutput=$(ls)
    combinedOutputTrimmed=$(echo "$combinedOutput" | tr -d '\0[:cntrl:][:space:]')
    stripped_output=$(echo "$output" | tr -d '\0[:cntrl:][:space:]')

    # Strip all whitespace from the expected output for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>Server:${combinedOutputTrimmed}dsh4>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    # if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "Actual Output (Stripped): ${stripped_output}"
    echo "Expected Output (Stripped): ${expected_output}"

    # Check exact match after stripping whitespace
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Testing piped command works for executing remote commands" {
    run ./dsh -s -i 0.0.0.0 & server_pid=$!
    sleep 0.5
    run ./dsh -c -i 127.0.0.1 <<EOF
    ls -l | grep ".c"
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    combinedOutput=$(ls -l | grep ".c")
    combinedOutputTrimmed=$(echo "$combinedOutput" | tr -d '\0[:cntrl:][:space:]')
    stripped_output=$(echo "$output" | tr -d '\0[:cntrl:][:space:]')

    # Strip all whitespace from the expected output for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>Server:${combinedOutputTrimmed}dsh4>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    # if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "Actual Output (Stripped): ${stripped_output}"
    echo "Expected Output (Stripped): ${expected_output}"

    # Check exact match after stripping whitespace
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Testing echo works for executing remote commands" {
    run ./dsh -s -i 0.0.0.0 & server_pid=$!
    sleep 0.5
    run ./dsh -c -i 127.0.0.1 <<EOF
    echo "hello"
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    combinedOutput=$(echo "hello")
    combinedOutputTrimmed=$(echo "$combinedOutput" | tr -d '\0[:cntrl:][:space:]')
    stripped_output=$(echo "$output" | tr -d '\0[:cntrl:][:space:]')

    # Strip all whitespace from the expected output for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>Server:${combinedOutputTrimmed}dsh4>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    # if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "Actual Output (Stripped): ${stripped_output}"
    echo "Expected Output (Stripped): ${expected_output}"

    # Check exact match after stripping whitespace
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Testing echo with pipes within quotes works for executing remote commands" {
    run ./dsh -s -i 0.0.0.0 & server_pid=$!
    sleep 0.5
    run ./dsh -c -i 127.0.0.1 <<EOF
    echo "hello | hi"
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    combinedOutput=$(echo "hello | hi")
    combinedOutputTrimmed=$(echo "$combinedOutput" | tr -d '\0[:cntrl:][:space:]')
    stripped_output=$(echo "$output" | tr -d '\0[:cntrl:][:space:]')

    # Strip all whitespace from the expected output for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>Server:${combinedOutputTrimmed}dsh4>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    # if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "Actual Output (Stripped): ${stripped_output}"
    echo "Expected Output (Stripped): ${expected_output}"

    # Check exact match after stripping whitespace
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Testing echo with pipes with irregular spaces works for executing remote commands" {
    run ./dsh -s -i 0.0.0.0 & server_pid=$!
    sleep 0.5
    run ./dsh -c -i 127.0.0.1 <<EOF
    echo     "  hello |    hi   "
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    combinedOutput=$(echo     "  hello |    hi   ")
    combinedOutputTrimmed=$(echo "$combinedOutput" | tr -d '\0[:cntrl:][:space:]')
    stripped_output=$(echo "$output" | tr -d '\0[:cntrl:][:space:]')

    # Strip all whitespace from the expected output for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>Server:${combinedOutputTrimmed}dsh4>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    # if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "Actual Output (Stripped): ${stripped_output}"
    echo "Expected Output (Stripped): ${expected_output}"

    # Check exact match after stripping whitespace
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Testing echo (command) with 7 (the max arguments) works for executing remote commands" {
    run ./dsh -s -i 0.0.0.0 & server_pid=$!
    sleep 0.5
    run ./dsh -c -i 127.0.0.1 <<EOF
    echo 1 2 3 4 5 6 7
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    combinedOutput=$(echo 1 2 3 4 5 6 7)
    combinedOutputTrimmed=$(echo "$combinedOutput" | tr -d '\0[:cntrl:][:space:]')
    stripped_output=$(echo "$output" | tr -d '\0[:cntrl:][:space:]')

    # Strip all whitespace from the expected output for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>Server:${combinedOutputTrimmed}dsh4>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    # if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "Actual Output (Stripped): ${stripped_output}"
    echo "Expected Output (Stripped): ${expected_output}"

    # Check exact match after stripping whitespace
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Testing echo (command) with 8 arguments (more than max) doesn't work for executing remote commands" {
    run ./dsh -s -i 0.0.0.0 & server_pid=$!
    sleep 0.5
    run ./dsh -c -i 127.0.0.1 <<EOF
    echo 1 2 3 4 5 6 7 8
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\0[:cntrl:][:space:]')

    # Strip all whitespace from the expected output for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>Server:Toomanyarguments.Maxis7dsh4>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    # if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "Actual Output (Stripped): ${stripped_output}"
    echo "Expected Output (Stripped): ${expected_output}"

    # Check exact match after stripping whitespace
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Testing 8 commands (the max) works for executing remote commands" {
    run ./dsh -s -i 0.0.0.0 & server_pid=$!
    sleep 0.5
    run ./dsh -c -i 127.0.0.1 <<EOF
    ls -l | grep ".c" | sort | uniq | wc -l | awk '{print $1}' | xargs echo | cat
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    combinedOutput=$(ls -l | grep ".c" | sort | uniq | wc -l | awk '{print $1}' | xargs echo | cat)
    combinedOutputTrimmed=$(echo "$combinedOutput" | tr -d '\0[:cntrl:][:space:]')
    stripped_output=$(echo "$output" | tr -d '\0[:cntrl:][:space:]')

    # Strip all whitespace from the expected output for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>Server:${combinedOutputTrimmed}dsh4>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    # if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "Actual Output (Stripped): ${stripped_output}"
    echo "Expected Output (Stripped): ${expected_output}"

    # Check exact match after stripping whitespace
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Testing 9 commands (more than max) doesn't work for executing remote commands" {
    run ./dsh -s -i 0.0.0.0 & server_pid=$!
    sleep 0.5
    run ./dsh -c -i 127.0.0.1 <<EOF
    ls -l | grep ".c" | sort | uniq | wc -l | awk '{print $1}' | xargs echo | tr '[:lower:]' '[:upper:]' | rev
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\0[:cntrl:][:space:]')

    # Strip all whitespace from the expected output for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1234dsh4>Server:error:pipinglimitedto8commandsdsh4>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    # if the test fails
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "Actual Output (Stripped): ${stripped_output}"
    echo "Expected Output (Stripped): ${expected_output}"

    # Check exact match after stripping whitespace
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}



