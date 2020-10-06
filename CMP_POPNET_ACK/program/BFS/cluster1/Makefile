OBJS = hello.o  func.o
CC = alpha-unknown-linux-gnu-gcc 

hello: $(OBJS)
	$(CC)  -O3 -g3 -w -static  $(OBJS) -o hello -lm

func.o: func.s
	$(CC) -O3 -g3 -w -static  -c func.s
hello.o: hello.c 
	$(CC) -O3 -g3 -w -static  -c hello.c

clean:
	rm -f *.o hello 

