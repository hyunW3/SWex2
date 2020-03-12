#!/bin/bash

echo $1 "$2" $3 "$4" $5
if [ $2 = "mul" ]; then
	if [ $4 = "mul" ] ; then
		echo `expr $1 \* $3 \* $5`
	else 
		echo `expr $1 \* $3 + $5`
	fi
elif [ $2 == "+" ]; then
	if [ $4 == "+" ]; then
		echo `expr $1 + $3 + $5`
	else 	
		echo `expr $1 + $3 \* $5`
	fi
fi
