#!/bin/sh
sh ../../clear.sh

#echo cluster0
gnome-terminal -x bash -c "cd cluster0/;sh ./fastsort_dvfs.sh;exec bash;"
#echo cluster1
gnome-terminal -x bash -c "cd cluster1/;sh ./fastsort_dvfs.sh;exec bash;"
#echo cluster2
#gnome-terminal -x bash -c "cd program/cluster2/;sh ./run.sh;exec bash;"
#echo cluster3
#gnome-terminal -x bash -c "cd program/cluster3/;sh ./run.sh;exec bash;"

#echo popnet
gnome-terminal -x bash -c "cd ../../POPNET/popnet/;sh run.sh;exec bash;"

exit 0
