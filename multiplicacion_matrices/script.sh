#! /bin/bash
echo MULTIPLICACION POR PROCESOS
for i in 100 200 400 600 800 1000 1200 1400 1600 1800 2000
do
	echo "dimension de N:"$i
	./procesos $i
done

