OBJS = sortinput.o
CC = g++ 

sortinput: $(OBJS)
	$(CC)  -O3 -g3 -w -static  $(OBJS) -o sortinput -lm
sortinput.o: sortinput.cc 
	$(CC) -O3 -g3 -w -static  -c sortinput.cc

clean:
	rm -f *.o sortinput

