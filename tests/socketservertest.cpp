/* 
 * File:   socketservertest.cpp
 * Author: newworld
 *
 * Created on 06.08.2015, 4:22:56
 */

#include <stdlib.h>
#include <iostream>
#include <arpa/inet.h>
#include "socketserver.h"

/*
 * Simple C++ Test Suite
 */

void test1() {
    std::cout << "socketservertest test 1" << std::endl;
}

void test2() {
    std::cout << "socketservertest test 2" << std::endl;
    std::cout << "%TEST_FAILED% time=0 testname=test2 (socketservertest) message=error message sample" << std::endl;
}

void createsocketservertest() {

    std::cout << "socketservertest test createsocketservertest" << std::endl;
    std::map <std::string, std::string> config;
    config.insert(std::pair<std::string, std::string>("port", "12345"));
    config.insert(std::pair<std::string, std::string>("clients", "12345"));
    config.insert(std::pair<std::string, std::string>("threadscount", "4"));
    config.insert(std::pair<std::string, std::string>("len_block", "1024"));
    std::cout << "createsocketservertest test config" << std::endl;
    try {
        socketserver::socketserver ss(config);
        ss.stop();
    } catch (exception e) {
        std::cout << "%TEST_FAILED% time=0 testname=createsocketservertest (socketservertest) message=error message sample" << std::endl;
    }
}

void playsocketservertest() {

    std::cout << "socketservertest test playsocketservertest" << std::endl;
    std::map <std::string, std::string> config;
    config.insert(std::pair<std::string, std::string>("port", "12345"));
    config.insert(std::pair<std::string, std::string>("clients", "12345"));
    config.insert(std::pair<std::string, std::string>("threadscount", "10"));
    config.insert(std::pair<std::string, std::string>("len_block", "1024"));
    std::cout << "playsocketservertest test config" << std::endl;
    try {
        std::cout << "playsocketservertest test create" << std::endl;
        socketserver::socketserver ss(config);
        std::cout << "playsocketservertest test play" << std::endl;
        ss.play();
        std::cout << "playsocketservertest test stop" << std::endl;
        ss.stop();
        std::cout << "playsocketservertest test finish" << std::endl;
    } catch (exception e) {
        std::cout << "%TEST_FAILED% time=0 testname=playsocketservertest (socketservertest) message=error message sample" << std::endl;
    }
}

void readwritesocketservertest() {

    std::cout << "socketservertest test playsocketservertest" << std::endl;
    std::map <std::string, std::string> config;
    config.insert(std::pair<std::string, std::string>("port", "12345"));
    config.insert(std::pair<std::string, std::string>("clients", "12345"));
    config.insert(std::pair<std::string, std::string>("threadscount", "10"));
    config.insert(std::pair<std::string, std::string>("len_block", "1024"));
    //    try {
    std::cout << "socketservertest test socketserver::socketserver ss(config);" << std::endl;
    socketserver::socketserver ss(config);
    std::cout << "socketservertest test ss.play();" << std::endl;
    ss.play();
    sockaddr_in sa;
    sa.sin_port = htons(std::atoi(config["port"].c_str()));
    sa.sin_family = AF_INET;
    ::inet_aton("127.0.0.1", &sa.sin_addr);
    int s_test = ::socket(PF_INET, SOCK_STREAM, 0);
    string test_str = "0dKAJALKDjsalkdja:LKDjADSJLASDJ1";
    vector <unsigned char> test_data(test_str.begin(), test_str.end());
    std::cout << "socketservertest test ::connect(s_test, (sockaddr*) & sa, sizeof (sa));" << std::endl;
    int con = ::connect(s_test, (sockaddr*) & sa, sizeof (sa));
    std::cout << "connect result:" << con << std::endl;
    ss.on("read", [&ss, &test_str, &test_data](int s_recv, vector<unsigned char> data) {
        std::cout << "hello!" << std::endl;
        string str(data.begin(), data.end());
        if (test_str != str) {
            std::cout << "%TEST_FAILED% time=0 testname=playsocketservertest (socketservertest) message=error on read" << std::endl;
        }
        std::cout << "\ttest_str:" << test_str << std::endl;
                std::cout << "\tstr:" << str << std::endl;
                ss.write(s_recv, test_data);
    });
    std::cout << "socketservertest test ::send(s_test, &test_data[0], test_data.size(), 0);:" << s_test << ":" << test_data.size() << ":" << ::send(s_test, &test_data[0], test_data.size(), 0) << "|" << std::endl;
    //    ::send(s_test, &test_data[0], test_data.size(), 0);
    std::cout << "socketservertest test ::send(s_test, &test_data[0], test_data.size(), 0); ok" << std::endl;
    vector<unsigned char> recv_data(10240);
    std::cout << "socketservertest test vector<unsigned char> recv_data(10240); ok" << std::endl;
    int cnt_rcv = ::recv(s_test, &recv_data[0], test_data.size(), MSG_WAITALL);
    recv_data.resize(cnt_rcv);
    std::cout << "socketservertest test recv_cnt:" << cnt_rcv << std::endl;

    string recv_str(recv_data.begin(), recv_data.end());
    std::cout << "socketservertest test recv_str:" << recv_str << std::endl;
    if (!(test_str == recv_str)) {
        std::cout << "%TEST_FAILED% time=0 testname=playsocketservertest (socketservertest) message=error on write" << std::endl;
        std::cout << "\ttest_str:" << test_str << std::endl;
        std::cout << "\trecv_str:" << recv_str << std::endl;
    }

    ss.stop();
    //    } catch (exception e) {
    //        std::cout << "%TEST_FAILED% time=0 testname=playsocketservertest (socketservertest) message=error catch" << std::endl;
    //        std::cout << e.what() << std::endl;
    //    }
}

int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% socketservertest" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% playsocketservertest (socketservertest)" << std::endl;
        playsocketservertest();
    std::cout << "%TEST_FINISHED% time=0 playsocketservertest (socketservertest)" << std::endl;

    std::cout << "%TEST_STARTED% readwritesocketservertest (socketservertest)\n" << std::endl;
    readwritesocketservertest();
    std::cout << "%TEST_FINISHED% time=0 readwritesocketservertest (socketservertest)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

