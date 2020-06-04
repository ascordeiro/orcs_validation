#!/bin/bash

while IFS= read -r opt
do 
    rm $opt
    g++ $opt.c -O3 -DNOINLINE -o $opt
done < tests_vima.txt