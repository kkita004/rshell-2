#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h> 
#include <boost/tokenizer.hpp>

using namespace std;
using namespace boost;

int main(int argc, char **argv){
	int exit = 0;	 //Set to 1 when user inputs "exit" to console
	string commands;
	vector<string> clist;
	while(1){	 //continue prompting until user exits
		cout << "$ "; // prompt		
		getline(cin,commands);

		//tokenize each command from user input
		char_separator<char> delim(" ");
		tokenizer< char_separator<char> > mytok(commands,delim);

		for(auto command = mytok.begin(); command != mytok.end() ; ++command) {
			cout << *command << endl;	
		}
		int i = fork();
		if(i == -1){
			perror("fork");
		}
		else if(i == 0){
		//	if(-1 == execvp(command,"a"))
		//		perror(command);		
		}
		else{
		//	wait(0);
		}
	}
	return 0;
}
