#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

bool isLessThan(int* totalResources, int* request, int* predict) {
	bool keepGoing = true;
	bool isLess = true;
	int i = 0;
	while (keepGoing) {
		if (i == *totalResources) {
			keepGoing = false;
		}//end if statement
		else if (request[i] <= predict[i]) {
			i++;
		}//end if statement
		else if (request[i] > predict[i]) {
			keepGoing = false;
			isLess = false;
		}//end else if statement
	}//end while loop
	return isLess;
}//end isLessThan function

bool isFinished(int* totalResources, int* allocate) {
	bool keepGoing = true;
	bool isZero = true;
	int sum = 0;
	int i = 0;
	while (keepGoing) {
		sum += allocate[i];
		if (i == *totalResources) {
			keepGoing = false;
		}//end if statement
		else if (sum == 0) {
			i++;
		}//end else if statement
		else if (sum > 0) {
			keepGoing = false;
			isZero = false;
		}//end else if statement
	}//end while loop
	return isZero;
}//end isFinished function

void updateAvailable(int* totalResources, int* available, int* request) {
	for (int i = 0; i < *totalResources; i++) {
		available[i] = available[i] - request[i];
	}//end for loop
}//end updateAvailable function

//This function determines the total number of processes and resources
//These numbers will be used in the deadlock detection simulation
void loadData(int* totalResources, int* processes, int* counter) {
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
	while (!readFile.eof()) {
		getline(readFile, data);
		*counter = *counter + 1;
	}//end while loop
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

void deadlockDetect(int* totalResources, int* processes, int** totalAllocation, int** totalRequest, int* predict, int* allocate, int* request, bool* finish) {
	bool keepGoing = true;
	bool test;
	int safeOrder = 0;
	int* safeSequence = new int[*processes];
	for (int i = 0; i < *processes; i++) {
		for (int j = 0; j < *totalResources; j++) {
			allocate[j] = totalAllocation[i][j];
		}//end for loop
		test = isFinished(totalResources, allocate);
		if (test == false) {
			finish[i] = false;
		}//end if statement
		else if (test == true) {
			finish[i] = true;
		}//end else if statement
	}//end for loop
	while (keepGoing) {
		bool finishCheck = true;
		int process = 0;
		while (finishCheck) {
			if (process >= *processes) {
				finishCheck = false;
				process--;
			}//end if statement
			else if (finish[process] == false) {
				test = isLessThan(totalResources, totalRequest[process], predict);
				if (test == false) {
					process++;
				}//end if statement
				else if (test == true) {
					finishCheck = false;
				}//end else if statement
			}//end else if statement
			else if (finish[process] == true) {
				process++;
			}//end else if statement
		}//end while loop
		if (test == true) {
			for (int j = 0; j < *totalResources; j++) {
				predict[j] = predict[j] + totalAllocation[process][j];
			}//end for loop
			finish[process] = true;
			safeSequence[safeOrder] = process;
			safeOrder++;
			if (safeOrder == *processes) {
				keepGoing = false;
				cout << "There is a safe sequence: ";
				for (int i = 0; i < *processes; i++) {
					cout << "P" << safeSequence[i] << ", ";
				}//end for loop
				cout << "Deadlock will NOT occur." << endl;
			}//end if statement
		}//end if statement
		else if (test == false) {
			keepGoing = false;
			cout << "There is not a safe sequence. Deadlock will occur." << endl;
		}//end else if statement
	}//end while loop
}//end deadlockDetect function

void loadRequests(int* totalResources, int* processes, int* counter, int* available, int** totalAllocation, int** totalRequest, int* predict, int* allocate, int* request, bool* finish) {
	int repeat = 0;
	ifstream readFile;
        string data;
        string resource = "";
        readFile.open("data.txt");
	for (int i = 0; i <= *processes; i++) {
		getline(readFile, data);
	}//end for loop
	bool keepGoing = true;
	bool keepReading = true;
	int k = 0;
	while (repeat <= *counter) {
		for (int i = 0; i < *processes; i++) {
                	k = 0;
                	getline(readFile, data);
                	for (int j = 0; j < *totalResources; j++) {
                        	while (keepReading) {
                        	        if (data[k] == ',') {
                        	                totalRequest[i][j] = atoi(resource.c_str());
                        	                resource = "";
                        	                k++;
                        	                keepReading = false;
                        	        }//end if statement
                        	        else if (k == data.length()) {
                        	                resource += data[k];
                        	                totalRequest[i][j] = atoi(resource.c_str());
                        	                keepReading = false;
                        	        }//end else if statement
                        	        else if (data[k] != ',') {
                        	                resource += data[k];
                        	                k++;
                        	        }//end else if statement
                        	}//end while loop
                        	keepReading = true;
                	}//end for loop
        	}//end for loop
		for (int i = 0; i < *totalResources; i++) {
			predict[i] = available[i];
		}//end for loop
		deadlockDetect(totalResources, processes, totalAllocation, totalRequest, predict, allocate, request, finish);
		repeat++;
	}//end while loop
}//end loadRequests function

int main() {
	int counter = 0;
	int totalResources = 0;
	int processes = 0;
	int* available;
	int** totalAllocation;
	int** totalRequest;
	int* predict;
	int* allocate;
	int* request;
	bool* finish;

	loadData(&totalResources, &processes, &counter);

	available = new int[totalResources];
	totalAllocation = new int*[processes];
	for (int i = 0; i < processes; i++) {
		totalAllocation[i] = new int[totalResources];
	}//end for loop
	totalRequest = new int*[processes];
	for (int i = 0; i < processes; i++) {
		totalRequest[i] = new int[totalResources];
	}//end for loop
	predict = new int[totalResources];
	allocate = new int[totalResources];
	request = new int[totalResources];
	finish = new bool[processes];
	counter = counter / processes;

	loadResources(&totalResources, &processes, available, totalAllocation, request);

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
			cout << totalAllocation[i][j] << ", ";
		}//end for loop
		cout << endl;
	}//end for loop
	cout << "Available Resources: " << endl;
        for (int i = 0; i < totalResources; i++) {
                cout << available[i] << ", ";
	}//end for loop
	cout << endl;

	loadRequests(&totalResources, &processes, &counter, available, totalAllocation, totalRequest, predict, allocate, request, finish);

	return 0;
}//end main function
