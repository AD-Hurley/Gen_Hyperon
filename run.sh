#!/bin/bash

for i in {1..100}
do
	echo "/random/setSeeds $RANDOM $RANDOM" &> run.mac
	echo "/run/beamOn 1000000" >> run.mac
	
	./gen_Hyperon
	mv output.root ./files_out/cmResDets_100x1M/output_$i.root
done
