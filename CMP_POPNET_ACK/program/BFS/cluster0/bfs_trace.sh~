rm ../trace/flow_bfs_0.txt 
rm ../trace/cpu_bfs_0.txt
rm ../trace/L2_bfs_0.txt
rm ../trace/router_bfs_0.txt
~/huarzail/CMP_POPNET/CMP/PMS_trace/cmp0/sim-outorder \
        -redir:sim ./result/test_bfs.txt \
       	-redir:dump ./result/test_bfs.txt.out \
       	-read_file ../../../pt_net_0.txt \
       	-write_file ../../../pt_net_in.txt \
       	-write_file_backup ../../../pt_net_in_backup.txt \
      	-flow_trace ../trace/flow_bfs_0.txt \
       	-router_trace ../trace/router_bfs_0.txt \
      	-cpu:trace ../trace/cpu_bfs_0.txt \
       	-L2:trace ../trace/L2_bfs_0.txt \
       	-config ./config_mesh_ooo_xy_org \
	    -max:barrier 0 -max:inst 100000000 hello.BNC
