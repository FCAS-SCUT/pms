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
#define CYCLE 5161296

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
    float a[80];
    int a1=0,a2=0,a3=0,a4=0,a5=0,a6=0;

    for(i=0;i<NODENUM;i++)//initialization
       index[i] = 0;

    ifstream infile;
    ofstream outfile;
    infile.open("../router_allShortPath_1.txt"); 
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
		infile >> Rx >> Ry >> flit_id >> flit_size >> event_time;
        cout << Rx << " " << Ry << " " << flit_id << " " <<flit_size <<" "<< event_time << endl;
        ID = Rx*MESH +Ry;
        index[ID]++; 
        k++;
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

    for(i=0;i<NODENUM;i++)
        a[i]=V[i].counter/(float)CYCLE;

    for(i=0;i<NODENUM;i++)
        if(0<a[i]<0.05)
            a1++;
        else if(0.05<=a[i]<0.1)
            a2++;
		else if(0.1<=a[i]<0.15)
            a3++;
        else if(0.15<=a[i]<0.2)
            a4++;
        else if(0.2<=a[i]<0.25)
            a5++;
		else
			a6++;

    printf("The percent of flit is:\n");

    printf("a1=%d ,a2=%d ,a3=%d ,a4=%d ,a5=%d ,a6=%d",a1,a2,a3,a4,a5,a6);

    printf("The filt/cycle is:\n");
    for(i=0;i<NODENUM;i++)                  
        cout  << "R" << " " << V[i].router_id << " " << a[i]/*V[i].counter*/ << endl;

    outfile.open("./fastsort_0.percent");
    for(i=0;i<NODENUM;i++)            
        outfile << "R" << " " << V[i].router_id << " " << V[i].counter/CYCLE << endl;
    outfile.close();
    return 0;
}

