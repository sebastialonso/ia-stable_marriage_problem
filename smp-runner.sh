#!/bin/bash
g++ -std=c++11 gale_shapley.cpp -o gs
echo "5 personas"
./gs InstanceSmti-5-0-0.txt
echo "10 personas"
./gs InstanceSmti-10-0-0.txt
echo "20 personas"
./gs InstanceSmti-20-0-0.txt
echo "40 personas"
./gs InstanceSmti-40-0-0.txt
echo "80 personas"
./gs InstanceSmti-80-0-0.txt
echo "160 personas"
./gs InstanceSmti-160-0-0.txt
echo "320 personas"
./gs InstanceSmti-320-0-0.txt
echo "640 personas"
./gs InstanceSmti-640-0-0.txt
