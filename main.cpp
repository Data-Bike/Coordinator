/* 
 * File:   main.cpp
 * Author: newworld
 *
 * Created on 5 Август 2015 г., 1:35
 */

#include <cstdlib>
#include <iostream>
#include <string>
#include "coordinator.h"
using namespace std;
using namespace coordinator;

/*
 * 
 */
int main(int argc, char** argv) {
    std::map <std::string, std::string> config;
    config.insert(std::pair<std::string, std::string>("port", "12345"));
    config.insert(std::pair<std::string, std::string>("clients", "12345"));
    config.insert(std::pair<std::string, std::string>("threadscount", "1"));
    config.insert(std::pair<std::string, std::string>("len_block", "1024"));
    ::coordinator::coordinator * c = new ::coordinator::coordinator(config);
    c->play();
    cin.get();
    return 0;
}

