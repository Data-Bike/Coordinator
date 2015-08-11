/* 
 * File:   mainfunctiontest.cpp
 * Author: newworld
 *
 * Created on 05.08.2015, 1:38:56
 */

#include <stdlib.h>
#include <iostream>
#include "coordinator.h"
#include <arpa/inet.h>
#include <exception>
#include <chrono>
#include <ctime>
#include <string.h>
using namespace std::chrono;

/*
 * Simple C++ Test Suite
 */

void test1() {
    std::cout << "mainfunctiontest test 1" << std::endl;
}

void test2() {
    std::cout << "mainfunctiontest test 2" << std::endl;
    std::cout << "%TEST_FAILED% time=0 testname=test2 (mainfunctiontest) message=error message sample" << std::endl;
}

void createtest() {
    std::cout << "mainfunctiontest createcoordianator" << std::endl;
    coordinator::coordinator* coord;
    try {
        std::map <std::string, std::string> config;
        config.insert(std::pair<std::string, std::string>("port", "12345"));
        config.insert(std::pair<std::string, std::string>("clients", "12345"));
        config.insert(std::pair<std::string, std::string>("threadscount", "1"));
        config.insert(std::pair<std::string, std::string>("len_block", "1024"));
        coordinator::coordinator* coord = new coordinator::coordinator(config);

    } catch (std::exception &e) {
        std::cout << "%TEST_FAILED% time=0 testname=createcoordianator (mainfunctiontest) message=error message sample" << std::endl;
    }
    if (!coord || coord == NULL) {
        std::cout << "%TEST_FAILED% time=0 testname=createcoordianator (mainfunctiontest) message=error message sample" << std::endl;
    }
}

void startedtest() {
    std::cout << "mainfunctiontest createcoordianator" << std::endl;
    coordinator::coordinator* coord;
    try {
        std::map <std::string, std::string> config;
        config.insert(std::pair<std::string, std::string>("port", "12345"));
        config.insert(std::pair<std::string, std::string>("clients", "12345"));
        config.insert(std::pair<std::string, std::string>("threadscount", "1"));
        config.insert(std::pair<std::string, std::string>("len_block", "1024"));
        coordinator::coordinator* coord = new coordinator::coordinator(config);
        coord->play();
        coord->stop();
    } catch (std::exception &e) {
        std::cout << "%TEST_FAILED% time=0 testname=createcoordianator (mainfunctiontest) message=error message sample" << std::endl;
    }
    if (!coord || coord == NULL) {
        std::cout << "%TEST_FAILED% time=0 testname=createcoordianator (mainfunctiontest) message=error message sample" << std::endl;
    }
}

