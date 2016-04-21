#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

void loadResources(int* totalResources, int* processes, int** allocation) {
	ifstream readFile;
	string data;
	readFile.open("data.txt");
	getline(readFile, data, ';');
	*processes = atoi(data.c_str());
	getline(readFile, data);
	for (int i = 0; i < data.length(); i++) {
		if (data[i] == ',') {
			*totalResources = *totalResources + 1;
		}//end if statement
	}//end for loop
	int length = data.length();
	if (data[length] != ',') {
		*totalResources = *totalResources + 1;
	}//end if statement
}//end readFile function

int main() {
	int totalResources = 0;
	int processes = 0;
	int* available;// = new int[*totalResources];
	int** allocation;// = new int*[*processes];
	int** request;// = new int*[*processes];
	int* allocatedResources;// = new int[*totalResources];
	int* requestedResources;// = new int[*totalResources];

	loadResources(&totalResources, &processes, allocation);
	cout << "Number of processes: " << processes << endl;
	cout << "Number of resources: " << totalResources << endl;
	return 0;
}//end main function
