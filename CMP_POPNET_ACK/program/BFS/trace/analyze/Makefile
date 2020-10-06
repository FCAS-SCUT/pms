OBJS = analyze.o
CC = g++ 

analyze: $(OBJS)
	$(CC)  -O0 -g3 -w -static  $(OBJS) -o analyze -lm
analyze.o: analyze.cc 
	$(CC) -O0 -g3 -w -static  -c analyze.cc

clean:
	rm -f *.o analyze

