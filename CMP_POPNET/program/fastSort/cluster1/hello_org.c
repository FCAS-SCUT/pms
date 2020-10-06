#define global
#include "stdio.h"
#include "barrier.h"
#include "stdint.h"

#define NPROC 2
volatile int CountLock = 0;

#define cmp0_0 0x1ff000000
#define cmp0_1 0x1ff000100
#define cmp0_2 0x1ff000200
#define cmp0_3 0x1ff000300
#define cmp0_4 0x1ff000400
#define cmp0_5 0x1ff000800
#define cmp0_6 0x11fde6f50

#define cmp1_0 0x1ff001000
#define cmp1_1 0x1ff002000
#define cmp1_2 0x1ff003000
#define cmp1_3 0x1ff004000
#define cmp1_4 0x1ff005000
#define cmp1_5 0x1ff006000
#define cmp1_6 0x1ff007000
#define cmp1_7 0x1ff008000

#define INF 9999
#define NODENUM 6
#define LOCALNUM 2
#define INITIALNUM 5

volatile int Dist[NODENUM];
volatile int x[NODENUM][NODENUM];

void ReadFile(char *contents, int c[NODENUM][NODENUM]){
    int i = 0;
    int j;
    int a[3];
    FILE *fp;

    //initialization
    for(i=0; i<NODENUM; i++){
        for(j=0; j<NODENUM; j++){
            if(i==j) c[i][j] = 0;
            else  c[i][j] = INF;
        }
    }
    
    //begin to read file
    fp=fopen(contents,"r");
    if (fp == NULL)
    {
        printf("open file error\n");
    }
    while(!feof(fp))    
    {
        char str[1024];
        memset(a,0,3);
        char *p;
        if (fgets(str,sizeof(str),fp)==NULL) break ;
            p=strtok(str," ");
            while (p)
            {
                //printf("%d\t",atoi(p));
                a[i] = atoi(p);
                //printf("%d ",a[i]);
                i++;
                p=strtok(NULL," ");
            }
                for(i=0;i<3;i++)
                    printf("%d ",a[i]);
                    printf("\n");

                if(i == 3) i=0;

        c[a[0]][a[1]]=a[2];
        c[a[1]][a[0]]=a[2];
                //printf("%d ",c[a[0]][a[1]]);
    }
    printf("\n");
    fclose(fp);
}

void CulculateMin(int node, int dist[NODENUM], int c[NODENUM][NODENUM]){
	int *localAddress0 = (int *)cmp1_2;
    int tmp[NODENUM];
    int i;
    for(i=0;i<NODENUM;i++)//initial the tmp[]
        tmp[i] = INF;
    for(i=0; i<NODENUM; i++){
         if(c[node][i]+dist[i] < tmp[node-(NODENUM-LOCALNUM)]){
		    tmp[node-(NODENUM-LOCALNUM)]= c[node][i] + dist[i];
        }
    }
    /*if(node == 4)
            printf("The tmp is:%d\n",tmp[0]);
    fflush("stdout");*/
	*(localAddress0+(node-(NODENUM-LOCALNUM))) = tmp[node-(NODENUM-LOCALNUM)];
}

void PrintResult(int dist[NODENUM]){
    int i;
	for(i=0;i<NODENUM;i++){
            printf("%d\t",dist[i]);
        }
        printf("\n");	
    fflush(stdout);
}
void ReceiveData(int dist[NODENUM]){
	int i;
    int *receiveAddress0= (int *)cmp1_0;
    printf("The ReceiveData is:\n");
	for(i=0; i<NODENUM-LOCALNUM; i++){
	    dist[i] = *(receiveAddress0++);
        //printf("%d\t",*receiveAddress0);
        printf("%d\t",dist[i]);
    }
    printf("\n");

}

void Wait(int a){
	int *receiveSignal0= (int *)cmp1_1;
    *receiveSignal0 = 0;//receive signal
	while(*receiveSignal0!=a){
         MTA_acquiesce(receiveSignal0);
    }	
}


