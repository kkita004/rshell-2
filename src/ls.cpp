#include <iostream> 
#include <dirent.h>
#include <sys/types.h>
#include <map>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

using namespace std;

void print();
void print(char *file);
void printAll(char *file);
void printL();
void printR();

int main(int argc, char **argv){
	// map declaration and construction
	// uses function pointers for each flag
	map<string, void(*)()> flags;
	//flags["--all"] = printAll;
	//flags["-a"] = printAll;
    flags["--long"] = printL;
	flags["-l"] = printL;
	flags["--recursive"] = printR;
	flags["-R"] = printR;

	// sets funct to function pointed by flag argv[i]
	// if funct == NULL then no recognized flag was input
	for(int i = 1; i < argc; i++){
		void(*funct)() = flags[string(argv[i])];
		if(funct != NULL)
			funct();
		else
			cerr << "invalid flag: " << argv[i] << endl;
	}

	pid_t pid = fork();
	if(pid == -1){
		perror("Fork failed.");
	}
	else if(pid == 0){
		printAll(argv[1]);
	}
	else{
		wait(0);
	}
	return 0;
}

void print(char *file){
	auto dir = opendir(file);
	if(dir == NULL){
		perror("opendir failed.");
		exit(1);
	}
	while(auto name = readdir(dir)){
		if((*name->d_name != '.')){
			string test = name->d_name;
			cout << test << "  ";
		}	
	}
	cout << endl;
	if(-1 == closedir(dir)){
		perror("closedir failed.");
		exit(1);
	}
}

void printAll(char *file){
	auto dir = opendir(file);
	if(dir == NULL){
		perror("opendir failed.");
		exit(1);
	}
	// readdir needs error checking
	while(auto dirinfo = (readdir(dir))){
		cout << dirinfo->d_name << " ";
	}
	cout << endl;
	if(-1 == closedir(dir)){
		perror("closedir failed.");
		exit(1);
	}
}	

void printL(){

}

void printR(char *file){

}
