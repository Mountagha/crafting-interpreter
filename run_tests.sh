#!/bin/bash

for f in $(find ./tests -type f)
do 
    if [[ $f == *.lox ]]
    then
        echo -e "==== Running test for $f====\n"
        ./build/interpreter $f 
        echo -e "\n"
    fi
done
