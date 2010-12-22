#!/bin/bash

configFiles=(config configPAX configP)
paritioning=(0 1)
threads=(5 10 15)

count=0
for i in ${configFiles[@]};
do
	for j in ${paritioning[@]};
	do
		for k in ${threads[@]};
		do
			(( count= $count + 1 ))
			echo "executing run $count:"
			echo "valgrind --tool=cachegrind ./benchmark $i $j $k < queries/queries_175_325 > benchmark-output-$i-$j-$k"
			./benchmark $i $j $k < queries/queries_175_325 > benchmark-output-$i-$j-$k
		done
	done
done