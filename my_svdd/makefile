
CC=g++

SRC_DIRS=./src
SRCS=$(shell find $(SRC_DIRS) -name *.cpp -or -name *.cc)
INC_DIRS=$(shell find $(SRC_DIRS) -type d)
INC_FLAGS=$(addprefix -I,$(INC_DIRS))

CFLAGS=-pthread -g $(INC_FLAGS)

my_svdd : $(SRCS)
	       $(CC) $(CFLAGS) -o my_svdd $(SRCS) -std=c++17 -lstdc++fs
