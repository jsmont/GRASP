IDIR =./include
CC=g++
CFLAGS=-I$(IDIR) -O3 -march=native -std=c++11 #-g
DEFINES=
INSTANCE=main

SDIR=./src

_DEPS = Grasp.h Solution.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = Grasp.o Solution.o 
OBJ = $(patsubst %,$(SDIR)/%,$(_OBJ))


%.o:%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(DEFINES)

all: $(INSTANCE) 

#parallel: CFLAGS += -fopenmp
#parallel: $(OBJ)
#	$(CC) -o $@ $^ $(CFLAGS) $(DEFINES)

#dummy: INSTANCE = main
#dummy: src/main.o
#dummy: main

#tasksc: INSTANCE = tasks_to_computer
#tasksc: src/tasks_to_computer.o
#tasksc: main

#nurses: INSTANCE = nurses
#nurses: CFLAGS += -fopenmp
#nurses: src/nurses.o
#nurses: main

main: $(OBJ) src/$(INSTANCE).o
	$(CC) -o $(INSTANCE) $^ $(CFLAGS) $(DEFINES)

#generator: src/instance_generator.cpp
#	$(CC) -o $@ $<

.PHONY: clean

clean:
	rm -f $(SDIR)/*.o
