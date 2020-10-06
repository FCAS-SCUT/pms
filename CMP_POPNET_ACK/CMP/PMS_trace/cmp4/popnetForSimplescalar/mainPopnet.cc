#include <exception>
#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <sstream>
#include <stdlib.h>
#include "index.h"
#include "SStd.h"
#include "SRGen.h"
#include "configuration.h"
#include "sim_foundation.h"
#include "mess_queue.h"
extern "C" {
#include "SIM_power.h"
#include "SIM_router_power.h"
#include "SIM_power_router.h"
#include "../host.h"
#include "../machine.h"
#include "../network.h"
#include "../options.h"
#define MULTI_VC

#include "../sim-outorder.h"//ljh
#include "syscall.h"//ljh
Regedno M;
#include "../MTA.h"
#include "../context.h"
#include <sys/file.h>

extern context *thecontexts[MAXTHREADS];
extern struct QuiesceStruct QuiesceAddrStruct[CLUSTERS];

/*********************ljh******************************/
int countNum = 0;
mess_struct_type MessageInfo[2000];
counter_t packageDelay;
/*********************ljh******************************/

int mainPopnet(int array_size);
void popnet_options(struct opt_odb_t *odb);
int popnetRunSim(long long int sim_cycle);
int popnetMsgComplete(long w, long x, long y, long z, long long int stTime, long long int msgNo);
int popnetBufferSpace(long s1, long s2, int opt);
void power_report(FILE *fd);
void delay_report(FILE *fd);
extern unsigned long long sim_cycle;
extern long numrouters;
}

mess_queue *network_mess_queue_ptr;
sim_foundation *sim_net_ptr;
configuration *c_par_ptr;
SRGen *random_gen_ptr;
int msgCompleteCount;
long long int messageQueued = 0;
char *mesh_input_buffer_size;
char *mesh_output_buffer_size;
char *mesh_flit_size;
char *vc_num;
char *routing_algr;
int link_width;

char *flowFile;   //Tengfei Wang 20140224
char *routerFile; //Tengfei Wang 20140224
char *readFile;  //Jianhua Li 20150521
char *writeFile;  //Jianhua Li 20150521
char *writeFileBackup;  //Jianhua Li 20150521

