OBJS = BFS.o
CC = g++ 

BFS: $(OBJS)
	$(CC)  -O3 -g3 -w -static  $(OBJS) -o BFS -lm
BFS.o: BFS.cc 
	$(CC) -O3 -g3 -w -static  -c BFS.cc

clean:
	rm -f *.o BFS

