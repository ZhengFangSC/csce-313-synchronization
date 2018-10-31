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
echo "Test 1, b = 1:" >> output.txt
./client -n 10000 -b 1 -w 5
echo "Test 2, b = 2:" >> output.txt
./client -n 10000 -b 2 -w 5
echo "Test 3, b = 3:" >> output.txt
./client -n 10000 -b 3 -w 5
echo "Test 4, b = 4:" >> output.txt
./client -n 10000 -b 4 -w 5
echo "Test 5, b = 5:" >> output.txt
./client -n 10000 -b 5 -w 5
echo "Test 6, b = 10:" >> output.txt
./client -n 10000 -b 10 -w 5
echo "Test 7, b = 20:" >> output.txt
./client -n 10000 -b 20 -w 5
echo "Test 8, b = 30:" >> output.txt
./client -n 10000 -b 30 -w 5
echo "Test 9, b = 40:" >> output.txt
./client -n 10000 -b 40 -w 5
echo "Test 10, b = 50:" >> output.txt
./client -n 10000 -b 50 -w 5
echo "Test 11, b = 60:" >> output.txt
./client -n 10000 -b 60 -w 5
echo "Test 12, b = 70:" >> output.txt
./client -n 10000 -b 70 -w 5
echo "Test 13, b = 80:" >> output.txt
./client -n 10000 -b 80 -w 5
echo "Test 14, b = 90:" >> output.txt
./client -n 10000 -b 90 -w 5
echo "Test 15, b = 100:" >> output.txt
./client -n 10000 -b 100 -w 5
echo "Test 16, b = 120:" >> output.txt
./client -n 10000 -b 120 -w 5
echo "Test 17, b = 140:" >> output.txt
./client -n 10000 -b 140 -w 5
echo "Test 18, b = 160:" >> output.txt
./client -n 10000 -b 160 -w 5
echo "Test 19, b = 180:" >> output.txt
./client -n 10000 -b 180 -w 5
echo "Test 20, b = 200:" >> output.txt
./client -n 10000 -b 200 -w 5
echo "Finished!"
