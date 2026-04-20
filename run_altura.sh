#!/bin/bash
echo "MAPA ALTURA"
for i in $(seq 1 30)
do
    echo "Corrida $i"
    ./busqueda random_small_map.txt 0 0 9 15
done