#!/bin/sh
#  test.sh
#  Created by Joshua Higginbotham on 11/8/15.

# This is only provided for your convenience.
# The tests used in this file may or may not
# match up with what is called for for the report,
# so be sure to read that part of the handout.
# But you're free to modify this script however
# you need to, it's not graded.

echo "Cleaning and compiling..."
make
echo "Running tests..."
echo "Test 1, w = 5:" >> output.txt
./client -n 10000 -w 5
echo "Test 2, w = 10:" >> output.txt
./client -n 10000 -w 10
echo "Test 3, w = 20:" >> output.txt
./client -n 10000 -w 20
echo "Test 4, w = 30:" >> output.txt
./client -n 10000 -w 30
echo "Test 5, w = 40:" >> output.txt
./client -n 10000 -w 40
echo "Test 6, w = 50:" >> output.txt
./client -n 10000 -w 50
echo "Test 7, w = 70:" >> output.txt
./client -n 10000 -w 70
echo "Test 8, w = 90:" >> output.txt
./client -n 10000 -w 90
echo "Test 9, w = 110:" >> output.txt
./client -n 10000 -w 110
echo "Test 10, w = 130:" >> output.txt
./client -n 10000 -w 130
echo "Test 11, w = 150:" >> output.txt
./client -n 10000 -w 150
echo "Test 12, w = 170:" >> output.txt
./client -n 10000 -w 170
echo "Test 13, w = 190:" >> output.txt
./client -n 10000 -w 190
echo "Test 14, w = 210:" >> output.txt
./client -n 10000 -w 210
echo "Test 15, w = 230:" >> output.txt
./client -n 10000 -w 230
echo "Test 16, w = 250:" >> output.txt
./client -n 10000 -w 250
echo "Test 17, w = 270:" >> output.txt
./client -n 10000 -w 270
echo "Test 18, w = 300:" >> output.txt
./client -n 10000 -w 300
echo "Test 19, w = 350:" >> output.txt
./client -n 10000 -w 350
echo "Test 20, w = 400:" >> output.txt
./client -n 10000 -w 400
echo "Finished!"
