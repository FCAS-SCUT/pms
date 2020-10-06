#include <exception>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <assert.h>
#include <string.h>
#include <vector>
#include <algorithm>
#define MESH 8
#define NODENUM 80
#

using namespace std;

typedef struct router_pair{
    int router_id;
    long  counter;
} RouterPair;

vector <RouterPair> V;
//RouterPair array[80];

bool compare(const RouterPair &a,const RouterPair &b)
{
	return a.counter > b.counter;
}

int main()
{
	int Rx,Ry,flit_size;
    long flit_id,ID;
    string  event_time;
    long long int index[NODENUM];
    int i;
    long long k=0;
    RouterPair RouterId;
    V.resize(80);

    for(i=0;i<NODENUM;i++)//initialization
       index[i] = 0;

    ifstream infile;
    ofstream outfile;
    /*infile.open("../router_allShortPath_0.txt"); 
    //infile.open("./aaa.txt"); 
    
    if(!infile) {
       cerr<<"Can not open source file."<<endl;
       assert(0);
    }
    
    infile >> Rx >> Ry >> flit_id >> flit_size >> event_time;
    cout << Rx << " " << Ry << " " << flit_id << " " <<flit_size <<" "<< event_time << endl;
    printf("File analyze is beginning...\n");
    infile.seekg(0,ios_base::beg);
	
    while(!infile.eof()){
    //while(1){
		infile >> Rx >> Ry >> flit_id >> flit_size >> event_time;
        cout << Rx << " " << Ry << " " << flit_id << " " <<flit_size <<" "<< event_time << endl;
       // if(infile.eof())   break;     
        //cout << "This is " << k << " " << "loop" << " " << endl;
        ID = Rx*MESH +Ry;
        index[ID]++; 
        k++;
        //if(infile.eof())   break;     
	}
    printf("File analyze is over...\n");
	infile.close();
    printf("The index[] is:\n");
    for(i=0;i<NODENUM;i++)
        printf("%d ",index[i]);

    for(i=0; i<80; i++){
        V[i].router_id = i;
        V[i].counter = index[i];
    }
        
    sort(V.begin(),V.end(),compare);

    printf("The analyse result is:\n");*/
    outfile.open("../../input_file/noc/allShortPath_1.sorted");
    //outfile.open("./allShortPath_0.sorted");
    for(i=0;i<80;i++)            
        outfile << "R" << " " << i << " " << "0.5" << endl;
   /* for(i=20;i<30;i++)
        outfile << "R" << " " << V[i].router_id << " " << "0.5" << endl;
    for(i=30;i<50;i++)
        outfile << "R" << " " << V[i].router_id << " " << "0.33" << endl;
    for(i=50;i<80;i++)
        outfile << "R" << " " << V[i].router_id << " " << "0.1" << endl;
    //for(i=0;i<NODENUM;i++)
    //        printf("%d ",index[i]);*/
    printf("\n");
    outfile.close();
    return 0;
}

