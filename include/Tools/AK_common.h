#ifndef AK_COMMON_H_
#define AK_COMMON_H_

#include <iostream>

// remove the carriage return of "getLine"
#ifdef linux
std::string trim(const std::string& str);   
#endif

#endif