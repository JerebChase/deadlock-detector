
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

//This function checks to see if the request for resources by a process
//is less than the total available resources
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

//Checks to see if a process has finished
//By summing the resources and seeing if they equal zero
//Returns true if process is finished and false if not
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

//Copies the data from one matrix to another
void copyMatrix(int* totalResources, int* processes, int** originalMatrix, int** copiedMatrix) {
	for (int i = 0; i < *processes; i++) {
		for (int j = 0; j < *totalResources; j++) {
			copiedMatrix[i][j] = originalMatrix[i][j];
		}//end for loop
	}//end for loop
}//end copyMatrix function

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
	readFile.close();
}//end loadData function

//This function determines the amount of each resource available
//It also determines the inital allocation of resources to each process
void loadResources(int* totalResources, int* processes, int* available, int** allocation, int* request) {
	ifstream readFile;
	stringstream readData;
	string data;
	string resource;
	readFile.open("data.txt");
	getline(readFile, data, ';');
	getline(readFile, data);
	readData << data;
	for (int i = 0; i < *totalResources; i++) {
		getline(readData, resource, ',');
		available[i] = atoi(resource.c_str());
	}//end for loop
	for (int i = 0; i < *processes; i++) {
		getline(readFile, data);
		readData.clear();
		readData.str("");
		readData << data;
		for (int j = 0; j < *totalResources; j++) {
			getline(readData, resource, ',');
			allocation[i][j] = atoi(resource.c_str());
		}//end for loop
	}//end for loop
	for (int i = 0; i < *processes; i++) {
		for (int j = 0; j < *totalResources; j++) {
			request[j] = allocation[i][j];
		}//end for loop
		updateAvailable(totalResources, available, request);
	}//end for loop
	readFile.close();
}//end loadResources function

//This deadlock detection algorithm does not preempt resources
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
	delete safeSequence;
}//end deadlockDetect function

//This function preempts resources from processes after three consecutive no responses to resource requests 
bool preempt(int* totalResources, int* processes, int** copyAllocation, int** totalRequest, int* predict, bool* finish, int relinquish, int process) {
	bool keepGoing = true;
	bool test;
	cout << "P" << process << " needs more resources" << endl;
	while (keepGoing) {
		bool check = true;
		while (check) {
			if (relinquish == *processes) {
				check = false;
			}//end if statement
			else if (finish[relinquish] == true) {
				relinquish++;
			}//end if statement
			else if (relinquish == process) {
				relinquish++;
			}//end if statement
			else {
				check = false;
			}//end else statement
		}//end while loop
		if (relinquish < *processes) {
			cout << "Release P" << relinquish << "'s resources" << endl;
			for (int i = 0; i < *totalResources; i++) {
				predict[i] += copyAllocation[relinquish][i];
				totalRequest[relinquish][i] += copyAllocation[relinquish][i];
				copyAllocation[relinquish][i] = 0;
			}//end for loop
			test = isLessThan(totalResources, totalRequest[process], predict);
			if (test == true) {
				keepGoing = false;
			}//end if statement
			else if (test == false) {
				relinquish++;
			}//end else if statement
		}//end if statement
		else {
			test = false;
			cout << "There are no processes whose resources can be released." << endl;
			keepGoing = false;
		}//end else statement
	}//end while loop
	return test;
}//end preempt function

//This deadlock detection function includes preemption of resources
void deadlockDetectTwo(int* totalResources, int* processes, int** totalAllocation, int** totalRequest, int* available, int* predict, int* allocate, int* request, bool* finish) {
	bool keepGoing = true;
	bool test;
	int safeOrder = 0;
	int preemptTest = 0;
	int relinquish;
	int* safeSequence = new int[*processes];
	int** copyAllocation = new int*[*processes];
	for (int i = 0; i < *processes; i++) {
		copyAllocation[i] = new int[*totalResources];
	}//end for loop
	for (int i = 0; i < *totalResources; i++) {
                predict[i] = available[i];
        }//end for loop
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
	copyMatrix(totalResources, processes, totalAllocation, copyAllocation);
	while (keepGoing) {
		bool finishCheck = true;
		int process = 0;
		while (finishCheck) {
			if (finish[process] == false) {
				test = isLessThan(totalResources, totalRequest[process], predict);
				if (test == false) {
					preemptTest++;
					if (preemptTest == 3) {
						relinquish = 0;
						test = preempt(totalResources, processes, copyAllocation, totalRequest, predict, finish, relinquish, process);
						preemptTest = 0;
						if (test == false) {
							copyMatrix(totalResources, processes, totalAllocation, copyAllocation);
						}//end else if statement
						finishCheck = false;
					}//end if statement
					else {
						process = (process + 1) % 6;
					}//end else statement
				}//end if statement
				else if (test == true) {
					preemptTest = 0;
					finishCheck = false;
				}//end else if statement
			}//end if statement
			else if (finish[process] == true) {
				process = (process + 1) % 6;
			}//end else if statement
		}//end while loop
		if (test == true) {
			for (int j = 0; j < *totalResources; j++) {
				predict[j] = predict[j] + copyAllocation[process][j];
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
	for (int i = 0; i < *processes; i++) {
		delete copyAllocation[i];
	}//end for loop
	delete copyAllocation;
	delete safeSequence;
}//end deadlockDetectTwo function

//This function loads the resource requests and calls the deadlock detection function
//The deadlock detection function with preemption is the default
//To run without preemption, un-comment the deadlockDetect function call
//And comment the deadlockDetectTwo function call
void loadRequests(int* totalResources, int* processes, int* counter, int* available, int** totalAllocation, int** totalRequest, int* predict, int* allocate, int* request, bool* finish) {
	int repeat = 0;
	ifstream readFile;
	stringstream readData;
        string data;
        string resource;
        readFile.open("data.txt");
	for (int i = 0; i <= *processes; i++) {
		getline(readFile, data);
	}//end for loop
	while (repeat < *counter) {
		for (int i = 0; i < *processes; i++) {
                	getline(readFile, data);
			readData.clear();
			readData.str("");
			readData << data;
                	for (int j = 0; j < *totalResources; j++) {
				getline(readData, resource, ',');
				totalRequest[i][j] = atoi(resource.c_str());
                	}//end for loop
        	}//end for loop
		for (int i = 0; i < *totalResources; i++) {
			predict[i] = available[i];
		}//end for loop
		cout << "Resource Request: " << endl;
                for (int i = 0; i < *processes; i++) {
                        for (int j = 0; j < *totalResources; j++) {
                                cout << totalRequest[i][j] << ", ";
                        }//end for loop
                        cout << endl;
                }//end for loop
                cout << "Available Resources: " << endl;
                for (int i = 0; i < *totalResources; i++) {
                        cout << available[i] << ", ";
                }//end for loop
                cout << endl;
		//deadlockDetect(totalResources, processes, totalAllocation, totalRequest, predict, allocate, request, finish);
		deadlockDetectTwo(totalResources, processes, totalAllocation, totalRequest, available, predict, allocate, request, finish);
		repeat++;
	}//end while loop
	readFile.close();
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
	counter = (counter / processes) - 1;

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

	loadRequests(&totalResources, &processes, &counter, available, totalAllocation, totalRequest, predict, allocate, request, finish);
	
	for (int i = 0; i < processes; i++) {
		delete totalAllocation[i];
		delete totalRequest[i];
	}//end for loop
	delete totalAllocation;
	delete totalRequest;
	delete available;
	delete predict;
        delete allocate;
        delete request;
        delete finish;
	return 0;
}//end main function
