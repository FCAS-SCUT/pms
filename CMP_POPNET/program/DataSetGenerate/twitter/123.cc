#include <exception>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <assert.h>
#include <string.h>
#include <vector>
#include <algorithm>

#define NODENUM 3656

using namespace std;

typedef struct info{
    int s;
    int d;
    long  weight;
} infomation;

vector <infomation> V;

bool compare(const infomation &a,const infomation &b)
{
	return a.d > b.d;
}

int main()
{
    V.resize(NODENUM);
    int Rx,Ry,weight;
    int i;

    ifstream infile;
    ofstream outfile;

    infile.open("./twittercrawlweight_int.txt"); 
    //infile.open("./output.txt"); 
    //outfile.open("./output.txt");
    
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
        V[i].s = Rx;
        V[i].d = Ry;
        V[i].weight = weight;
	}
    printf("File analyze is over...\n");
    //printf("max = %d.\n",max);
	infile.close();
	outfile.close();

    /*printf("The index[] is:\n");
    for(i=0;i<NODENUM;i++)
        printf("%d ",index[i]);

    for(i=0; i<NODENUM; i++){
        V[i].router_id = i;
        V[i].counter = index[i];
    }
    */    
    sort(V.begin(),V.end(),compare);

    printf("The analyse result is:\n");
    outfile.open("./output.txt");
    for(i=0;i<1280;i++)            
        outfile << V[i].s << " " << V[i].d << " " << weight<< endl;
    outfile.close();
    return 0;
}

