#include <iostream>
#include <cstdlib>
#include <dirent.h>
#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

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
	} //while

} //main
