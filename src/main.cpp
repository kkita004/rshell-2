#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h> 
#include <boost/tokenizer.hpp>

using namespace boost;
using namespace std;

int main(int argc, char **argv){
	while(1){	 //continue prompting until user exits
		string input;
		vector<string> commands;
		vector<string> flags;
		string temp;

		cout << "$ "; // prompt		
		getline(cin,input);

		//tokenize each command from user input
		char_separator<char> delim("; ");
		tokenizer< char_separator<char> > mytok(input ,delim);

		for(auto command = mytok.begin(); command != mytok.end() ; ++command) {
				commands.push_back(*command);	
				cout << *command << endl; 
		}
		for(size_t i = 0; i < commands.size(); i++){ // loop until every command 
													// is done or if connector && breaks
 			for(size_t i = 0; i < flags.size(); i++){
				cout << "FLAGS: " << flags.at(i) << endl;	
 			}

			if(commands.at(i) == "exit") {
				exit(0);
			}
			
			int	j = fork();
	
			if(j == -1){
				perror("fork");
			}
				
			else if(j == 0){	
				if(-1 == execvp(commands.at(i).c_str(),argv)); 
					perror(commands.at(0).c_str());		
			}

			else{
				wait(0);
			}
		}	
	}	
	return 0;
}
