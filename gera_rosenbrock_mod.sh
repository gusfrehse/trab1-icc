#!/bin/bash

N=$1
echo $N
for i in $(seq 1 $((N-1)))
do
	echo -n "100*(x$(($i+1))-x$i^2)^2+(1-x$i)^2+"
done
echo "0"
for i in $(seq 1 $N)
do
	echo -n "2.01 "
done
echo -e "\n0\n50\n"
