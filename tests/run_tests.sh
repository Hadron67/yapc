#!/bin/bash

yapc=../yapc

for file in ./*.y; do
    echo "testing for $file"
    $yapc --show-lah $file
done