void popnet_options(struct opt_odb_t *odb)
{
    opt_reg_string (odb, "-mesh_network:buffer_size", "", &mesh_input_buffer_size, /* default */"12", 
		    /* print */ TRUE, /* format */ NULL);
    opt_reg_string (odb, "-mesh_network:outbuffer_size", "", &mesh_output_buffer_size, /* default */ "1", 
		    /* print */ TRUE, /* format */ NULL);
    opt_reg_string (odb, "-mesh_network:mesh_flit_size", "", &mesh_flit_size, /* default */"64", 
		    /* print */ TRUE, /* format */ NULL);
    opt_reg_string (odb, "-mesh_network:vc_num", "", &vc_num, /* default */"1", 
		    /* print */ TRUE, /* format */ NULL);
    opt_reg_string (odb, "-mesh_network:routing_algr", "", &routing_algr, /* default */"0", 
		    /* print */ TRUE, /* format */ NULL);
    opt_reg_int (odb, "-mesh_network:phit_size", "", &link_width, /* default */64, 
		    /* print */ TRUE, /* format */ NULL);
//
opt_reg_string(odb,"-flow_trace","",&flowFile,/*default*/"",/*print*/TRUE,/*format*/NULL);   //Tengfei Wang 20140224
opt_reg_string(odb,"-router_trace","",&routerFile,/*default*/"",/*print*/TRUE,/*format*/NULL); //Tengfei Wang 20140224
opt_reg_string(odb,"-read_file","",&readFile,/*default*/"",/*print*/TRUE,/*format*/NULL); //Jianhua Li 20140224
opt_reg_string(odb,"-write_file","",&writeFile,/*default*/"",/*print*/TRUE,/*format*/NULL); //Jianhua Li 20140224
opt_reg_string(odb,"-write_file_backup","",&writeFileBackup,/*default*/"",/*print*/TRUE,/*format*/NULL); //Jianhua Li 20140224
//

}
void my_itoa(int n, char **s)
{
	switch (n)
	{
		case 1: (*s) = "1"; break;
		case 2: (*s) = "2"; break;
		case 3: (*s) = "3"; break;
		case 4: (*s) = "4"; break;
		case 5: (*s) = "5"; break;
		case 6: (*s) = "6"; break;
		case 7: (*s) = "7"; break;
		case 8: (*s) = "8"; break;
		case 9: (*s) = "9"; break;
		case 10: (*s) = "10"; break;
		case 11: (*s) = "11"; break;
		case 12: (*s) = "12"; break;
		default: (*s) = "6";
	}
	return;
}
int mainPopnet(int array_size)
{
	char *arg[23];
	char temp[23][32];
	char *my_array_size;
    packageDelay = 0;//ljh
	my_itoa(array_size, &my_array_size);
	strcpy(temp[0], "popnet");
	strcpy(temp[1], "-A");
	strcpy(temp[2], my_array_size);
	strcpy(temp[3], "-c");
	strcpy(temp[4], "2");
	strcpy(temp[5], "-V");
	strcpy(temp[6], vc_num);
	strcpy(temp[7], "-B");
	strcpy(temp[8], mesh_input_buffer_size);
	strcpy(temp[9], "-O");
	strcpy(temp[10], mesh_output_buffer_size);
	strcpy(temp[11], "-F");
	strcpy(temp[12], mesh_flit_size);
	strcpy(temp[13], "-L");
	strcpy(temp[14], "1000");
	strcpy(temp[15], "-T");
	strcpy(temp[16], "20000");
	strcpy(temp[17], "-r");
	strcpy(temp[18], "1");
	strcpy(temp[19], "-I");
	strcpy(temp[20], "bench");
	strcpy(temp[21], "-R");
	strcpy(temp[22], routing_algr);

	arg[0] = temp[0];
	arg[1] = temp[1];
	arg[2] = temp[2];
	arg[3] = temp[3];
	arg[4] = temp[4];
	arg[5] = temp[5];
	arg[6] = temp[6];
	arg[7] = temp[7];
	arg[8] = temp[8];
	arg[9] = temp[9];
	arg[10] = temp[10];
	arg[11] = temp[11];
	arg[12] = temp[12];
	arg[13] = temp[13];
	arg[14] = temp[14];
	arg[15] = temp[15];
	arg[16] = temp[16];
	arg[17] = temp[17];
	arg[18] = temp[18];
	arg[19] = temp[19];
	arg[20] = temp[20];
	arg[21] = temp[21];
	arg[22] = temp[22];
	
		 
	 	
	cout << "in here: C++ program" << endl;
	try {
		random_gen_ptr = new SRGen;
		c_par_ptr = new configuration(23, arg);//argc, argv);
		network_mess_queue_ptr = new mess_queue(0.0);
		sim_net_ptr = new sim_foundation;
		numrouters = sim_net_ptr->router_counter();

	} catch (exception & e) {
		cerr << e.what();
	}
}

int popnetRunSim(long long int sim_cycle)
{ 

	msgCompleteCount = 0;
	//       pointer_t = mess_record;

	network_mess_queue_ptr->simulator(sim_cycle);
	
	if(msgCompleteCount > 0)
		cout << "Messages completed this cycle!!!!!" << endl;
	return 0;
}