void InitialDist(int dist[NODENUM],int destNo){
    int i;
    for(i=0; i<destNo; i++){
        dist[i] = INF;
    }
    dist[destNo] = 0;
}

int main()
{
	unsigned ProcessId = 0;
	int my_id = 0;
    int i,j,cnt = 0;

    ReadFile("./array.txt",x);//read file
    for(i=0;i<NODENUM;i++){
            for(j=0;j<NODENUM;j++){
                   printf("%d ", x[i][j]);
            }
            printf("\n");
    }

    //iialization of Dist[] is:\n");
    InitialDist(Dist,INITIALNUM);
   
    printf("The initial Dist is:\n");
    for(i=0; i<NODENUM;i++)
    {
        printf("%d\t",Dist[i]);
    } 
	barrier_init(NPROC);
	for(ProcessId = 1;ProcessId <= NPROC; ProcessId++)
    {
		my_id = MTA_fork();
		printf("loop %d after fork %d\n",ProcessId,my_id);
		my_id = MTA_getthreadID();
		printf("loop %d after MTA_getthreadID %d\n",ProcessId,my_id);
		if(my_id != 0)		break;
	}    
	MTA_Bar_Stats(1);
	barrier(MTA_getthreadID() , NPROC);
	MTA_Bar_Stats(0);	

	MTA_Stats(1);
	{
		my_id = MTA_getthreadID();
		printf("come on after fork %d.\n",my_id);

		int i,j,k;
		int *otherAddress0  =   (int *)cmp0_0;
		int *otherSignal0   =   (int *)cmp0_5;
		int *localAddress1  =   (int *)cmp1_2;
		int *localAddress2  =   (int *)cmp1_2;
		
		int *localSignal0= (int *)cmp1_4;
		*localSignal0 = 998;//send signal

		for(k=0; k<NODENUM; k++){

			printf("This is the %dth loop.\n",k);
            
			MTA_Bar_Stats(1);
			barrier(MTA_getthreadID() , NPROC);
			MTA_Bar_Stats(0);
            /*if(my_id == 0)
                   printf("********The dist is:**********\n") ;
            for (i=0;i<6;i++)
                   printf("%d\t",Dist[i]);
            fflush(stdout);*/
			CulculateMin(my_id+(NODENUM-LOCALNUM), Dist, x);//culculate the minimize
			
			MTA_Bar_Stats(1);
			barrier(MTA_getthreadID() , NPROC);
			MTA_Bar_Stats(0);
			
			Dist[my_id+(NODENUM-LOCALNUM)] = *(localAddress1+my_id);
	        
			MTA_Bar_Stats(1);
			barrier(MTA_getthreadID() , NPROC);
			MTA_Bar_Stats(0);

			if(my_id == 1){
				
				printf("Data from cluster0 send to cluster1.\n");
				MTA_mailboxsend(1,localAddress2,0,otherAddress0,LOCALNUM,1);//send the data    
				MTA_mailboxsend(1,localSignal0,0,otherSignal0,1,1);//send signal
				printf("Cluster0 sending is over!\n"); 	

				Wait(999);//wait
				printf("I received the signal.\n");

				ReceiveData(Dist);//receive the other Dist[]			
				printf("I received the data.\n");
            }

            MTA_Bar_Stats(0);
            barrier(MTA_getthreadID() , NPROC);
            MTA_Bar_Stats(0);

        }
        if(my_id == 0){
            printf("The result is:\n");
            PrintResult(Dist);
        }
        else{
            for(i=0;i<100;i++){
                for(j=0;j<100;j++){
                    cnt++;
                    if(cnt == 1000)
                        cnt = 0;
                }
            }
        }
	}
	MTA_Bar_Stats(1);
	barrier(MTA_getthreadID() , NPROC);
	MTA_Bar_Stats(0);
	MTA_Stats(0);
	while(1);
}

	

	
