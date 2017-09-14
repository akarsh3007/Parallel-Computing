CFLAGS=-O3 -std=c11
CXXFLAGS=-O3 -std=c++11
LDFLAGS=
ARCHIVES=libfunctions.a
LD=g++

main: main.o
	$(LD) $(LDFLAGS) main.o $(ARCHIVES) -o main

libfunctions.a: functions.o
	ar rcs libfunctions.a functions.o

test: main approx
	./test.sh

bench: main 
	./bench.sh

assignment.tgz: main.cpp approx.cpp Makefile bench.sh test.sh assignment.pdf libfunctions.a
	tar zcvf assignment.tgz main.cpp approx.cpp Makefile bench.sh test.sh assignment.pdf libfunctions.a
