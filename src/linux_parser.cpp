#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <map>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line;
  string key;
  float value;
  float memUtil;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  float memAvailable, memTotal;
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      // std::replace(line.begin(), line.end(), ' ', '_');
      // std::replace(line.begin(), line.end(), '=', ' ');
      // std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemAvailable:") {
          // std::replace(value.begin(), value.end(), '_', ' ');
          memAvailable = value;
        } else {
          if (key == "MemTotal:") {
            // std::replace(value.begin(), value.end(), '_', ' ');
            memTotal = value;
          }
        }
      }
    }
  }
  memUtil = (memTotal - memAvailable) / memTotal;
  return memUtil;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  /*
  /proc/uptime
          This file contains two numbers: the uptime of the system
          (seconds), and the amount of time spent in idle process
          (seconds).
  */
  long int upTime;
  string line;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> upTime;
  }
  return upTime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// DONE: Read and return CPU utilization
// vector<string> LinuxParser::CpuUtilization() { return {};}
float LinuxParser::CpuUtilization() {
  string line, cpu;
  std::ifstream stream(kProcDirectory + kStatFilename);
  std::map<CPUStates, float> stats_map;

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    //     user    nice   system  idle      iowait irq   softirq  steal  guest
    //     guest_nice
    linestream >> cpu >> stats_map[CPUStates::kUser_] >>
        stats_map[CPUStates::kNice_] >> stats_map[CPUStates::kSystem_] >>
        stats_map[CPUStates::kIdle_] >> stats_map[CPUStates::kIOwait_] >>
        stats_map[CPUStates::kIRQ_] >> stats_map[CPUStates::kSoftIRQ_] >>
        stats_map[CPUStates::kSteal_] >> stats_map[CPUStates::kGuest_] >>
        stats_map[CPUStates::kGuestNice_];
  }

  // calculation logic

  // PrevIdle = previdle + previowait
  // Idle = idle + iowait
  float idle = stats_map[CPUStates::kIdle_] + stats_map[CPUStates::kIOwait_];

  // PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq +
  // prevsteal NonIdle = user + nice + system + irq + softirq + steal
  float nonIdle = stats_map[CPUStates::kUser_] + stats_map[CPUStates::kNice_] +
                  stats_map[CPUStates::kSystem_] + stats_map[CPUStates::kIRQ_] +
                  stats_map[CPUStates::kSoftIRQ_] +
                  stats_map[CPUStates::kSteal_];

  // PrevTotal = PrevIdle + PrevNonIdle
  // Total = Idle + NonIdle
  float total = idle + nonIdle;
  // # differentiate: actual value minus the previous one
  // totald = Total - PrevTotal
  // idled = Idle - PrevIdle

  // CPU_Percentage = (totald - idled)/totald
  float cpu_pct = (total - idle) / total;
  return cpu_pct;
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  // return LinuxParser::readStat("processes");
  return LinuxParser::readKeyValFromFile<int>("processes",
                                              kProcDirectory + kStatFilename);
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  // return LinuxParser::readStat("procs_running");
  return LinuxParser::readKeyValFromFile<int>("procs_running",
                                              kProcDirectory + kStatFilename);
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid [[maybe_unused]]) { return string(); }

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  return LinuxParser::readKeyValFromFile<std::string>(
      "Uid:", kProcDirectory + to_string(pid) + kStatusFilename);
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(std::string Uid) {
  std::string value = "NOTFOUND";

  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    string line;
    string key;

    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      vector<std::string> result;
      while (linestream.good()) {
        std::string substr;
        getline(linestream, substr, ':');
        result.push_back(substr);
      }
      if (result[2] == Uid) {
        value = result[0];
        return value;
      }
    }
  }
  return value;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid [[maybe_unused]]) { return 0; }


template <class T>
T LinuxParser::readKeyValFromFile(const std::string& _key,
                                  const std::string& file) {
  // T value = 0;
  T value;

  std::ifstream filestream(file);
  if (filestream.is_open()) {
    string line;
    string key;

    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == _key) {
          return value;
        }
      }
    }
  }
  return value;
}
std::vector<std::string> LinuxParser::readFileToVector(const char _seperator, const std::string& file) {
  vector<std::string> result;
  std::ifstream filestream(file);
  if (filestream.is_open()) {
    string line;
    // string key;

    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      // vector<std::string> result;
      while (linestream.good()) {
        std::string substr;
        getline(linestream, substr, _seperator);
        result.push_back(substr);
      }
    }
  }
  return result;
}