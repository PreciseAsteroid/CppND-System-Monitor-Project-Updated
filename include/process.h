#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int _pid); // constructor
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Uid();
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process & a);  // TODO: See src/process.cpp
  bool operator==(const Process & a) const;  // TODO: See src/process.cpp


  // TODO: Declare any necessary private members
 private:
    int pid;
    std::string command;
    std::string user;
    std::string uid;
    static int clockTicks;
};

#endif