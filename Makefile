INCLUDE_DIR=include
HEADERS=$(wildcard $(INCLUDE_DIR)/*.h)

SRC_DIR=src
IMPL_DIRS=$(wildcard $(SRC_DIR)/*)

CXX=g++
CXXFLAGS=-I$(INCLUDE_DIR)

FILES=main.cpp
FILES+=$(foreach dir,$(IMPL_DIRS),$(wildcard $(dir)/*.cpp))

BASE_CMD=$(CXX) $(CXXFLAGS) $(FILES)

all:
	$(BASE_CMD) -o main

debug:
	$(BASE_CMD) -o main -g -DDEBUG

print:
	@echo $(HEADERS)
