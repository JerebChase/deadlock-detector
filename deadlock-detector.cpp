#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

void loadData(int* totalResources, int* processes) {
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
}//end function

void loadResources(int* totalResources, int* processes, int* available, int** allocation) {
	ifstream readFile;
	string data;
	string resource = "";
	readFile.open("data.txt");
	getline(readFile, data, ';');
	getline(readFile, data);
	int position = 0;
	for (int i = 0; i < data.length(); i++) {
		if (data[i] == ',') {
			available[position] = atoi(resource.c_str());
			position++;
			resource = "";
		}//end if statement
		else if (data[i] != ',') {
			resource += data[i];
		}//end else if statement
	}//end for loop
	int length = data.length();
	if (data[length] != ',') {
		available[position] = atoi(resource.c_str());
	}//end if statement
}//end readFile function

int main() {
	int totalResources = 0;
	int processes = 0;
	int* available;
	int** allocation;
	int** request;// = new int*[*processes];
	int* allocatedResources;// = new int[*totalResources];
	int* requestedResources;// = new int[*totalResources];

	loadData(&totalResources, &processes);
	available = new int[totalResources];
	allocation = new int*[processes];
	for (int i = 0; i < processes; i++) {
		allocation[i] = new int[totalResources];
	}//end for loop
	loadResources(&totalResources, &processes, available, allocation);
	cout << "Number of processes: " << processes << endl;
	cout << "Number of resources: " << totalResources << endl;
	cout << "Available resources: ";
	for (int i = 0; i < totalResources; i++) {
		cout << available[i] << ", ";
	}//end for loop
	return 0;
}//end main function
