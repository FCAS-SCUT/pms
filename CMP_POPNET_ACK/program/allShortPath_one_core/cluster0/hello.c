#define global
#include "stdio.h"
#include "barrier.h"
#define NPROC 2
#define NOLINK 9999
#define NODENUM 128
#define INITIALNODE 127

volatile int CountLock = 0;
volatile int C[NODENUM][NODENUM];

void Dijkstra(int n,int v,int *dist, int *prev, int c[NODENUM][NODENUM])
{
	int s[NODENUM];
	int i =0;
	for(i;i<n;++i){
		dist[i] = c[v][i];
		s[i] = 0;
		if(dist[i]==NOLINK)
			prev[i] = 0;
		else
			prev[i]	= v;
	}
	dist[v] = 0;
	s[v] = 1;
	i = 1;
	for(i;i<n;++i){
		int tmp = NOLINK;
		int u = v;
		int j = 0;
		for(j;j<n;++j){
			if((!s[j])&& dist[j]<tmp){
				u = j;
				tmp = dist[j];
			}
		}
		s[u] = 1;
		j = 0;
		for(j;j<n;++j){
			if((!s[j])&& c[u][j]<NOLINK){
				int newdist = dist[u] + c[u][j];
				if(newdist < dist[j]){
					dist[j] = newdist;
					prev[j] = u;
				}
			}
		}
	}
}
void searchPath(int *prev,int v, int u)
{
	int que[NODENUM];
    	int tot = 1;
    	que[tot] = u;
    	tot++;
    	int tmp = prev[u];
    	while(tmp != v){
        	que[tot] = tmp;
        	tot++;
        	tmp = prev[tmp];
    	}
    	que[tot] = v;
	int i = tot;
    	for(i; i>=1; --i)
        	if(i != 1)
            		printf(" %d->",que[i]);
        	else
            		printf(" %d\n",que[i]);
}

void ReadFile(char *contents, int c[NODENUM][NODENUM]){
    int i = 0;
    int j;
    int a[3];
    FILE *fp;

    //initialization
    for(i=0; i<NODENUM; i++){
        for(j=0; j<NODENUM; j++){
            if(i==j) c[i][j] = 0;
            else  c[i][j] = NOLINK;
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
	for(i=0; i<NODENUM; i++){
        for(j=0; j<NODENUM; j++){
            if(i==j) c[i][j] = 0;
        }
    }
}

int main()
{
	unsigned ProcessId = 0;
	int i,j;
	int my_id = 0;
	
	ReadFile("./input.txt",C);//read file
    for(i=0;i<NODENUM;i++){
            for(j=0;j<NODENUM;j++){
                   printf("%d ", C[i][j]);
            }
            printf("\n");
    }
	
	barrier_init(NPROC);
	for(ProcessId = 1;ProcessId < NPROC; ProcessId++){
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
		MTA_Lock_Stats(1);
		MTA_Lock_Acquire(&CountLock);
		MTA_Lock_Stats(0);

		my_id = MTA_getthreadID();
		
		int dist[NODENUM];     
    	int prev[NODENUM]; 
		
		for(i=0;i<NODENUM;i++)
			dist[i] = NOLINK;
		
		if(my_id == 1){
			Dijkstra(NODENUM,INITIALNODE,dist,prev,C);
			int i = 0;
			for(i;i<NODENUM;i++){
				//printf("The distance from %d to %d is %d\n",my_id,i,dist[i]);
				//printf("The shortest path from %d to %d is :\n",my_id,i,dist[i]);
				//searchPath(prev,my_id,i);
				printf("The dist from node_%d is:\n",my_id*5);
				for(i=0; i<NODENUM; i++)
					printf("%d  ",dist[i]);
			}
            fflush(stdout);
			printf("\n");
		}
		MTA_Lock_Stats(1);
		MTA_Lock_Release(&CountLock);
		MTA_Lock_Stats(0);

        if(my_id == 0){
            printf("The simulation is over...\n");
            MTA_printresult();
        }
	}

	MTA_Bar_Stats(1);
	barrier(MTA_getthreadID() , NPROC);
	MTA_Bar_Stats(0);
	MTA_Stats(0);
	 while(MTA_wait() == -1);

}

	
	
