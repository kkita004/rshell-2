VPATH = src
FLAGS = -pedantic -Wall -Werror
STD=-std=c++11

all: | bin 
	g++ $(STD) $(FLAGS) src/main.cpp -o bin/rshell

rshell: src/main.cpp | bin 
	g++ $(STD) $(FLAGS) src/main.cpp -o bin/rshell

bin: 
	mkdir bin

clean: 
	rm -rf bin 
