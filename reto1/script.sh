#! /bin/bash
echo SCRIPT QUE EJECUTA VARIOS N
for i in 100000 1000000 10000000 100000000 1000000000
do
	echo "Numero de iteraciones: "$i
	./ompDart $i
done

