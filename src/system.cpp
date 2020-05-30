#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
// using std::find;

#include "linux_parser.h"

// DONE: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// DONE: Return a container composed of the system's processes
vector<Process>& System::Processes() {
    vector<int> processes_pids = LinuxParser::Pids();
    // vector<Process> processes;
    for (auto &&i : processes_pids){
        Process candiateProcess(i);
        if( processes_.empty()||
            std::find(processes_.begin(), processes_.end(), candiateProcess) == processes_.end()){
                    processes_.push_back(candiateProcess);
        }
    }
    std::sort(processes_.begin(),processes_.end());
    return processes_;    
 }

// DONE: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// DONE: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// DONE: Return the operating system name
std::string System::OperatingSystem() { 
    // return string();
    return LinuxParser::OperatingSystem(); 
    }

// DONE: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// DONE: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// DONE: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }