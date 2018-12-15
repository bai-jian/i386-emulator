#!/bin/bash

nemu=$1
shift
cmd="r\nc\nq"
for file in $@; do
	echo "testfile: $file"
	echo -e $cmd | $nemu $file | grep 'nemu:'
	echo ""
done

