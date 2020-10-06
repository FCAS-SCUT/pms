if [ -f "../output_file/all/allShortPath_0.txt" ];then
        rm ../output_file/all/allShortPath_0.txt
fi
~/huarzail/CMP_POPNET_ACK/CMP/PMS_dvfs/cmp0/sim-outorder \
        -redir:sim ../dvfs_log/allShortPath_0.txt \
        -redir:dump ../dvfs_log/allShortPath_0.out \
       	-read_file ../../../pt_net_0.txt \
       	-write_file ../../../pt_net_in.txt \
       	-write_file_backup ../../../pt_net_in_backup.txt \
        -sorted:router ../input_file/noc/allShortPath_0.sorted\
        -sorted:processor ../input_file/processor/allShortPath_0.sorted \
        -sorted:l2 ../input_file/memory/allShortPath_0.sorted\
        -output:cycle ../output_file/all/allShortPath_0.txt\
        -config config_mesh_ooo_xy_dvfs \
		-max:barrier 0 -max:inst 1000000000 hello.BNC