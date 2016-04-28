# deadlock-detector
Overview:
The purpose of this project is to simulate a procedure to detect deadlock from process requests for resource allocation. The request data is read in from a file and the Banker's Algorithm is used to try to find a safe sequence which all processes requesting resources can follow to avoid deadlock. The algorithm uses the following matrices and arrays in the deadlock detection algorithm:

- available - an array holding the number of resources available at any given time
- totalAllocation - a matrix showing the number of resources allocated to each process
- totalRequest - a matrix holding the resource requests for each process
- finish - an array of boolean values showing whether or not a process has finished

A "safe sequence" is generated showing the order in which the processes can run to avoid deadlock, if such a sequence exists. If a safe sequence does not exist, deadlock will occur. The program contains two deadlock detection functions. One involves preemption and the other does not. The function that involves preemption will take resources from a process (if such a process exists) if there are three consecutive denials to resource reqeusts. The function with preemption will run automatically. To run the function without preemption, it will have to be un-commented in the loadRequests function and the deadlock detection function with preemption will have to be commented out.

Instructions:
To run this program, upload deadlock-detector.cpp and the included data.txt file onto pegasus or tesla. Compile the program by typing the following command: g++ deadlock-detector.cpp. Press enter. Then type ./a.out and press enter again. The program should output the results of the deadlock detection algorithm. Please note that the data.txt file has been modified to remove commas that were making the file "non-standard". Commas were removed after the lines representing resource requests by a process and the commas were random, thus they messed up the program's execution. Please use the file uploaded with the assignment to run the deadlock detection algorithm. 
