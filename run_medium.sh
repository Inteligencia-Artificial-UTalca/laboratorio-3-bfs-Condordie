#!/bin/bash
echo "MAPA MEDIUM"
for  i in $(seq 1 30)
do
    echo "---Corrida $i ---"
    ./app medium.txt 1 1 9 14
done