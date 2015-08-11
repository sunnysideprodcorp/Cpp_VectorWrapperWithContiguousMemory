
#ifndef LOGGING_POLICY_H
#define LOGGING_POLICY_H


#include <string>
#include <iostream>

//Chatty policy users print log statements to std::cout
template <typename U>
struct Chatty
{
  static void Print(std::string s)
  {
    std::cout << s << std::endl;
  }
};

//Chatty policy users print null statements
template <typename U>
struct Silent
{
  static void Print(std::string s)
  {
  }
};

#endif  // LOGGING_POLICY_H
