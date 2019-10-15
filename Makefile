INCLUDE_DIR=include
HEADERS=$(wildcard $(INCLUDE_DIR)/*.h)

SRC_DIR=src
IMPL_DIRS=$(wildcard $(SRC_DIR)/*)

CXX=g++
CXXFLAGS=-I$(INCLUDE_DIR) -std=c++11 -lm -g -DDEBUG -Wno-format

TRAINER_SOURCE=src/train.cpp
EVALUATE_SOURCE=src/evaluate.cpp
COMMON_FILES=$(foreach dir,$(IMPL_DIRS),$(wildcard $(dir)/*.cpp))

all: train evaluate

train:
	$(CXX) $(CXXFLAGS) $(TRAINER_SOURCE) $(COMMON_FILES) -o train

evaluate:
	$(CXX) $(CXXFLAGS) $(EVALUATE_SOURCE) $(COMMON_FILES) -o evaluate
