/* 
 * File:   coordinator.cpp
 * Author: newworld
 * 
 * Created on 5 Август 2015 г., 1:42
 */

#include "coordinator.h"

namespace coordinator {

    coordinator::coordinator(map <string, string> config) : socketserver(config) {
        this->hash_guard = new mutex();
    }

    coordinator::coordinator(const coordinator& orig) : socketserver(orig.config) {
    }

    void coordinator::doRead(int s_recv, vector<unsigned char> data) {
        parser parsed(data);
        if (parsed.to.size() == 0) {
            this->hash_guard->lock();
            this->hashidsocket.insert(pair<vector<unsigned char>, int>(parsed.from, s_recv));
            this->sockethashid.insert(pair<int, vector<unsigned char>>(s_recv, parsed.from));
            this->hash_guard->unlock();
        } else {
            this->write(this->hashidsocket[parsed.to], data);
        }
    }

    void coordinator::doClient(sockaddr_in address, int s_client) {
        socketserver::socketserver::doClient(address, s_client);
    }

    void coordinator::doSocket(int s_client) {
        int *len_data = new int;
        ::recv(s_client, len_data, sizeof (*len_data), 0);
        socketserver::socketserver::read(s_client, *len_data);
    }

    coordinator::~coordinator() {
    }

}