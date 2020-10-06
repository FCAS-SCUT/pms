#define global
#include "stdio.h"
#include "barrier.h"
#include "stdint.h"

#define NPROC 16
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

#define INF 20000
#define NODENUM 500
#define LOCALNUM 250


extern int MTA_printresult();

volatile int InputArray[NODENUM];
volatile int FinalArray[NODENUM+128];

void ReadFile(char *contents, int c[NODENUM]){
    int i = 0;
    int j;
    FILE *fp;
 
    //begin to read file
    fp=fopen(contents,"r");
    if (fp == NULL)
    {
        printf("open file error\n");
    }
    while(!feof(fp))    
    {
        char str[1024];
        char *p;
        if (fgets(str,sizeof(str),fp)==NULL) break ;
            p=strtok(str," ");
            while (p)
            {
                c[i] = atoi(p);
                i++;
                p=strtok(NULL," ");
            }
                
    }
    printf("\n");
    fclose(fp);
}

int QuickSortFirst(int left,int right,int a[NODENUM]) 
{ 
	int i,j,t,temp; 
	if(left>right) return; 
	                                
	temp=a[left]; 
	i=left; 
	j=right; 
	while(i!=j) 
	{ 
		while(a[j]>=temp && i<j) j--; 
		while(a[i]<=temp && i<j) i++; 
		if(i<j) 
		{ 
			t=a[i]; 
			a[i]=a[j]; 
			a[j]=t; 
		}
	}
    a[left]=a[i]; 
	a[i]=temp; 
	return i;
}

void QuickSortSecond(int left,int right,int a[NODENUM]) 
{ 
	int i,j,t,temp; 
	if(left>right) return; 
	                                
	temp=a[left]; 
	i=left; 
	j=right; 
	while(i!=j) 
	{ 
		while(a[j]>=temp && i<j) j--; 
		while(a[i]<=temp && i<j) i++; 
		if(i<j) 
		{ 
			t=a[i]; 
			a[i]=a[j]; 
			a[j]=t; 
		}
	}
    a[left]=a[i]; 
	a[i]=temp; 
    QuickSortSecond(left,i-1,a);
    QuickSortSecond(i+1,right,a);
} 
void MergeSort(int arr[],int low,int mid, int high)
{ 
	int i=low,j=mid+1,p=0;   
	int newarr[100000];
    printf("The merge_sort is beginning...\n");
    fflush(stdout);
	while(i<=mid && j<=high){    
		if(arr[i] < arr[j])   newarr[p++] = arr[i++];     
		else      newarr[p++] = arr[j++];   
	}     
	while(i<=mid)     newarr[p++] = arr[i++];   
	while(j<=high)     newarr[p++] = arr[j++];     
	for(i=low,p=0;p<(high-low+1);i++,p++)  
		arr[i] = newarr[p];   
}

void PrintResult(int dist[NODENUM]){
    int i;
	for(i=0;i<NODENUM+2;i++){
            printf("%d ",dist[i]);
        }
        printf("\n");	
    fflush(stdout);
}
void ReceiveData(int c[NODENUM]){
	int i;
    int *receiveAddress0= (int *)cmp0_0;
	for(i=LOCALNUM+1; i<NODENUM; i++){
	    c[i] = *receiveAddress0++;
    }
}

void WriteFile(char *contents,int array[],int n)
{
	FILE *fp;
	int i=0;
    printf("Write file is beginning...\n");
    fflush(stdout);
	fp=fopen(contents,"w");
	while(i<n)
		{fprintf(fp,"%d ",array[i]);
		i++;
	}
	fclose(fp);
    printf("Write file is over...\n");
    fflush(stdout);
}

void WaitTime(int a){
	int *receiveSignal0= (int *)cmp0_5;
    *receiveSignal0 = 0;//receive signal
	while(*receiveSignal0!=a){
         MTA_acquiesce(receiveSignal0);
    }	
}

void InitialDist(int coreNum,int array[],int x[],int *p, int *q){
	int i;
	int tmp;
	int num;
	if(LOCALNUM%coreNum == 0){
		num = LOCALNUM/coreNum;
    }
	else{
		num = LOCALNUM/coreNum+1;
    }
		tmp = (num*coreNum)-LOCALNUM; 
		for(i=LOCALNUM;i<NODENUM;i++){
			array[i-LOCALNUM] = x[i];
        }
		for(i=NODENUM;i<tmp+NODENUM;i++){
			array[i-LOCALNUM] = INF;	
        }
    *p = num;
    *q = tmp;    
}

