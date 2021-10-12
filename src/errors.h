#include <string>
#include <iostream>

// not currently used due to hasError variable. Need to figure this out later
static void report (int line, std::string where, std::string message){
    std::cerr << "[line " << line 
            << "] Error" << where
            << ": " << message << std::endl;
}
static void error(int line, std::string message) {
    report(line, "", message);
}