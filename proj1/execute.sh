#!/bin/bash
# ELEMENTS = 7

echo "Compiling server..."
gcc serverV2.c -o serverV2 -std=c99

for i in `seq 1 7`;do
	echo "Compiling client$i..."
	gcc client$i.c -o client$i
done 

./server&
for i in `seq 1 7`;do
	echo "Executing client$i..."
	./client$i 127.0.0.1 &
done

sleep 5s
killall -9 server
exit 0
