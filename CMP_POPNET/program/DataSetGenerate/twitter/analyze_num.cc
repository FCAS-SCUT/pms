#include <exception>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <assert.h>
#include <string.h>
#include <vector>
#include <algorithm>

#define NODENUM 256

using namespace std;

typedef struct router_pair{
    int router_id;
    long  counter;
} RouterPair;

vector <RouterPair> V;

bool compare(const RouterPair &a,const RouterPair &b)
{
	return a.counter > b.counter;
}

int main()
{
	int Rx,Ry,weight;
    long long int index[NODENUM];
    int i;
    int max=0;
    RouterPair RouterId;
    V.resize(NODENUM);

    for(i=0;i<NODENUM;i++)//initialization
       index[i] = 0;

    ifstream infile;
    ofstream outfile;
    //infile.open("./twittercrawlweight_int.txt"); 
    infile.open("./input.txt"); 
    //outfile.open("./input.txt");
    
    if(!infile) {
       cerr<<"Can not open source file."<<endl;
       assert(0);
    }
    
    infile >> Rx >> Ry >> weight;
    cout << Rx << " " << Ry << " " <<weight << endl;
    printf("File analyze is beginning...\n");
    infile.seekg(0,ios_base::beg);
	
    while(!infile.eof()){
		infile >> Rx >> Ry >> weight;
        cout << Rx << " " << Ry << " " << weight << endl;
        //if(Rx>max || Ry>max)
        //        continue;
        if(Rx>max)
                max = Rx;
        if(Ry>max)
                max = Ry;
        
        //outfile.open("./output.txt");
        outfile << Rx << " " << Ry << " " << weight << endl;
	}
    printf("File analyze is over...\n");
    printf("max = %d.\n",max);
	infile.close();
	//outfile.close();

    /*printf("The index[] is:\n");
    for(i=0;i<NODENUM;i++)
        printf("%d ",index[i]);

    for(i=0; i<NODENUM; i++){
        V[i].router_id = i;
        V[i].counter = index[i];
    }
        
    sort(V.begin(),V.end(),compare);

    printf("The analyse result is:\n");
    outfile.open("./output.txt");
    for(i=0;i<300;i++)            
        outfile << V[i].router_id << " " << V[i].counter<< endl;*/
    outfile.close();
    return 0;
}

