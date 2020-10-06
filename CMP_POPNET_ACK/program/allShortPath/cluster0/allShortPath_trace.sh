rm ../trace/flow_allShortPath_0.txt 
rm ../trace/cpu_allShortPath_0.txt
rm ../trace/L2_allShortPath_0.txt
rm ../trace/router_allShortPath_0.txt
../../../CMP/PMS_trace/cmp0/sim-outorder \
        -redir:sim ./result/test_allShortPath.txt \
       	-redir:dump ./result/test_allShortPath.txt.out \
       	-read_file ../../../pt_net_0.txt \
       	-write_file ../../../pt_net_in.txt \
       	-write_file_backup ../../../pt_net_in_backup.txt \
      	-flow_trace ../trace/flow_allShortPath_0.txt \
       	-router_trace ../trace/router_allShortPath_0.txt \
      	-cpu:trace ../trace/cpu_allShortPath_0.txt \
       	-L2:trace ../trace/L2_allShortPath_0.txt \
       	-config ./config_mesh_ooo_xy_org \
	    -max:barrier 0 -max:inst 100000000000 hello_world.bnc
