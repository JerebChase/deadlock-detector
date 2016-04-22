#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

void updateAvailable(int* totalResources, int* available, int* request) {
	for (int i = 0; i < *totalResources; i++) {
		available[i] = available[i] - request[i];
	}//end for loop
}//end updateAvailable function

//This function determines the total number of processes and resources
//These numbers will be used in the deadlock detection simulation
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
}//end loadData function

//This function determines the amount of each resource available
//It also determines the inital allocation of resources to each process
void loadResources(int* totalResources, int* processes, int* available, int** allocation, int* request) {
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
	int k = 0;
	bool keepGoing = true;
	resource = "";
	for (int i = 0; i < *processes; i++) {
		k = 0;
		getline(readFile, data);
		for (int j = 0; j < *totalResources; j++) {
			while (keepGoing) {
				if (data[k] == ',') {
					allocation[i][j] = atoi(resource.c_str());
					resource = "";
					k++;
					keepGoing = false;
				}//end if statement
				else if (k == data.length()) {
					resource += data[k];
					allocation[i][j] = atoi(resource.c_str());
					keepGoing = false;
				}//end else if statement
				else if (data[k] != ',') {
					resource += data[k];
					k++;
				}//end else if statement
			}//end while loop
			keepGoing = true;
		}//end for loop
	}//end for loop
	for (int i = 0; i < *processes; i++) {
		for (int j = 0; j < *totalResources; j++) {
			request[j] = allocation[i][j];
		}//end for loop
		updateAvailable(totalResources, available, request);
	}//end for loop
}//end loadResources function

void loadRequests(int* totalResources, int* processes, int** allocation, int** totalRequest, int* predict, int* request, bool* finish) {
	ifstream readFile;
        string data;
        string resource = "";
        readFile.open("data.txt");
	for (int i = 0; i <= *processes; i ++) {
		getline(readFile, data)
	}//end for loop
	bool keepGoing = true;
	while (keepGoing) {
		
	}//end while loop
}//end loadRequests function

int main() {
	int totalResources = 0;
	int processes = 0;
	int* available;
	int** allocation;
	int** totalRequest;
	int* predict;
	int* request;
	bool* finish;

	loadData(&totalResources, &processes);

	available = new int[totalResources];
	allocation = new int*[processes];
	for (int i = 0; i < processes; i++) {
		allocation[i] = new int[totalResources];
	}//end for loop
	totalRequest = new int*[processes];
	for (int i = 0; i < processes; i++) {
		totalRequest[i] = new int[totalResources];
	}//end for loop
	predict = new int[totalResources];
	request = new int[totalResources];
	finish = new bool[processes];

	loadResources(&totalResources, &processes, available, allocation, request);

	cout << "Number of processes: " << processes << endl;
	cout << "Number of resources: " << totalResources << endl;
	cout << "Available resources: " << endl;
	for (int i = 0; i < totalResources; i++) {
		cout << available[i] << ", ";
	}//end for loop
	cout << endl;
	cout << "Resource Allocation: " << endl;
	for (int i = 0; i < processes; i++) {
		for (int j = 0; j < totalResources; j++) {
			cout << allocation[i][j] << ", ";
		}//end for loop
		cout << endl;
	}//end for loop
	cout << "Available Resources: " << endl;
        for (int i = 0; i < totalResources; i++) {
                cout << available[i] << ", ";
	}//end for loop
	cout << endl;

	loadRequests(&totalResources, &processes, available, allocation, totalRequest, predict, request, finish);

	return 0;
}//end main function
