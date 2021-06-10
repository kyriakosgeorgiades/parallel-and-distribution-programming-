CC 	= g++ -std=c++11
CFLAGS 	= -Wall -O3 -fopenmp
LDFLAGS = -lgomp

#----------

EXE = run

OBJS = $(patsubst %.cpp,%.o,$(wildcard *.cpp))

#----------

$(EXE): $(OBJS)
	$(CC) $(OBJS) $(LIB_DIRS) $(LLIBS)$(LDFLAGS) -o $(EXE)

$(OBJS): %.o : %.cpp
	$(CC) $(CFLAGS) -c $<
#----------

clean:
	rm -f *.o *.*~ *~  $(EXE) 
 
 