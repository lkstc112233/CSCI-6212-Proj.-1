#!/bin/bash
rm result
for n in 100 200 400 800 1600 3200 6400 10000 20000 40000 80000 100000 110000 120000 130000 140000 150000 160000 170000 180000 190000 200000
do
	echo Now testing n = ${n}
	echo -n ${n}\	 >> result
	"./main" ${n} >> result
done

echo All work done!
