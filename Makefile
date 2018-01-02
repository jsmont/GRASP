IDIR =./include
CC=g++
CFLAGS=-I$(IDIR) -O3 -march=native #-g
DEFINES=
INSTANCE=

SDIR=./src

_DEPS = Grasp.h Solution.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = Grasp.o Solution.o
OBJ = $(patsubst %,$(SDIR)/%,$(_OBJ))


%.o:%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(DEFINES)

all: main 

parallel: CFLAGS += -fopenmp
parallel: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(DEFINES)

dummy: INSTANCE = main
dummy: src/main.o
dummy: main

#tasksc: INSTANCE = tasks_to_computer
#tasksc: src/tasks_to_computer.o
#tasksc: main

#nurses: INSTANCE = nurses
#nurses: CFLAGS += -fopenmp
#nurses: src/nurses.o
#nurses: main

main: $(OBJ) 
	$(CC) -o $(INSTANCE) src/$(INSTANCE).o $^ $(CFLAGS) $(DEFINES)

#generator: src/instance_generator.cpp
#	$(CC) -o $@ $<

.PHONY: clean

clean:
	rm -f $(SDIR)/*.o
