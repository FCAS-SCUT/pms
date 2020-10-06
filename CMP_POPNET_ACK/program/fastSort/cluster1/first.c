#define global
#include "stdio.h"
#include "barrier.h"
#include "stdint.h"
#define NPROC 2
volatile int CountLock = 0;
//#define mailbox 0xff0000000
#define mailbox 0x1ff000000

int main()
{
	unsigned ProcessId = 0;
	int my_id = 0;
	
	barrier_init(NPROC);
	for(ProcessId = 1;ProcessId <= NPROC; ProcessId++){
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
		printf("come on atfer fork %d\n",my_id);
		//MTA_hello();
		switch (my_id )
		{
		case 0:
			{	
			   int a=1;
               while(a!=0)
               {
               a++;               
                 if(a>1000)
                {
                   a=1;
                }
              }            
		    }break;
		}
		fflush (stdout);
		while(1);
	}

	MTA_Bar_Stats(1);
	barrier(MTA_getthreadID() , NPROC);
	MTA_Bar_Stats(0);
	MTA_Stats(0);
	 while(MTA_wait() == -1);

}

	
	
