if [ -f "../output_file/all/fastsort_1.txt" ];then
        rm ../output_file/all/fastsort_1.txt
fi
~/huarzail/CMP_POPNET/CMP/PMS_dvfs/cmp0/sim-outorder \
        -redir:sim ../dvfs_log/fastsort_1.txt \
        -redir:dump ../dvfs_log/fastsort_1.out \
       	-read_file ../../../pt_net_1.txt \
       	-write_file ../../../pt_net_in.txt \
       	-write_file_backup ../../../pt_net_in_backup.txt \
        -sorted:router ../input_file/noc/fastsort_1.sorted\
        -sorted:processor ../input_file/processor/fastsort_1.sorted \
        -sorted:l2 ../input_file/memory/fastsort.sorted_1\
        -output:cycle ../output_file/all/fastsort_1.txt\
        -config config_mesh_ooo_xy_dvfs \
		-max:barrier 0 -max:inst 1000000 hello.BNC