//sim_cycle:src_addr   size:data_length    msgNo:src_cmp     addr:dst_addr   vc:dst_cmp  operation:operation 
void popnetMessageInsert(long s1, long s2, long d1, long d2, long long int sim_cycle, long size, counter_t msgNo, md_addr_t dst_cmp, int op, int vc,long long int src_addr, long data_length, counter_t src_cmp, md_addr_t dst_addr, int operation)
{
    int mailbox[100000];
    int i;
	int read_data = 0; 

    if(dst_cmp < 0)
	    dst_cmp = dst_cmp * -1;
	if(src_cmp == 7)
		cout << "New message inserted" << sim_cycle << " " << s1 << " " << s2 << " " << d1 << " " << d2 << " " << src_cmp << endl;
    //-----write commuication flow trace----------//
    //---------Tengfei Wang 20140224--------------//
    ofstream flow;
    flow.open(flowFile,ios::app);
    flow<<s1<<"  "<<s2<<"   "<<d1<<"  "<<d2<<"   "<<sim_cycle<<"  "<<msgNo<<"   "<<size<<endl;
    flow.close();
	
	if(d1==0&&d2==0){
		MessageInfo[countNum].src1 = s1;
	    MessageInfo[countNum].src2 = s2;
	    MessageInfo[countNum].dst1 = d1;
	    MessageInfo[countNum].dst2 = d2;
	    MessageInfo[countNum].sim_cycle_m = sim_cycle;
	    MessageInfo[countNum].src_cmp_m = src_cmp;
   	    MessageInfo[countNum].src_addr_m = src_addr;
	    MessageInfo[countNum].dst_cmp_m = dst_cmp;
	    MessageInfo[countNum].dst_addr_m = dst_addr;
	    MessageInfo[countNum].data_length_m = data_length;
	    MessageInfo[countNum].operation_m = operation;
	    MessageInfo[countNum].messageNo_m = msgNo;
	    countNum++;
        if(firstFlag == 0){
            firstFlag == 1;
	network_mess_queue_ptr->insertMsg(MessageInfo[sendNum].src1, MessageInfo[sendNum].src2, MessageInfo[sendNum].dst1, MessageInfo[sendNum].dst2, MessageInfo[sendNum].sim_cycle_m, MessageInfo[sendNum].size_m, MessageInfo[sendNum].messageNo_m, MessageInfo[sendNum].dst_cmp_m, (long)MessageInfo[sendNum].vc_m);
        }

    } 
    else{
        network_mess_queue_ptr->insertMsg(s1, s2, d1, d2, sim_cycle, size, msgNo, dst_cmp, (long)vc);
    }
    // if (d1 ==0 && d2 == 0)
	//	cout << "Insert - Time: " << sim_cycle << "Source ID: " << s1 << s2 << "Message no: " << msgNo << endl;


	messageQueued++;
}

int popnetBufferSpace(long s1, long s2, int opt)
{
	add_type sor_addr_t;
	sor_addr_t.resize(2);
	sor_addr_t[0] = s1;
	sor_addr_t[1] = s2;
//	return sim_foundation::wsf().router(sor_addr_t).isBufferFull();
#ifdef MULTI_VC
	if(opt == -1)
		return sim_foundation::wsf().router(sor_addr_t).suggestVC();
	return sim_foundation::wsf().router(sor_addr_t).isBufferFull(0, opt) ? -1:opt;
#else
	return sim_foundation::wsf().router(sor_addr_t).isBufferFull(0, 0);
#endif
}

int finishedMessage(long w, long x, long y, long z, long long int stTime, long long int msgNo)
{
	if(msgNo == 7)
		cout << "packet received" << stTime << " " << w << " " << x << " " << y << " " << z << " " << msgNo << endl;
	if(popnetMsgComplete(w, x, y, z, stTime, msgNo))
	{
		messageQueued--;
		return 1;
	}
	return 0;
}
void delay_report(FILE *fd)
{
	vector<sim_router_template>::const_iterator first = 
							sim_foundation::wsf().inter_network().begin();
	vector<sim_router_template>::const_iterator last = 
							sim_foundation::wsf().inter_network().end();
	double total_delay = 0;
	//calculate the total delay
	first = sim_foundation::wsf().inter_network().begin();
	for(; first != last; first++) {
		total_delay += first->total_delay();
	}
	long tot_f_t = mess_queue::wm_pointer().total_finished();

	fprintf(fd,"Total finished packet:		    %d\n",tot_f_t);
	fprintf(fd,"Total delay in popNet:		    %g\n",total_delay);
	fprintf(fd,"Average delay in popNet:		%g\n", total_delay/tot_f_t);
}

