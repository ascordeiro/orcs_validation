#!/bin/bash

while IFS= read -r opt
do 
    rm $opt
    g++ $opt.c -O3 -mavx2 -march=native -o $opt
done < tests_x86.txt