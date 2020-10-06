rm ../trace/flow_fastsort_1.txt 
rm ../trace/cpu_fastsort_1.txt
rm ../trace/L2_fastsort_1.txt
rm ../trace/router_fastsort_1.txt
~/huarzail/CMP_POPNET_ACK/CMP/PMS_trace/cmp1/sim-outorder \
        -redir:sim ./result/test_fastsort.txt \
       	-redir:dump ./result/test_fastsort.txt.out \
       	-read_file ../../../pt_net_1.txt \
       	-write_file ../../../pt_net_in.txt \
       	-write_file_backup ../../../pt_net_in_backup.txt \
      	-flow_trace ../trace/flow_fastsort_1.txt \
       	-router_trace ../trace/router_fastsort_1.txt \
      	-cpu:trace ../trace/cpu_fastsort_1.txt \
       	-L2:trace ../trace/L2_fastsort_1.txt \
       	-config ./config_mesh_ooo_xy_org \
	    -max:barrier 0 -max:inst 10000000 hello.BNC