void power_report(FILE *fd)
{
    sim_foundation::wsf().simulation_results();
    double total_power = 0;
	long total_inject = 0, inject = 0, receive = 0;
    for(int i = 0; i < sim_foundation::wsf().ary_size(); i++)
    {
	for(int j = 0; j < sim_foundation::wsf().ary_size(); j++)
	{
	    add_type sor_addr_t;
	    sor_addr_t.resize(2);
	    sor_addr_t[0] = i;
	    sor_addr_t[1] = j;
	    //total_power += sim_foundation::wsf().router(sor_addr_t).power_report(fd, sim_foundation::wsf().ary_size()*sim_foundation::wsf().ary_size());
	    total_power += sim_foundation::wsf().router(sor_addr_t).power_report(fd, sim_foundation::wsf().ary_size()*sim_foundation::wsf().ary_size());
		inject = sim_foundation::wsf().router(sor_addr_t).packet_counter();
		total_inject +=inject;
		receive = sim_foundation::wsf().router(sor_addr_t).receive_counter();
		fprintf(fd,"The router %d %d send packet:       %d\n", i, j, inject);
		fprintf(fd,"The router %d %d receive packet:    %d\n", i, j, receive);
	}
    }
    fprintf(fd,"Total Network power:   %g\n", total_power);
	fprintf(fd,"Total inject packet:   %d\n", total_inject);
}

std::vector<std::string> split(std::string str,std::string pattern)//divide the string
{
    std::string::size_type pos;
    std::vector<std::string> result;
    str+=pattern;
    int size=str.size();

    for(int i=0; i<size; i++)
    {
        pos=str.find(pattern,i);
        if(pos<size)
        {
            std::string s=str.substr(i,pos-i);
            result.push_back(s);
            i=pos+pattern.size()-1;
        }
    }
    return result;
}

unsigned int GetFileContent(char* filename, vector<string> &filecontent)//read the whole contents to vector
{
	unsigned int counterLine = 0;
	string str;
	FILE *fp = NULL;
 
    if ((fp = fopen(filename, "r")) == NULL){ 
        printf("file open error!\n");
        exit(0);
    }
    if (flock(fp->_fileno, LOCK_EX) != 0) 
        printf("file lock by others\n");

    fclose(fp); 
	ifstream infile (filename);
	while(!infile) {
		cerr<<"Can not open source file."<<endl;
		assert(0);
	}
	
	infile.seekg(ios::beg);//file point to begin
	while(!infile.eof()){
		getline(infile,str);
		if(infile.fail()) {
			break;
		}
		filecontent.push_back(str);
		//memset(str.c_str(), 0, sizeof(str));// clear str
		counterLine++;
	}
	infile.close();
	infile.clear();
    flock(fp->_fileno, LOCK_UN); 
	return counterLine;  // return the number of file line 
}

bool ConvertStringToNum(string lineSrc, long long int *numDest)
{
	bool bRet = false;

	std::vector<std::string> stringResult = split(lineSrc," "); //divide the string to single value
	//long long int llintResult[100];
	if(stringResult.size()>0){
		for(int i=0; i<stringResult.size(); i++)
			{
			//	cout<<stringResult[i]<<" ";
				char *stopstring;
				numDest[i] = strtoll(stringResult[i].c_str(), &stopstring, 10);//convert string -> int
			//	cout<<numDest[i]<<endl;
			}
		bRet = true;
	}
	return bRet;
}

