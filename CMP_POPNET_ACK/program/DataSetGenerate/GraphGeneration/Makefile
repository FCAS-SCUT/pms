OBJS = GraphGenerate.o
CC = g++ 

GraphGenerate: $(OBJS)
	$(CC)  -O3 -g3 -w -static  $(OBJS) -o GraphGenerate -lm
GraphGenerate.o: GraphGenerate.cc 
	$(CC) -O3 -g3 -w -static  -c GraphGenerate.cc

clean:
	rm -f *.o GraphGenerate

