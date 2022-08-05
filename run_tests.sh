#!/bin/bash

for f in $(find ./tests -type f)
do 
    if [[ $f == *.lox ]]
    then
        echo -e "==== Running test for $f====\n"
        $1 $f 
        echo -e "\n"
    fi
done
