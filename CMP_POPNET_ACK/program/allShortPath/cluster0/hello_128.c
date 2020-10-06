#define global
#include "stdio.h"
#include "barrier.h"
#include "stdint.h"

#define NPROC 64
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
#define NODENUM 128
#define LOCALNUM 64
#define INITIALNUM 127

volatile int Dist[NODENUM];
volatile int DistTmp[NODENUM];
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
                a[i] = atoi(p);
                i++;
                p=strtok(NULL," ");
            }
                for(i=0;i<3;i++)
                    printf("%d ",a[i]);
                    printf("\n");
                if(i == 3) i=0;
        c[a[0]][a[1]]=a[2];
        c[a[1]][a[0]]=a[2];
    }
    printf("\n");
    fclose(fp);
	for(i=0; i<NODENUM; i++){
        for(j=0; j<NODENUM; j++){
            if(i==j) c[i][j] = 0;
        }
    }
}

void CulculateMin(int node, int dist[NODENUM], int c[NODENUM][NODENUM]){
	int *localAddress0 = (int *)cmp0_2;
    int tmp[LOCALNUM];
    int i;
    for(i=0;i<LOCALNUM;i++)//initial the tmp[]
        tmp[i] = INF;
    for(i=0; i<NODENUM; i++){
         if(c[node][i]+dist[i] < tmp[node]){
		    tmp[node]= c[node][i] + dist[i];
        }
    }
    *(localAddress0+node) = tmp[node];
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
    int *receiveAddress0= (int *)cmp0_0;
	for(i=LOCALNUM; i<NODENUM; i++){
	    dist[i] = *receiveAddress0++;
    }

}

void Wait(){
	int *receiveSignal0= (int *)cmp0_5;
    printf("entering MTA_acquiesce.......... \n");
    fflush(stdout);
    MTA_printresult();
	while(0xaa != *receiveSignal0){
         MTA_acquiesce(receiveSignal0);
    }	
    *receiveSignal0 = 0x55;
    printf("leaving MTA_acquiesce..........\n");
    fflush(stdout);
    MTA_printresult();
}


void InitialDist(int dist[NODENUM],int destNo){
    int i;
    for(i=0; i<NODENUM; i++){
        dist[i] = INF;
    }
    dist[destNo] = 0;
}

void WriteFile(char *contents,int array[],int n)
{
	FILE *fp;
	int i=0;
    printf("Write file is beginning...\n");
    fflush(stdout);
	fp=fopen(contents,"w");
	while(i<n)
	{
        fprintf(fp,"%d ",array[i]);
		i++;
	}
	fclose(fp);
    printf("Write file is over...\n");
    fflush(stdout);
}

void CopyArray(int a[NODENUM],int b[NODENUM]){
    int i;
    for(i=0;i<NODENUM;i++){
        a[i]=b[i];
    }
}

int main()
{
	unsigned ProcessId = 0;
	int my_id = 0;
    int i,j,cnt=0;

    ReadFile("./input.txt",x);//read file
    printf("The orignal x[][] is:\n");
    for(i=0;i<64;i++){
            for(j=0;j<NODENUM;j++){
                   printf("%d ", x[i][j]);
            }
            printf("\n");
    }

    InitialDist(Dist,INITIALNUM);//initialization 
   
    printf("The orignal dist[] is:\n");
    for(i=0; i<NODENUM;i++)
    {
        printf("%d\t",Dist[i]);
    } 
	barrier_init(NPROC);
	for(ProcessId = 1;ProcessId < NPROC; ProcessId++)
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
		//printf("come on after fork %d.\n",my_id);
        //fflush(stdout);

        int i,j,k;
		int *otherAddress0  =   (int *)cmp1_0;
		int *otherSignal0   =   (int *)cmp1_1;
		int *localAddress1  =   (int *)cmp0_2;
		int *localAddress2  =   (int *)cmp0_2;

        int *localSignal0   =   (int *)cmp0_1;

		for(k=0; k<128; k++){
            if (my_id == 3){
                printf("**********This is the %dth loop**********\n",k);
                fflush(stdout);
            }
            
			CulculateMin(my_id, Dist, x);//culculate the minimize
			
			DistTmp[my_id] = *(localAddress1+my_id);
	        
			MTA_Bar_Stats(1);
			barrier(MTA_getthreadID() , NPROC);
			MTA_Bar_Stats(0);

            if(my_id == 0){	

                CopyArray(Dist,DistTmp);        

				printf("Data from cluster0 send to cluster1.\n");
				MTA_mailboxsend(0,localAddress2,1,otherAddress0,LOCALNUM,1);//send the data    
				//MTA_mailboxsend(0,localSignal0,1,otherSignal0,1,1);//send signal
                printf("Cluster0 sending is over!\n"); 
				Wait();//wait

				ReceiveData(Dist);//receive the other Dist[]			
                printf("I received the data.\n");
                fflush(stdout);
			}			
			MTA_Bar_Stats(1);
			barrier(MTA_getthreadID() , NPROC);
			MTA_Bar_Stats(0);	

        }
        if(my_id == 0){
            printf("****************print the final cycle begin********************\n");
            fflush(stdout);
            MTA_printresult();
            printf("****************print the final cycle end********************\n");

        }
        if(my_id == 0){
            printf("The simulation result is:\n");
            PrintResult(Dist);
		    WriteFile("./output.txt",Dist,NODENUM);
        }
	}
	MTA_Bar_Stats(1);
	barrier(MTA_getthreadID() , NPROC);
	MTA_Bar_Stats(0);
	MTA_Stats(0);
	while(1);
}

	

	