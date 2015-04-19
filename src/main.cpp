#include <iostream>
#include <string.h>
#include <sstream>
#include <unistd.h>
#include <vector>
#include <stdio.h>
#include <boost/algorithm/string.hpp>
#include <sys/wait.h> 
#include <boost/tokenizer.hpp>

using namespace boost;
using namespace std;

void run(char **command, char **argv);
string userinput();
void parse(const string &input, vector<string> &commands, char *del);
void rmComments(string &s);

int main(int argc, char **argv){
	while(1){	 //continue prompting until user exits
		vector<string> commands;
		cout << "$ "; // prompt		
		string input = userinput();
		rmComments(input);
		cout << "Input after comments removed: " << input << endl;
		char delim[] = ";";
		parse(input, commands, delim);
		const unsigned int size = commands.size();
		unsigned csize = 0;
		for(size_t i = 0; i < size; i++){ // loop until every command is done || exit 
			trim(commands.at(i));
			if(commands.at(i) == "exit") 
				exit(0);
			csize = commands.at(i).size();
			char *temp = new char(csize + 1);
			strcpy(temp, commands.at(i).c_str());
			temp[csize] = '\0';
			char **command = &temp;
			run(command, argv);
			delete [] temp;
		}	
	}	
	return 0;
}

string userinput(){ // get user input 
	string input;
	getline(cin, input);
	return input;
}

void parse(const string &input, vector<string> &commands, char *del){
	char_separator<char> delim(del);
	tokenizer< char_separator<char> > mytok(input ,delim);
	for(auto command = mytok.begin(); command != mytok.end() ; ++command) {
		commands.push_back(*command);
		cout << "tokenized command: " << *command << endl << flush; 
	}
}

void run(char **command, char **argv){
			int	j = fork();
			if(j == -1){
				cout << "rshell: " << flush;
				perror("fork");
			}
				
			else if(j == 0){	
				if(-1 == execvp(command[0] , argv)); 
					cout << "rshell: " << flush;
					perror(*command);
			}

			else{
				if(-1 == wait(0))
					perror("wait");
			}
}

void rmComments(string &s){
	size_t loc = string::npos;
	loc = s.find("#");
	if(loc != string::npos){
		s.erase(loc);
	}
}
