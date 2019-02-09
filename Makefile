CPP=g++
COMMON_OPTS=-I. -Ifile -o main
COMMON_FILES=main.cpp file/idxfile.cpp file/imgfile.cpp
BASE_CMD=${CPP} ${COMMON_OPTS} ${COMMON_FILES}

all:
	${BASE_CMD}

debug:
	${BASE_CMD} -g -DDEBUG
