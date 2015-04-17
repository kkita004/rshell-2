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
	vector<string> list;
	vector<string> clist;
	while(1){	 //continue prompting until user exits
		cout << "$ "; // prompt		
		getline(cin,commands);

		//tokenize each command from user input
		char_separator<char> delim(" ");
		tokenizer< char_separator<char> > mytok(commands,delim);

		for(auto command = mytok.begin(); command != mytok.end() ; ++command) {
			list.push_back(*command);	
		}
		for(auto i = 0; i < list.size(); i++){
			cout << list.at(i) << endl;
		}
		int i = fork();
		if(i == -1){
			perror("fork");
		}
		else if(i == 0){	
			if(-1 == execvp(list.at(0).c_str(),argv)); 
				perror(list.at(0).c_str());		
		}
		else{
			wait(0);
		}
	}
	return 0;
}
