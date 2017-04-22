#!/bin/bash

yapc=../yapc

for file in ./*.y; do
    echo "testing for $file"
    $yapc $file
done
