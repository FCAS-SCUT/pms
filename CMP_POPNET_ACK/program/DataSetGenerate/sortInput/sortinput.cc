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
		
	for(i=0; i<500; i++)
		Fgraphweight << rand()%500 <<endl;
	Fgraphweight.close();
		
	cout<<"The data is generated over!"<<endl;

	return 0; 
}

