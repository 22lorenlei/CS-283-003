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

@test "Multiple Command With Pipes 1" {
    run "./dsh" <<EOF                
ls | grep ".c"
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    combinedOutput=$(ls | grep ".c")
    combinedOutputTrimmed=$(echo "$combinedOutput" | tr -d '\t\n\r\f\v')
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="${combinedOutputTrimmed}dsh3>dsh3>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Testing echo with spaces 1" {
    run "./dsh" <<EOF                
echo " a b c "
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    combinedOutput=$(echo " a b c ")
    combinedOutputTrimmed=$(echo "$combinedOutput" | tr -d '\t\n\r\f\v')
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    # Expected output with all whitespace removed for easier matching
    expected_output="${combinedOutputTrimmed}dsh3> dsh3> cmd loop returned 0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Testing echo with pipe in quotes" {
 run "./dsh" <<EOF                
echo " a | b | c"
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    combinedOutput=$(echo " a | b | c")
    combinedOutputTrimmed=$(echo "$combinedOutput" | tr -d '\t\n\r\f\v')
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    # Expected output with all whitespace removed for easier matching
    expected_output="${combinedOutputTrimmed}dsh3> dsh3> cmd loop returned 0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Testing echo with pipe in quotes with multiple commands 1" {
    run "./dsh" <<EOF                
echo " a | b | c fff" | grep "|"
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    combinedOutput=$(echo " a | b | c fff" | grep "|")
    combinedOutputTrimmed=$(echo "$combinedOutput" | tr -d '\t\n\r\f\v')
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    # Expected output with all whitespace removed for easier matching
    expected_output="${combinedOutputTrimmed}dsh3> dsh3> cmd loop returned 0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Testing echo with pipe in quotes with multiple commands 2" {
    run "./dsh" <<EOF                
echo " a | b | c fff" | wc -c
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    combinedOutput=$(echo " a | b | c fff" | wc -c)
    combinedOutputTrimmed=$(echo "$combinedOutput" | tr -d '\t\n\r\f\v')
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    # Expected output with all whitespace removed for easier matching
    expected_output="${combinedOutputTrimmed}dsh3> dsh3> cmd loop returned 0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Testing max of 8 command works" {
    run "./dsh" <<EOF                
ls -l | grep ".c" | sort | uniq | wc -l | awk '{print $1}' | xargs echo | cat
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    combinedOutput=$(ls -l | grep ".c" | sort | uniq | wc -l | awk '{print $1}' | xargs echo | cat)
    combinedOutputTrimmed=$(echo "$combinedOutput" | tr -d '\t\n\r\f\v')
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    # Expected output with all whitespace removed for easier matching
    expected_output="${combinedOutputTrimmed}dsh3> dsh3> cmd loop returned 0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Testing 9 commands doesn't work" {
    run "./dsh" <<EOF                
ls -l | grep ".c" | sort | uniq | wc -l | awk '{print $1}' | xargs echo | tr '[:lower:]' '[:upper:]' | rev
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="dsh3>error:pipinglimitedto8commandsdsh3>cmdloopreturned-2"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Testing max of 7 arguments work" {
    run "./dsh" <<EOF                
echo 1 2 3 4 5 6 7
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    combinedOutput=$(echo 1 2 3 4 5 6 7)
    combinedOutputTrimmed=$(echo "$combinedOutput" | tr -d '\t\n\r\f\v')
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    # Expected output with all whitespace removed for easier matching
    expected_output="${combinedOutputTrimmed}dsh3> dsh3> cmd loop returned 0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Testing 8 arguments doesn't work" {
run "./dsh" <<EOF                
echo 1 2 3 4 5 6 7 8
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="dsh3>Toomanyarguments.Maxis7dsh3>cmdloopreturned-3";

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test  "Testing bad commands don't work" {
run "./dsh" <<EOF                
a | b
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="execvp:Nosuchfileordirectorydsh3>execvp:Nosuchfileordirectorydsh3>dsh3>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}



