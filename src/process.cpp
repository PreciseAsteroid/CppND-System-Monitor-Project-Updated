#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"


using std::string;
using std::to_string;
using std::vector;

int Process::clockTicks = sysconf(_SC_CLK_TCK);


Process::Process(int _pid):pid(_pid){
    // set the things that won't change
    // user
    uid = LinuxParser::Uid(Pid());
};
// DONE: Return this process's ID
int Process::Pid() { return pid; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() { 
    std::vector<std::string> cpu_vector = LinuxParser::readFileToVector(' ',LinuxParser::kProcDirectory + to_string(Pid()) + LinuxParser::kStatFilename);
    
    // #14 utime - CPU time spent in user code, measured in clock ticks
    int utime = std::stoi(cpu_vector[13]);
    // #15 stime - CPU time spent in kernel code, measured in clock ticks
    int stime = std::stoi(cpu_vector[14]);
    // #16 cutime - Waited-for children's CPU time spent in user code (in clock ticks)
    int cutime = std::stoi(cpu_vector[15]);
    // #17 cstime - Waited-for children's CPU time spent in kernel code (in clock ticks)
    int cstime = std::stoi(cpu_vector[16]);
    // #22 starttime - Time when the process started, measured in clock ticks
    int starttime = std::stoi(cpu_vector[21]);

    int total_time = utime + stime + cutime + cstime;
    // float hertz = sysconf(_SC_CLK_TCK);
    float seconds = LinuxParser::UpTime() - (starttime / clockTicks);
    float cpu_usage = (total_time / clockTicks) / seconds;
    
    return cpu_usage;
    
 }

// DONE: Return the command that generated this process
string Process::Command() { 
    std::vector<std::string> cmd_vector = LinuxParser::readFileToVector(' ',LinuxParser::kProcDirectory + to_string(Pid()) + LinuxParser::kCmdlineFilename);
    if (cmd_vector.empty())
    {
        return "";
    } 
    
    
    return cmd_vector[0];
 }

// DONE: Return this process's memory utilization
string Process::Ram() { 
    int vmsize = LinuxParser::readKeyValFromFile <int>("VmSize:", LinuxParser::kProcDirectory + to_string(Pid()) + LinuxParser::kStatusFilename);
    return to_string(vmsize / 1000); // convert to MB
 }

// DONE: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(uid); }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { 
    std::vector<std::string> stat_vector = LinuxParser::readFileToVector(' ',LinuxParser::kProcDirectory + to_string(Pid()) + LinuxParser::kStatFilename);
    /*
     (22) starttime %llu
    The time the process started after system boot. In kernels before Linux 2.6, this value was expressed in jiffies.
    Since Linux 2.6, the value is expressed in clock ticks (divide by sysconf(_SC_CLK_TCK)).
    */
    long int processUptime = LinuxParser::UpTime() - (std::stoi(stat_vector[21])/ clockTicks);    
    return processUptime;
 }

//DONE
string Process::Uid() { return uid; }




// DONE: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process & a) { 
    return (this->CpuUtilization() > a.CpuUtilization());   
 }

bool Process::operator==(const Process & a) const{ 
    if (this->pid == a.pid){
        return true;
    } else {
        return false;
    }  
 }