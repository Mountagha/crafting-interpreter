#!/bin/bash

for f in $(find ./tests -type f)
do 
    if [[ $f == *.lox ]]
    then
        $1 $f 
    fi
done
