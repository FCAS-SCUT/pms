#include <exception>
#include <iostream>
#include "stdio.h"
#include "stdlib.h"
#include "index.h"
#include "SStd.h"
#include "SRGen.h"
#include "configuration.h"
#include "sim_foundation.h"
#include "mess_queue.h"
#include "main.h"

extern "C" {
#include "SIM_power.h"
#include "SIM_router_power.h"
#include "SIM_power_router.h"
}
extern long destInfoPass[4];
extern long srcInfoPass[4];
extern long sizeInfoPass;
extern time_type eventTimePass;
extern long counter;
int countNum = 0;//ljh
mess_struct_type MessageInfo[2000];//ljh

mess_queue *network_mess_queue_ptr;
sim_foundation *sim_net_ptr;
configuration *c_par_ptr;
SRGen *random_gen_ptr;

void readfile0();
void readfile1();
int sync(long );

int main(int argc, char *argv[])
{
	try {
		random_gen_ptr = new SRGen;
		configuration c_par(argc, argv);
		cout<<c_par;
		network_mess_queue_ptr = new mess_queue(0.0);
		sim_net_ptr = new sim_foundation;
		network_mess_queue_ptr->simulator();

		
	} catch (exception & e) {
		cerr << e.what();
	}
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

unsigned int GetFileContent(char *filename, vector<string> &filecontent)//read the whole contents to vector
{
	unsigned int counterLine = 0;
	string str;
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
		//cout<<str<<" wangling"<<endl;
		filecontent.push_back(str);
		//memset(str.c_str(), 0, sizeof(str));// clear str
		counterLine++;
	}
	infile.close();
	infile.clear();
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

void ClearFile(char *filename)//clear the text
{
	FILE *fp_pt_net_in = fopen(filename,"w+"); 
	while(!fp_pt_net_in) {
		cerr<<"Can not open source file."<<endl;
		assert(0);
	}
	fclose(fp_pt_net_in);
	fp_pt_net_in = NULL;
}

/*void readpackage()
{
	add_type sor_addr_t;
	add_type des_addr_t;
	time_type event_time_t, current_time_;
	long s[2], d[2], id, temp;
	int number;

	mess_event current_message = * network_mess_queue_ptr-> get_message();	
	current_time_ = current_message.event_start();

	ifstream infile ("../../pt_net_in.txt"); 
	
    while(!infile) {
       cerr<<"Can not open source file."<<endl;
       assert(0);
    }
	while(!infile.eof()){
		infile >> event_time_t;				
	 	for(long i = 0; i < 2; i++) {
			long t; infile >> t;
			s[i] = t;
		}
    		for(long i = 0; i < 2; i++) {
			long t; infile >> t;
			d[i] = t;
		}
		infile >> id;
		infile >> number;
		if (current_time_ >= event_time_t) continue;
		network_mess_queue_ptr->insertMsg(event_time_t, s[0], s[1], d[0], d[1] , id, number);

	}
	infile.close();
	infile.clear();
}*/

int id = 2000;
void readpackage()
{
	add_type sor_addr_t;
	add_type des_addr_t;
	time_type event_time_t;
	time_type current_time_;
	time_type costCycle;
	unsigned long long src_addr_t;
	unsigned long long dst_addr_t;
	long data_length_t;
	int operation_t;
    long long int messageNo_t;
	int mailbox_send[100000];
	int mailbox_receive[100000];
	long s[2], d[2];
	int number = 100;
	long int id = 2000;
    int i,j;
	vector<string> filecontent;
	long long int numResult[100000];//decimal data
   
	unsigned int fileLine = GetFileContent("../../pt_net_in.txt", filecontent);
   	ClearFile("../../pt_net_in.txt");

	if(fileLine){
		for(int i = 0; i < fileLine; i++){
			//cout << filecontent[i] <<endl;
			if(ConvertStringToNum(filecontent[i],numResult)){
				event_time_t = numResult[0];
                costCycle = numResult[1];
				s[0] = numResult[2];
				s[1] = numResult[3];
				src_addr_t = numResult[4];
				d[0] = numResult[5];
				d[1] = numResult[6];
				dst_addr_t = numResult[7];
				data_length_t = numResult[8];
				operation_t = numResult[9];
                messageNo_t = numResult[10];
				for (long i=0; i<data_length_t; i++){
					mailbox_receive[i] = numResult[i+11];					
				}

				MessageInfo[countNum].src1 = s[0];
	            MessageInfo[countNum].src2 = s[1];
	            MessageInfo[countNum].dst1 = d[0];
	            MessageInfo[countNum].dst2 = d[1];
	            MessageInfo[countNum].sim_cycle_m = event_time_t;
	            MessageInfo[countNum].src_cmp_m = 0;
   	            MessageInfo[countNum].src_addr_m = src_addr_t;
	            MessageInfo[countNum].dst_cmp_m = 1;
	            MessageInfo[countNum].dst_addr_m = dst_addr_t;
	            MessageInfo[countNum].data_length_m = data_length_t;
	            MessageInfo[countNum].operation_m = operation_t;
	            MessageInfo[countNum].messageNo_m = messageNo_t;
	            MessageInfo[countNum].costCycle_m = costCycle;
                for(j=0; j<data_length_t; j++)
	                MessageInfo[countNum].array[j] = mailbox_receive[j];
	            countNum++;
                id++;
				network_mess_queue_ptr->insertMsg(event_time_t, s[0], s[1], d[0], d[1] ,messageNo_t, 1);
				
			}		
		}
	}
}

/*void readpackage()
{
	add_type sor_addr_t;
	add_type des_addr_t;
	time_type event_time_t, current_time_;
	long s[2], d[2], id, temp;
	int number;

	mess_event current_message = * network_mess_queue_ptr-> get_message();	
	current_time_ = current_message.event_start();

	ifstream infile ("../../pt_net_in.txt"); 
	
    while(!infile) {
       cerr<<"Can not open source file."<<endl;
       assert(0);
    }
	while(!infile.eof()){
		infile >> event_time_t;				
	 	for(long i = 0; i < 2; i++) {
			long t; infile >> t;
			s[i] = t;
		}
    		for(long i = 0; i < 2; i++) {
			long t; infile >> t;
			d[i] = t;
		}
		infile >> id;

		if (current_time_ >= event_time_t) continue;
		network_mess_queue_ptr->insertMsg(event_time_t, s[0], s[1], d[0], d[1] ,id);

	}
	infile.close();
	infile.clear();
}
*/
 
int sync(long n)
{
	long  synctime0, synctime1;
	long  synctime2, synctime3;
	fstream sync_1_net ("../../sync_1_net.txt"); 
	fstream sync_0_net ("../../sync_0_net.txt"); 
	fstream sync_2_net ("../../sync_2_net.txt"); 
	fstream sync_3_net ("../../sync_3_net.txt"); 
	
	while(sync_1_net == NULL && sync_0_net == NULL && sync_2_net == NULL && sync_3_net == NULL){
		cout << "Sync is not ready!!";
	        	assert(0);
	}
	
	sync_0_net >> synctime0;
	sync_1_net >> synctime1;
	sync_2_net >> synctime2;
	sync_3_net >> synctime3;
	if((synctime0 - 1000) == n && (synctime1 - 1000) == n && (synctime2 - 1000) == n && (synctime3 - 1000) == n){
		sync_0_net.close();
		sync_1_net.close();
		sync_2_net.close();
		sync_3_net.close();		
		return 0;
	}
	else{
		sync_0_net.close();
		sync_1_net.close();
		sync_2_net.close();
		sync_3_net.close();
		return 1;
	}
}