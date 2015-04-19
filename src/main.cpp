#include <iostream>
#include <sstream>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h> 
#include <boost/tokenizer.hpp>

using namespace boost;
using namespace std;

void run(char *command, char **flags);
string userinput();
void parse(const string &input, vector<string> &commands);

int main(){
	while(1){	 //continue prompting until user exits
		vector<string> commands;
		cout << "$ "; // prompt		
		string input = userinput();
		parse(input, commands);
		for(size_t i = 0; i < commands.size(); i++){ // loop until every command 
													// is done or if connector && breaks		
			//run(commands.at(i), flag);
		}	
	}	
	return 0;
}

string userinput(){ // get user input 
	string input;
	getline(cin, input);
	return input;
}

void parse(const string &input, vector<string> &commands){
	char_separator<char> delim(";");
	tokenizer< char_separator<char> > mytok(input ,delim);
	for(auto command = mytok.begin(); command != mytok.end() ; ++command) {
		commands.push_back(*command);	
		cout << "tokenized command: " << *command << endl << flush; 
	}

		
}
void run(char *command, char **flags){
			int	j = fork();

			if(j == -1){
				perror("Error in fork");
			}
				
			else if(j == 0){	
				if(-1 == execvp(command,flags)); 
					perror("Syntax error");
			}

			else{
				if(-1 == wait(0))
					perror("wait");
			}
}

