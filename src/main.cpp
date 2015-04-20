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

int run(string &command); // return -1 if command failed
string userinput(); // retrieves inputted commands
void parse(const string &input, vector<string> &commands, char *del); //parse strings
void rmComments(string &s); // removes comments from commands
void prompt(); // prints out terminal prompt

int main(int argc, char **argv){
	while(1){	 //continue prompting until user exits
		vector<string> commands;
		int status = 0; // used to check if command was successful
		prompt();
		string input = userinput();
		rmComments(input); // removes comments from command string
		char delim[] = ";";
		parse(input, commands, delim); // parse string at each semicolon
		const unsigned int size = commands.size();

		for(size_t i = 0; i < size; i++){ // loop until every command is done || exit 
			trim(commands.at(i));
			if(commands.at(i) == "exit") 
				exit(0);
			status = run(commands.at(i));	
			if(-1 == status){	
				// code for && connector
			}
		//	if((-1 == status) && /* contains '||' */){
				// code for || connector
	//		}
			//else keep going 
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
	}
}

int run(string &command){
	int ret = 0; // return 0 if cmd succeeds -1 if fails

	// parse cmd in order to use as execvp parameter
	vector<char*> c;
	int size = command.size();
	char *token;
	char *temp = new char[size];
	strcpy(temp, command.c_str());
	token = strtok(temp, " ");
	c.push_back(token);
	while(token != NULL){
		token = strtok(NULL, " ");
		c.push_back(token);
	}
	c.push_back('\0'); // execvp 2nd arg looks for null to end line

	// call child process and run command
	pid_t pid = fork();
	if(pid == -1){
		cout << "rshell: " << flush;
		perror("fork");
		ret = -1;
	}
	else if(pid == 0){	
		if(-1 == execvp(c[0],  &c[0] )){ 
			cout << "rshell: " << flush;
			perror(c[0]);
			ret = -1;
		}
	}
	else{
		if(-1 == wait(0)){
			perror("wait");
			ret = -1;
			}
	}
	delete []temp; // delete dynam alloc mem
	return ret;
}

void rmComments(string &s){
	size_t loc = string::npos;
	loc = s.find("#");
	if(loc != string::npos){
		s.erase(loc);
	}
}

void prompt(){
	char *name;
	name = getlogin();
	if(name == NULL){
		perror("getlogin error");
	}
	char hostname[64] = ""; // max value of hostname is 64
	if(-1 == gethostname(hostname, 64)){
		perror("Hostname retrieval error.");
	}

	cout << name << "@" << hostname << "$ "; // prompt		

}
