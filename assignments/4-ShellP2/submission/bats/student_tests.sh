#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

if [[ "$(uname)" == "Darwin" || "$(uname)" == "Linux" ]]; then
    pathSeparator="/"
else
    pathSeparator="\\"
fi

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Test if echo works with quotes" {
    run ./dsh <<EOF
echo "hello"
EOF

    strippeddOutput=$(echo "$output" | tr -d '[:space:]')
    expectedOutput="hellodsh2>dsh2>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${strippeddOutput} -> ${expectedOutput}"

    # Check exact match
    [ "$strippeddOutput" = "$expectedOutput" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Test if echo works without quotes" {
    run ./dsh <<EOF
echo hello
EOF

    strippeddOutput=$(echo "$output" | tr -d '[:space:]')
    expectedOutput="hellodsh2>dsh2>cmdloopreturned0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${strippeddOutput} -> ${expectedOutput}"

    # Check exact match
    [ "$strippeddOutput" = "$expectedOutput" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Test if echo works with single spaces within the quotes" {
    run ./dsh <<EOF
echo " a b c "
EOF
    actualOutput=$(echo "$output" | tr -d '\t\n\r\f\v')
    expectedOutput=" a b c dsh2> dsh2> cmd loop returned 0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${actualOutput} -> ${expectedOutput}"

    # Check exact match
    [ "$actualOutput" = "$expectedOutput" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Test if echo works with multiple spaces within the quotes" {
    run ./dsh <<EOF
echo "  a b   c "
EOF
    actualOutput=$(echo "$output" | tr -d '\t\n\r\f\v')
    expectedOutput="  a b   c dsh2> dsh2> cmd loop returned 0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${actualOutput} -> ${expectedOutput}"

    # Check exact match
    [ "$actualOutput" = "$expectedOutput" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Test if echo works with single quotes" {
    run ./dsh <<EOF
echo '  a bee   c '
EOF
    actualOutput=$(echo "$output" | tr -d '\t\n\r\f\v')
    expectedOutput="  a bee   c dsh2> dsh2> cmd loop returned 0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${actualOutput} -> ${expectedOutput}"

    # Check exact match
    [ "$actualOutput" = "$expectedOutput" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Test if echo works with multiplev quotes" {
    run ./dsh <<EOF
echo "  'a b   c' d "
EOF
    actualOutput=$(echo "$output" | tr -d '\t\n\r\f\v')
    expectedOutput="  'a b   c' d dsh2> dsh2> cmd loop returned 0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${actualOutput} -> ${expectedOutput}"

    # Check exact match
    [ "$actualOutput" = "$expectedOutput" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Test if cd with no argument works" {
 run ./dsh <<EOF
cd
pwd
EOF
    currentDirectory=$(pwd)
    actualOutput=$(echo "$output" | tr -d '\t\n\r\f\v')
    expectedOutput="${currentDirectory}dsh2> dsh2> dsh2> cmd loop returned 0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${actualOutput} -> ${expectedOutput}"

    # Check exact match
    [ "$actualOutput" = "$expectedOutput" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Test if cd with one argument works" {
 run ./dsh <<EOF
cd myTempFolder
pwd
EOF
    currentDirectory=$(pwd)
    actualOutput=$(echo "$output" | tr -d '\t\n\r\f\v')
    expectedOutput="${currentDirectory}${pathSeparator}myTempFolderdsh2> dsh2> dsh2> cmd loop returned 0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${actualOutput} -> ${expectedOutput}"

    # Check exact match
    [ "$actualOutput" = "$expectedOutput" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Test if cd with one argument that is invalid throws error" {
run ./dsh <<EOF
cd thisFolderDoesntExist
EOF

    actualOutput=$(echo "$output" | tr -d '\t\n\r\f\v')
    expectedOutput="dsh2> Could not get into folderdsh2> cmd loop returned 0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${actualOutput} -> ${expectedOutput}"

    # Check exact match
    [ "$actualOutput" = "$expectedOutput" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Test if cd with two arguments that is invalid throws error" {
run ./dsh <<EOF
cd myTempFolder asdlfasdf
EOF

    actualOutput=$(echo "$output" | tr -d '\t\n\r\f\v')
    expectedOutput="dsh2> cd takes one argument onlydsh2> cmd loop returned 0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${actualOutput} -> ${expectedOutput}"

    # Check exact match
    [ "$actualOutput" = "$expectedOutput" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Test if ls -l works" {
run ./dsh <<EOF
ls -l
EOF

    lsOutput=$(ls -l)
    lsOutputTrimmed=$(echo "$lsOutput" | tr -d '\t\n\r\f\v')
    actualOutput=$(echo "$output" | tr -d '\t\n\r\f\v')
    expectedOutput="${lsOutputTrimmed}dsh2> dsh2> cmd loop returned 0"

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${actualOutput} -> ${expectedOutput}"

    # Check exact match
    [ "$actualOutput" = "$expectedOutput" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Test if bogus commands throw error" {
run ./dsh <<EOF
badCOmmand a
EOF

    actualOutput=$(echo "$output" | tr -d '[:space:]')
    # So my thought here is that while it was an error, the act of exiting was successful
    # Therefore, the first command was unsuccessful -6, but exiting was good, so 0
    expectedOutput="dsh2>cmdloopreturned-6dsh2>dsh2>cmdloopreturned0" 

    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${actualOutput} -> ${expectedOutput}"

    # Check exact match
    [ "$actualOutput" = "$expectedOutput" ]

    # Assertions
    [ "$status" -eq -0 ]
}


