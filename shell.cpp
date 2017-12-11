#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <sys/wait.h>
#include <sys/types.h>

using namespace std;

/**
* Displays the prompt
*/
void prompt() {

	char path[FILENAME_MAX]; // get path name

	// check for error retrieving path
	if (!getcwd(path, sizeof(path)))
		cout << "Error: getcwd()" << endl;

	// display prompt
	cout << "1730sh:" << path << "$ ";

} //prompt

int main() {
	int iodirection = -1; //int representing I/O redirection
	int fd;
	int backup;
	string inputstr = ""; // user input of type string
	char* input = &inputstr[0]; // converts input to char array
	// program runs until 'exit' is typed
	while (strcmp(input, "exit") != 0) {
		prompt();
		getline(cin, inputstr);
		iodirection = -1;
		// exits from loop if exit is passed
		if (strcmp(input, "exit") == 0) {
			break;
		}

		string argsstr[12]; // breaks args up by spaces
		stringstream ss(input);
		int numArgs = 0; // counts number of arguments

		// fills args array
		while (ss >> argsstr[numArgs]) {
			numArgs++;
 		} //while

		char * args[12] = {nullptr};
		// converts string args to char* args
		for (int i = 0; i < numArgs; i++) {

			//check if I/O redirection
			if (argsstr[i] == ">" || argsstr[i] == ">>" || argsstr[i] == "<" || argsstr[i] == "<<") {
				if (argsstr[i] == ">") {
					iodirection = 0;
					fd = open((char*)argsstr[i+1].c_str(),O_CREAT|O_RDWR|O_TRUNC,0777);
				}else if (argsstr[i] == ">>") {
					iodirection = 0;
					fd = open((char*)argsstr[i+1].c_str(),O_CREAT|O_RDWR|O_APPEND,0777);
				}else if (argsstr[i] == "<") {
					iodirection = 1;
					fd = open((char*)argsstr[i+1].c_str(),O_RDONLY);
				}
				break; //breaks out of loop if detects redirection
			}
			args[i] = (char*) argsstr[i].c_str();
		}

		int val = fork();

		if (val == 0) {
			// child
			//redirects stdout to fd
			if (iodirection == 0) {
				backup = dup(STDOUT_FILENO);
				int i = dup2(fd,1);
			}
			if (iodirection == 1) {
				backup = dup(STDIN_FILENO);
				int i = dup2(fd,0);
			}
			execvp(args[0], args); // execute command

			//redirects back to stdout
			if (iodirection == 0) {
				dup2(backup,1);
			}
			if (iodirection == 1) {
				int i = dup2(backup,0);
			}
			
			break; // terminates child if command does not exist
		} else {
			// parent
			wait(0); // wait for child to finish
		} //if-else
	} //while

} //main
