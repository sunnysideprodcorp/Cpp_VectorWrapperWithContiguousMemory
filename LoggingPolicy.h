#ifndef LOGGING_POLICY_H
#define LOGGING_POLICY_H
#include <string>
#include <iostream>

// Logging policy determines the logging policy both of the
// memory-managing Arena class associated with SmallVector and
// with SmallVector itself


// Chatty policy users print log statements to std::cout
template <typename U>
struct Chatty
{
  static void Print(std::string s)
  { std::cout << s << std::endl; }
};

// Silent policy users print null statements
// In many compilers this is likely optimized away, meaning no performance hit
// But confirm for your application
template <typename U>
struct Silent
{
  static void Print(std::string s) { }
};

#endif  // LOGGING_POLICY_H
