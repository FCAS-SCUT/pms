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

#define INF 20000
#define NODENUM 500
#define LOCALNUM 250


extern void MTA_printresult();

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
	for(i=LOCALNUM; i<NODENUM; i++){
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

        int i,j;
        
        if(my_id == 0){
            printf("The fast sorting is beginning...\n");	          
            QuickSortSecond(0,NODENUM-1,InputArray);
            printf("The fast sorting is over...\n");										
            fflush(stdout);
        }

	}
	MTA_Bar_Stats(1);
	barrier(MTA_getthreadID() , NPROC);
	MTA_Bar_Stats(0);
    if(my_id == 0){
        printf("The final sim_cycle is to displaying...\n");
        MTA_printresult();
        fflush(stdout);
        }
	MTA_Stats(0);
	while(1);
}
