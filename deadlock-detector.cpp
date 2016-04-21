#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

void loadResources(int* totalResources, int* processes, int** allocation) {
	ofstream readFile;
	string data;
	readFile.open("data.txt");
	getline(readFile, string);
	cout << string << endl;
}//end readFile function

int main() {
	int* totalResources;
	int* processes;
	int* available = new int[*totalResources];
	int** allocation = new int*[*processes];
	int** request = new int*[*processes];
	int* allocatedResources = new int[*totalResources];
	int* requestedResources = new int[*totalResources];	

	loadResources(totalResources, processes, allocation);
	
	delete totalResources;
	delete processes;
	delete available;
	delete allocatedResources;
	delete requestedResources;
	return 0;
}//end main function
