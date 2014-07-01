#!/bin/bash
g++ -std=c++11 main.cpp -o output
echo "5 personas"
./output InstanceSmti-5-0-0.txt
echo "10 personas"
./output InstanceSmti-10-0-0.txt
echo "20 personas"
./output InstanceSmti-20-0-0.txt
echo "40 personas"
./output InstanceSmti-40-0-0.txt
echo "80 personas"
./output InstanceSmti-80-0-0.txt
echo "160 personas"
./output InstanceSmti-160-0-0.txt
echo "320 personas"
./output InstanceSmti-320-0-0.txt
