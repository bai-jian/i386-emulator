#!/bin/bash

cmd="r\nq"

for file in $@; do
	echo "testfile: $file"
	echo -e $cmd | ./nemu $file | grep 'nemu:'
	echo ""
done