void contest() {

    std::map <std::string, std::string> config;
    config.insert(std::pair<std::string, std::string>("port", "12345"));
    config.insert(std::pair<std::string, std::string>("clients", "12345"));
    config.insert(std::pair<std::string, std::string>("threadscount", "1"));
    config.insert(std::pair<std::string, std::string>("len_block", "1024"));
    sockaddr_in sa;
    sa.sin_family = AF_INET;
    sa.sin_port = htons(std::atoi(config["port"].c_str()));
    ::inet_aton("127.0.0.1", &sa.sin_addr);
    //        ::inet_aton("188.40.41.20", &sa.sin_addr);
    //    std::cout << "address inited" << std::endl;
    int s_test = ::socket(AF_INET, SOCK_STREAM, 0);
    //    std::cout << "socket test:" << s_test << std::endl;
    int res_con = ::connect(s_test, (sockaddr*) & sa, sizeof (sa));
    std::cout << "connect result:" << res_con << std::endl;
    if (res_con < 0) {
        return;
    }
    std::string from_str = "ai93kmbt.0bj3kc gdpsjdheldfjclss";
    std::string to_str = "7Y3RR7WR7BWA7RIARIBhdsadjkdhsajk";
    std::string body_str = "jskdfhjkasdfkjsfkjsfjksdfhkjsdfsdjfsdgfjsdfgsj";
    std::vector<unsigned char> from_data(from_str.begin(), from_str.end());
    std::vector<unsigned char> to_data(to_str.begin(), to_str.end());
    std::vector<unsigned char> body_data(body_str.begin(), body_str.end());
    std::vector<unsigned char> msg(0);
    msg.insert(msg.end(), from_data.begin(), from_data.end());
    msg.insert(msg.end(), to_data.begin(), to_data.end());
    msg.insert(msg.end(), body_data.begin(), body_data.end());
    //    std::cout << "data setted" << std::endl;
    int len_msg_from = from_data.size();
    unsigned char* len_buf = (unsigned char*) &len_msg_from;
    from_data.insert(from_data.begin(), len_buf, len_buf + 4);
    //    std::cout << "len_msg_from sended:" << ::send(s_test, &len_msg_from, sizeof (len_msg_from), 0) << std::endl;
    std::cout << "from_data sended:" << ::send(s_test, &from_data[0], from_data.size(), 0) << std::endl;
    int len_msg_to = to_data.size();
    len_buf = (unsigned char*) &len_msg_to;
    to_data.insert(to_data.begin(), len_buf, len_buf + 4);
    //    std::cout << "len_msg_to sended:" << ::send(s_test, &len_msg_to, sizeof (len_msg_to), 0) << std::endl;
    std::cout << "to_data sended:" << ::send(s_test, &to_data[0], to_data.size(), 0) << std::endl;
    int len_msg_body = msg.size();
    len_buf = (unsigned char*) &len_msg_body;
    msg.insert(msg.begin(), len_buf, len_buf + 4);
    //    std::cout << "len_msg_body sended:" << ::send(s_test, &len_msg_body, sizeof (len_msg_body), 0) << std::endl;
    std::cout << "msg sended:" << ::send(s_test, &msg[0], msg.size(), 0) << std::endl;
    ::vector<unsigned char> ask_data(len_msg_body * 10);
    int recv_len = 0;
    recv_len = ::recv(s_test, &ask_data[0], ask_data.size(), 0);
    std::cout << "ask_data recved:" << recv_len << std::endl;
    ask_data.resize(recv_len);
    ::string ask_str(ask_data.begin(), ask_data.end());
    ::string req_str(msg.begin() + 4, msg.end());
    if (!(req_str == ask_str)) {
        std::cout << "send:" << req_str << std::endl;
        std::cout << "recv:" << ask_str << std::endl;
        std::cout << "%TEST_FAILED% time=0 testname=askrequesttest (mainfunctiontest) message=error message sample" << std::endl;
    }
    std::cout << "close st" << std::endl;
    //    ::close(s_test);
    std::cout << "close fn" << std::endl;

}

void askrequesttest() {
    std::cout << "mainfunctiontest askrequesttest" << std::endl;
    coordinator::coordinator* coord;
    try {
        std::cout << "start test" << std::endl;
        std::map <std::string, std::string> config;
        config.insert(std::pair<std::string, std::string>("port", "12345"));
        config.insert(std::pair<std::string, std::string>("clients", "12345"));
        config.insert(std::pair<std::string, std::string>("threadscount", "10"));
        config.insert(std::pair<std::string, std::string>("len_block", "1024"));
        std::cout << "cofig inited" << std::endl;
        coordinator::coordinator* coord = new coordinator::coordinator(config);
        std::cout << "coordinator created" << std::endl;
//        coord->play();
        sleep(3);
        std::cout << "coordinator played" << std::endl;
        //        sleep(3);
        std::clock_t start = std::clock();
        for (int i = 0; i < 10; i++) {
            contest();
            //                        sleep(1);
        }

        std::cout << "RESULT:" << (std::clock() - start) / (double) (CLOCKS_PER_SEC / 1000) << "s" << std::endl;

        //        sleep(3);
        //        contest();
        //        contest();
        //        contest();
        //        contest();
        //        contest();
        //        contest();
        //        sleep(3);


//        coord->stop();
        std::cout << "coordinator stoped" << std::endl;
    } catch (std::exception &e) {
        std::cout << "%TEST_FAILED% time=0 testname=askrequesttest (mainfunctiontest) message=error message sample" << std::endl;
    }
    if (!coord || coord == NULL) {
        std::cout << "%TEST_FAILED% time=0 testname=askrequesttest (mainfunctiontest) message=error message sample" << std::endl;
    }
}

int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% mainfunctiontest" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    //    std::cout << "%TEST_STARTED% test1 (mainfunctiontest)" << std::endl;
    //    createtest();
    //    std::cout << "%TEST_FINISHED% time=0 test1 (mainfunctiontest)" << std::endl;

    std::cout << "%TEST_STARTED% askrequesttest (mainfunctiontest)\n" << std::endl;
    askrequesttest();
    std::cout << "%TEST_FINISHED% time=0 askrequesttest (mainfunctiontest)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

