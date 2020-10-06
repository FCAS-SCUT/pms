#!/bin/sh
for name in *.txt
do
	rm $name 
	touch $name
done
