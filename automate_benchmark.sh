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
			for l in 1 2 3 4 5 6 7 8 9 10
			do
				(( count= $count + 1 ))
				echo "executing run $count:"
				echo "time ./benchmark $i $j $k < queries/queries_175_325 > benchmark-output-$i-$j-$k-run$l"
				time ./benchmark $i $j $k < queries/queries_175_325 > benchmark-output-$i-$j-$k-run$l
			done
		done
	done
done