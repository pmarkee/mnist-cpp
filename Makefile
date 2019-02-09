CPP=g++
COMMON_OPTS=-I. -Ifile -Imathutils -o main
COMMON_FILES=main.cpp \
			 file/idxfile.cpp \
			 file/imgfile.cpp \
			 mathutils/matrix.cpp \
			 mathutils/vector.cpp \
			 mathutils/math_exception.cpp

BASE_CMD=${CPP} ${COMMON_OPTS} ${COMMON_FILES}

all:
	${BASE_CMD}

debug:
	${BASE_CMD} -g -DDEBUG
