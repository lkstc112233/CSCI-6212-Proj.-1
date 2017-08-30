#!/bin/bash
rm result
for n in 1 2 4 8 10 20 30 40 50 60 70 80 90 100 200 300 500 1000 2000 5000 10000 20000 50000 100000 200000
do
	echo Now testing n = ${n}
	echo -n ${n}\	 >> result
	"./CSCI 6212 Proj. 1" ${n} >> result
done

echo All work done!