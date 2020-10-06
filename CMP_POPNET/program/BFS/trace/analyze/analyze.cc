#include <exception>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <assert.h>
#include <string.h>
#define MESH 8
#define NODENUM 80

using namespace std;

typedef struct router_pair{
    int router_id;
    long  counter;
} RouterPair;

int main()
{
	int Rx,Ry,flit_size;
    long long int flit_id,ID,event_time;
    long long int index[NODENUM];
    int i;

    for(i=0;i<NODENUM;i++)//initialization
       index[i] = 0;

    ifstream infile;
    infile.open("../router_allShortPath_0.txt"); 
    
    if(!infile) {
       cerr<<"Can not open source file."<<endl;
       assert(0);
    }
    
    printf("File analyze is beginning...\n");
    infile.seekg(0,ios_base::beg);
	
    while(!infile.eof()){
		infile >> Rx >> Ry >> flit_id >> flit_size >> event_time;
        ID = Rx*MESH +Ry;
        index[ID]++;       	 	
	}
    printf("File analyze is over...\n");
	infile.close();
	infile.clear();
    

    printf("The analyse result is:\n");
    for(i=0;i<NODENUM;i++)
            printf("%d ",index[i]);
    printf("\n");
    return 0;
}

