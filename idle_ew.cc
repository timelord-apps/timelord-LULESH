#include <sched.h>
#include <omp.h>
#include "iostream"
#include <csignal>
#include <fstream>
#include <string>
#include <iomanip>
#include <unistd.h>

//Variables for random number calculations
unsigned int seed = 123;  // Initial seed value
unsigned int a = 1103515245;  // Multiplier
unsigned int c = 12345;  // Increment
unsigned int m = 2147483648;  // Modulus

//Iteration counts
unsigned long long int iterations = 0;

std::string filename = "output_" + std::to_string(getpid()) + ".txt";

//Function to handle the termination signal
void signalHandler(int signal) {
    std::ofstream file(filename);
    if (file.is_open()) {
        std::cout << "\nReceived termination signal" << std::endl;
        file << "The Random Value is " << seed << std::endl;
        file << "Total Iterations: " << iterations << std::endl;
        //cout total iterations with scientific notation with e
        std::cout << "Total Iterations: " << std::scientific << std::setprecision(8) << iterations << std::endl;
        file.close(); // Close the file
    }
    else {
        std::cerr << "Failed to open the file." << std::endl;
    }
    exit(signal);
}

// Random number calculations
unsigned int lcg() {
    seed = (a * seed + c) % m;
    return seed;
}

int main() {

    // Install signal handler
    std::signal(SIGTERM, signalHandler);
    std::signal(SIGINT, signalHandler);

    // std::cout << "Installed Signal Handlers" << std::endl;

    pid_t parent_pid = getppid();
    std::cout << "Parent's PID in the child process: " << parent_pid << std::endl;

     // Get the CPU affinity of the parent process
    cpu_set_t parent_affinity;
    if (sched_getaffinity(parent_pid, sizeof(cpu_set_t), &parent_affinity) == -1) {
        std::cout << "Cannot get CPU affinity for parent process\n";
        exit(1);
    }

    std::cout << "Set CPU affinity for child process\n";

    if (sched_setaffinity(0, sizeof(cpu_set_t), &parent_affinity) == -1) {
        std::cout << "Cannot set CPU affinity for parent process\n";
        exit(1);
    }

    pid_t pid = getpid(); // Get the process ID of the current process

    int result;
    struct sched_param params;
    params.sched_priority = 0; // Unused for SCHED_IDLE


    //Change the scheduling
    result = sched_setscheduler(pid, SCHED_IDLE, &params);
    if (result == -1)
    {
        std::cout << "Failed to set scheduling policy. Error: " << errno << std::endl;
        return 1;
    }
    
    std::cout << "Started Calcs" << std::endl;
    //Begin extra work
    while(true) {
        lcg();
        iterations++;
    }
    return 0;
}