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
		iodirection = -1; // by default, io is not redirected
		
		char* inputchar = new char[inputstr.length() + 1]; //used for temp holding input as char array
		strcpy(inputchar, inputstr.c_str());
		// exits from loop if exit is passed
		if (strcmp(inputchar, "exit") == 0) {
			exit(EXIT_SUCCESS);
		}

		string argsstr[12]; // breaks args up by spaces and =
		char* token = strtok(inputchar, "= ");
		int numArgs = 0; // counts number of arguments

		// fills args array
		while (token != NULL) {
			argsstr[numArgs] = token;
			numArgs++;
			token = strtok(NULL, "= ");
		} //while
		
		delete [] inputchar; // free memory
		
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

		//check if fisrt arg is export then does putenv and then continues
		if (numArgs > 1) {
			if (strcmp(args[0], "export") == 0) {
				string envv = argsstr[1] + "=" + argsstr[2];
				int pe = setenv((char*)argsstr[1].c_str(),(char*)argsstr[2].c_str(),0);
				std::cout << pe << '\n';
				if (pe != 0) {
					perror("export error");
				}
				continue;
			}
		}

		int val = fork();
		int i = 0;
		if (val == 0) {
			// child
			//redirects stdout to fd
			if (iodirection == 0) {
				backup = dup(STDOUT_FILENO);
				i = dup2(fd,1);
			}
			if (iodirection == 1) {
				backup = dup(STDIN_FILENO);
				i = dup2(fd,0);
			}

			// checks for errors during dup2
			if (i < 0) {
				cout << "Error: dup2()" << endl;
				exit(0);
			}
			execvp(args[0], args); // execute command

			//redirects back to stdout/stdin
			if (iodirection == 0) {
				i = dup2(backup,1);
			}
			if (iodirection == 1) {
				i = dup2(backup,0);
			}
			
			// checks for errors during dup2
			if (i < 0) {
				cout << "Error: dup2()" << endl;
				exit(0);
			}

			break; // terminates child if command does not exist
		} else {
			// parent
			wait(0); // wait for child to finish
		} //if-else
	} //while

	return EXIT_SUCCESS;

} //main
