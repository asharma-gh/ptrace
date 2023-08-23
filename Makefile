CXX =clang++
CFLAGS =-std=c++20 -Wall -pedantic
SRCS :=$(wildcard *.cc)
MAMBA_ENV :=$(shell python3 _getenv.py)
MAMBA_INCLUDE :=$(MAMBA_ENV)/include
MAMBA_LIB :=$(MAMBA_ENV)/lib/
LSEARCHPATH :=-Wl,-rpath,$(MAMBA_LIB) -L$(MAMBA_LIB)
LFLAGS =-lopenblas
INCLUDE_DIR :=-Iinc -I$(MAMBA_INCLUDE)
DEPS :=$(wildcard inc/*.hh)
OBJS :=$(patsubst %.cc, %.o, $(SRCS))
OBJ_DIR :=bin
OBJ_LOC :=$(addprefix $(OBJ_DIR)/, $(OBJS))
EXE =$(shell basename $(shell pwd)).o

.PHONY: run clean $(OBJS)

all: $(OBJS)
	$(CXX) $(CFLAGS) -o $(EXE) $(INCLUDE_DIR) $(LSEARCHPATH) $(OBJ_LOC) $(LFLAGS)

$(OBJ_DIR)/$(OBJS): $(SRCS)
	mkdir -p $(OBJ_DIR)
	$(CXX) $(CFLAGS) -o $(OBJ_DIR)/$@ $(INCLUDE_DIR) -c $<


run:
	make clean; make; ./$(EXE)

clean:
	rm *.o *.out bin/* *.ppm

