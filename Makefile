VPATH = src
FLAGS = -pedantic -Wall -Werror
STD=-std=c++11

all: | bin 
	g++ $(STD) $(FLAGS) src/main.cpp -o bin/rshell
	g++ $(STD) $(FLAGS) src/cp.cpp -o bin/cp

rshell: src/main.cpp | bin 
	g++ $(STD) $(FLAGS) src/main.cpp -o bin/rshell

cp: src/cp.cpp | bin
	g++ $(STD) $(FLAGS) src/cp.cpp -o bin/cp

bin: 
	mkdir bin

clean: 
	rm -rf bin 
