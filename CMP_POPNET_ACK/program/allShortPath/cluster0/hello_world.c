#include"barrier.h"

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<math.h>
#include<tgmath.h>
#include<time.h>

const int THREAD_CNT=8;
const int M=10000;

volatile int CountLock = 0,mtx=0,mtx2=0;
long double pi=0;// pi square
bool nonused=true;
clock_t t0;

void compute()
{
	//printf("Hello from thread %d\n",MTA_getthreadID());
	int pid=MTA_getthreadID();
	long double t=0;
	int i=pid+1;
	unsigned long long i2=i*i;
	/*mutex_lock((int *)&mtx);
	printf("Thread %d started\n",pid);
	mutex_unlock((int *)&mtx);*/
	for(;i<M;i+=THREAD_CNT){
		t+=6.0/i2;
		i2+=2*THREAD_CNT*i+THREAD_CNT*THREAD_CNT;
	}
	mutex_lock((int *)&CountLock);
	pi+=t;
	printf("Thread %d finished\n",pid);
	mutex_unlock((int *)&CountLock);
}
void output()
{
	mutex_lock((int *)&mtx2);
	if(nonused){
		//printf("Pi is %Lf and the calculation costs %lf seconds\n",sqrtl(pi),(clock()-t0)/(double)CLOCKS_PER_SEC);
		printf("Pi is %Lf\n",sqrtl(pi));
		nonused=false;
	}
	mutex_unlock((int *)&mtx2);
}

int main()
{
	int p;
	barrier_init(THREAD_CNT);
	//t0=clock();
	for(p=1;p<THREAD_CNT;p++){
		int pid=MTA_fork();
		if(pid!=0)break;
		//else if(pid<0)printf("Error: failed to fork\n");
	}
	barrier_wait(MTA_getthreadID() , THREAD_CNT);
	MTA_Stats(1);
	compute();
	barrier_wait(MTA_getthreadID() , THREAD_CNT);
	output();
	MTA_Stats(0);
	while(MTA_wait() == -1);
	return 0;
}
