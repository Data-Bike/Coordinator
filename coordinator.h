/* 
 * File:   coordinator.h
 * Author: newworld
 *
 * Created on 5 Август 2015 г., 1:42
 */

#ifndef COORDINATOR_H
#define	COORDINATOR_H
#include <map>
#include <string>
#include <vector>
#include <mutex>
#include <iostream>
#include "socketserver.h"
#include "parser.h"
using namespace std;
namespace coordinator {

    class coordinator : public socketserver::socketserver {
    public:
        coordinator(map <string, string> config);
        coordinator(const coordinator& orig);
        void doRead(int s_recv, vector<unsigned char> data);
        void doClient(sockaddr_in address, int s_client);
        void doSocket(int s_client);
        virtual ~coordinator();
    private:
        map <string, string> config;
        map <int, vector<unsigned char>> sockethashid;
        map <vector<unsigned char>, int> hashidsocket;
        mutex* hash_guard;
    };
}
#endif	/* COORDINATOR_H */

