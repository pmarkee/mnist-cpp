all:
	g++ -I. main.cpp idxfile.cpp imgfile.cpp -o main

debug:
	g++ -g -DDEBUG -I. main.cpp idxfile.cpp imgfile.cpp -o main