void ClearFile(char* filename)//clear the text
{
	FILE *fp_pt_net_in = fopen(filename,"w+"); 
	while(!fp_pt_net_in) {
		cerr<<"Can not open source file."<<endl;
		assert(0);
	}
	fclose(fp_pt_net_in);
	fp_pt_net_in = NULL;
}

void readfile()
{
	add_type sor_addr_t;
	add_type des_addr_t;
	time_type event_time_t;
	time_type current_time_;
	unsigned long long src_addr_t;
	unsigned long long dst_addr_t;
	unsigned long long costCycle;
	unsigned long long Time;
	long data_length_t;
	int operation_t;
    long long int messageNo_t;
	int mailbox_send[100000];
	int mailbox_receive[100000];
	long s[2], d[2];
	int number = 100;
	long int id = 2000;
	vector<string> filecontent;
	long long int numResult[100000];//decimal data
	//wangling
    int signal_wl = 0xaa;//0x55--false;0xaa--true
    int *pt_signal_wl = &signal_wl;
    int src_cmp;
    int dst_cmp;
   
	unsigned int fileLine = GetFileContent(readFile, filecontent);
   	ClearFile(readFile);
	if(fileLine){
		for(int i = 0; i < fileLine; i++){
			//cout << filecontent[i] <<endl;
			if(ConvertStringToNum(filecontent[i],numResult)){	
				event_time_t = numResult[0];
				costCycle = numResult[1];
                src_cmp = numResult[2];
				src_addr_t = numResult[3];
                dst_cmp = numResult[4];
				dst_addr_t = numResult[5];
				data_length_t = numResult[6];
				operation_t = numResult[7];
                messageNo_t = numResult[8];
				for (long i=0; i<data_length_t; i++){
					mailbox_receive[i] = numResult[i+9];					
				}
                packageDelay += costCycle;
                Time = 0;


				if(operation_t == 1){ //operation equal to 1 is stand for other clusters want to write this cluster.
					write_memory(data_length_t, dst_addr_t, mailbox_receive);
					//write_memory(1,(md_addr_t)0x1ff500000LL, pt_signal_wl);
				}
				else if(operation_t == 0){ //operation equal to 0 is stand for this cluster want to read other clusters.
					operation_t = 1;
					read_memory(mailbox_send, data_length_t, dst_addr_t);
					ofstream fout(writeFile, ios::app);
					if(!fout){   
						cerr<<"Can not open source file."<<endl;
						assert(0);
					}
					fout << event_time_t <<" " <<Time << " "  << dst_cmp <<" " << dst_addr_t <<" " << src_cmp << " " << src_addr_t <<" " << data_length_t <<" " << operation_t <<" " << messageNo_t <<" ";      
					for(int i=0; i<data_length_t; i++){
						fout << mailbox_send[i] <<" ";
					}
					fout << endl;
					fflush(stdout);
					fout.close();
					ofstream fout_b(writeFileBackup, ios::app);
					if(!fout_b){   
						cerr<<"Can not open source file."<<endl;
						assert(0);
					}
					fout_b << event_time_t <<" " <<Time << " " << dst_cmp <<" " << dst_addr_t <<" " << src_cmp <<" " << src_addr_t <<" " << data_length_t <<" " << operation_t <<" "<< messageNo_t<< " ";
      
					for(int i=0; i<data_length_t; i++){
						fout_b << mailbox_send[i] <<" ";
					}
					fout_b << endl;
					fflush(stdout);
					fout_b.close();
				}
			}		
		}
	}
}
//
int sync(long long n)
{
	long  synctime;
	fstream sync_net ("/home/w/huarzail/CMP_POPNET/sync_net.txt"); 

	while(sync_net == NULL){
		cout << "Sync_net is not ready!!";
	        assert(0);
	}	

	sync_net >> synctime;

	if(synctime == n ){
		sync_net.close();		
		return 0;
	}
	else {
		sync_net.close();
		return 1;
	}
}
