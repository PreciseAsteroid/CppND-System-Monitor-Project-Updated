#include "format.h"

#include <iomanip>
#include <sstream>
#include <string>

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
// string Format::ElapsedTime(long seconds[[maybe_unused]]) { return string(); }
string Format::ElapsedTime(long seconds) {
  long seconds_left = seconds;
  int secs, hours, minutes, days;
  
  days = seconds_left / (24 * 60 * 60);
  seconds_left = seconds_left - days * (24 * 60 * 60);
  if (days > 99) days = 99;

  hours = seconds_left / (60 * 60);
  seconds_left = seconds_left - hours * (60 * 60);

  minutes = seconds_left / (60);
  seconds_left = seconds_left - minutes * (60);

  secs = seconds_left;
  auto s = std::to_string(42);

//   std:string result = Format::IntToString(2, hours) + ":" + Format::IntToString(2, minutes) +
//          ":" + Format::IntToString(2, secs);

  return Format::IntToString(2, hours) + ":" + Format::IntToString(2, minutes) +
         ":" + Format::IntToString(2, secs);
}
string Format::IntToString(int length, int number) {
  std::stringstream ss;
  ss << std::setw(length) << std::setfill('0') << number;
  std::string s = ss.str();
  return s;
}