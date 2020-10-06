#include"barrier.h"

#include<cstdio>
#include<iostream>

using namespace std;

const int NPROC=8;

volatile int CountLock = 0;

void compute()
{
	mutex_lock((int *)&CountLock);
	cout<<"Hello from thread "<<MTA_getthreadID()<<endl;
	//printf("Hello from thread %d\n",MTA_getthreadID());
	//fflush(stdout);
	mutex_unlock((int *)&CountLock);
}

int main()
{
	//cout<<"Hello world"<<endl;
	barrier_init(NPROC);
	for(int p=1;p<NPROC;p++){
		int pid=MTA_fork();
		if(pid!=0)break;
		//else if(pid<0)cerr<<"Error: failed to fork\n";
	}
	barrier_wait(MTA_getthreadID() , NPROC);
	MTA_Stats(1);
	compute();
	barrier_wait(MTA_getthreadID() , NPROC);
	MTA_Stats(0);
	while(MTA_wait() == -1);
	return 0;
}
