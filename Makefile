CXX =clang++
CFLAGS =-std=c++20 -Wall -pedantic
SRCS :=$(wildcard src/*.cc)
MAMBA_ENV :=$(shell python3 _getenv.py)
MAMBA_INCLUDE :=$(MAMBA_ENV)/include
MAMBA_LIB :=$(MAMBA_ENV)/lib/
LSEARCHPATH :=#-Wl,-rpath,$(MAMBA_LIB) -L$(MAMBA_LIB)
LFLAGS =#-lopenblas
INCLUDE_DIR :=-Iinc -I$(MAMBA_INCLUDE)
DEPS :=$(wildcard inc/*.hh)
OBJS :=$(patsubst %.cc, %.o, $(SRCS))
OBJ_DIR :=bin
OBJ_LOC :=$(addprefix $(OBJ_DIR)/, $(OBJS))
EXE =$(shell basename $(shell pwd)).o

.PHONY: run clean all

all:
	make -j build

build: $(OBJ_DIR) $(OBJ_LOC)
	$(CXX) $(CFLAGS) -o $(EXE) $(OBJ_LOC)

$(OBJ_DIR)/%.o: %.cc $(DEPS)
	$(CXX) $(CFLAGS) -o $@ $(INCLUDE_DIR) $(LSEARCHPATH) $(LFLAGS) -c $<

run: all
	make clean; make; ./$(EXE)

clean:
	rm *.o *.out bin/* bin/src/* *.ppm

