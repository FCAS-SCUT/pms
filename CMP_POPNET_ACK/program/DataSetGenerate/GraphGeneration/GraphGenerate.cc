#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <time.h>  
#include <algorithm> 
#include <vector>

using namespace std;

struct vertex{
	int src;
	int dst;
	int weight;	
};

int Range(int m, int n)
{
        int pos, dis;
        if(m == n)
        {
            return m;
        }
        else if(m > n)
        {
            pos = n;
            dis = m - n + 1;
            return rand() % dis + pos;
        }
        else
        {
            pos = m;
            dis = n - m + 1;
            return rand() % dis + pos;
        }
}

vector< vertex> V;

bool compare(const vertex &a,const vertex &b)
{
	return a.src < b.src;
}

int main(int argc, char *argv[])
{
	int i,j,k;
	srand((unsigned)time(NULL));  
	
    ofstream Fgraphweight;
    Fgraphweight.open("./input.txt");
    V.resize(100);
		
	for(i=0; i<100; i++){
		V[i].src = rand()%32;
		V[i].dst = rand()%32;
		V[i].weight = Range(1,9);
	}
		
	sort(V.begin(),V.end(),compare);
			
	//Fgraphweight<<"Source"<<" "<<"Target"<<" "<<"Weight"<<endl;
	
    for(j=0; j<100; j++)
		Fgraphweight << V[j].src << " " << V[j].dst << " " << V[j].weight <<endl;
		
	Fgraphweight.close();
		
	cout<<"The data is generated over!"<<endl;

	return 0; 
}
