#include <iostream>
#include <cstdlib>
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

	string inputstr = ""; // user input of type string
	char* input = &inputstr[0]; // converts input to char array
	// program runs until 'exit' is typed
	while (strcmp(input, "exit") != 0) {
		prompt();
		getline(cin, inputstr);
		
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
			args[i] = (char*) argsstr[i].c_str();
		}
		
		int val = fork();
		
		if (val == 0) {
			// child
			execvp(args[0], args); // execute command
			break; // terminates child if command does not exist
		} else {
			// parent
			wait(0); // wait for child to finish
		} //if-else
	} //while

} //main