int main()
{
	unsigned ProcessId = 0;
	int my_id = 0;
    int i,j;
	int eachCoreNum=0;
    int remainder=0;
    long long int finalSimCycle;
    
    ReadFile("./input.txt",InputArray);//read file   
    for(j=0;j<NODENUM;j++){
        printf("%d ", InputArray[j]);
    }
    printf("\n");
	
	InitialDist(NPROC,FinalArray,InputArray,&eachCoreNum,&remainder);//initialazation
    for(j=0;j<LOCALNUM+remainder;j++){
        printf("%d ", FinalArray[j]);
    }
    printf("\n");

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

	MTA_Stats(1);//enter to kernal
	{
		my_id = MTA_getthreadID();
		printf("come on after fork %d.\n",my_id);
        fflush(stdout);

        int i,j,cnt = 0;
		int *otherAddress0  =   (int *)cmp0_0;
		int *otherSignal0   =   (int *)cmp0_5;
		int *localAddress1  =   (int *)cmp1_2;
		int *localAddress2  =   (int *)cmp1_2;

        int *localSignal0   =   (int *)cmp0_1;
        *localSignal0 = 999;//send signal

        printf("The first sorting is beginning...\n");						
        QuickSortSecond(eachCoreNum*my_id,eachCoreNum*(my_id+1)-1,FinalArray);
		MTA_Bar_Stats(1);
		barrier(MTA_getthreadID() , NPROC);
		MTA_Bar_Stats(0);
		
		if(my_id < NPROC/2){
			MergeSort(FinalArray,my_id*(2*eachCoreNum),my_id*(2*eachCoreNum)+((((my_id+1)*2*eachCoreNum) -1)-my_id*(2*eachCoreNum))/2,((my_id+1)*(2*eachCoreNum)) -1);
            printf("This is the 2th loop");
            fflush(stdout);
        }
		MTA_Bar_Stats(1);
		barrier(MTA_getthreadID() , NPROC);
		MTA_Bar_Stats(0);

		if(my_id < NPROC/4){   
			MergeSort(FinalArray,my_id*(4*eachCoreNum),my_id*(4*eachCoreNum)+((((my_id+1)*4*eachCoreNum) -1)-my_id*(4*eachCoreNum))/2,((my_id+1)*(4*eachCoreNum)) -1);
            printf("This is the 4th loop");
            fflush(stdout);
        }		
		MTA_Bar_Stats(1);
		barrier(MTA_getthreadID() , NPROC);
		MTA_Bar_Stats(0);
		
        if(my_id < NPROC/8){ 
			MergeSort(FinalArray,my_id*(8*eachCoreNum),my_id*(8*eachCoreNum)+((((my_id+1)*8*eachCoreNum) -1)-my_id*(8*eachCoreNum))/2,((my_id+1)*(8*eachCoreNum)) -1);
            printf("This is the 8th loop");
            fflush(stdout);
        }
		MTA_Bar_Stats(1);
		barrier(MTA_getthreadID() , NPROC);
		MTA_Bar_Stats(0);

		if(my_id < NPROC/16){ 
			MergeSort(FinalArray,my_id*(16*eachCoreNum),my_id*(16*eachCoreNum)+((((my_id+1)*16*eachCoreNum) -1)-my_id*(16*eachCoreNum))/2,((my_id+1)*(16*eachCoreNum)) -1);
            printf("This is the 16th loop");
            fflush(stdout);
        }
		MTA_Bar_Stats(1);
		barrier(MTA_getthreadID() , NPROC);
		MTA_Bar_Stats(0);

		if(my_id < NPROC/32){ 
			MergeSort(FinalArray,my_id*(32*eachCoreNum),my_id*(32*eachCoreNum)+((((my_id+1)*32*eachCoreNum) -1)-my_id*(32*eachCoreNum))/2,((my_id+1)*(32*eachCoreNum)) -1);
            printf("This is the 32th loop");
        }
		MTA_Bar_Stats(1);
		barrier(MTA_getthreadID() , NPROC);
		MTA_Bar_Stats(0);

		if(my_id < NPROC/64){ 
			MergeSort(FinalArray,my_id*(64*eachCoreNum),my_id*(64*eachCoreNum)+((((my_id+1)*64*eachCoreNum) -1)-my_id*(64*eachCoreNum))/2,((my_id+1)*(64*eachCoreNum)) -1);
            printf("This is the 64th loop");
            fflush(stdout);
        }
		MTA_Bar_Stats(1);
		barrier(MTA_getthreadID() , NPROC);
		MTA_Bar_Stats(0);

        if(my_id == 0){
            int *localAddress0 = (int *)cmp1_2;
            for(i=0;i<LOCALNUM;i++)
                *(localAddress0++) = FinalArray[i];
            printf("Data from cluster0 send to cluster1.\n");
			MTA_mailboxsend(1,localAddress2,0,otherAddress0,LOCALNUM,1);//send the data    
			MTA_mailboxsend(1,localSignal0,0,otherSignal0,1,1);//send signal
			printf("Cluster0 sending is over!\n"); 	
        }

		MTA_Bar_Stats(1);
		barrier(MTA_getthreadID() , NPROC);
		MTA_Bar_Stats(0);
	}
    if(my_id == 0){
            printf("Print the final sim_cycle\n");
            finalSimCycle = MTA_printresult();
            printf("The final sim-cycle is:%llu\n",finalSimCycle);
            fflush(stdout);
    }

	MTA_Bar_Stats(1);
	barrier(MTA_getthreadID() , NPROC);
	MTA_Bar_Stats(0);
	MTA_Stats(0);
	while(1);
}
