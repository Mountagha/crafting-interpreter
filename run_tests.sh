#!/bin/bash

for f in $(find ./tests -type f)
do 
    if [[ $f == *.lox ]]
    then
        echo "Running test for $f"
        $1 $f 
    fi
done
