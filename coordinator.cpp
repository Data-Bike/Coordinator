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
            if (this->hashidsocket.find(parsed.from) != this->hashidsocket.end()) {
                std::cout << "ever is hashid in hashidsocket:" << this->hashidsocket[parsed.from] << std::endl;
                this->hashidsocket[parsed.from] = s_recv;
                this->sockethashid[s_recv] = parsed.from;
            }
            this->hashidsocket.insert(pair<vector<unsigned char>, int>(parsed.from, s_recv));
            this->sockethashid.insert(pair<int, vector<unsigned char>>(s_recv, parsed.from));
            this->hash_guard->unlock();
        } else {
            std::cout << "write " << string(data.begin(), data.end()) << std::endl;
            this->write(this->hashidsocket[parsed.to], data);
        }
    }

    void coordinator::doClient(sockaddr_in address, int s_client) {
        socketserver::socketserver::doClient(address, s_client);
    }

    void coordinator::doSocket(int s_client) {
        int *len_data = new int;
        int len_recv = ::recv(s_client, len_data, 4, 0);
        if (len_recv == 4 && len_data>0) {
            std::cout << " init len_data=" << *len_data << " size len_data=" << sizeof (*len_data) << " size int=" << sizeof (int) << std::endl;
            socketserver::socketserver::read(s_client, *len_data);
        } else {
            std::cout << " init recv failed client:" << s_client << std::endl;
        }
    }

    void coordinator::removeClient(int s_client) {
        //        socketserver::socketserver::removeClient(s_client);
        //        //std::cout << "socket removed" << std::endl;
        //        this->hashidsocket.erase(this->hashidsocket.find(this->sockethashid[s_client]));
        //        //std::cout << "hashidsocket removed" << std::endl;
        //        this->sockethashid.erase(this->sockethashid.find(s_client));
        //        //std::cout << "sockethashid removed" << std::endl;

    }

    coordinator::~coordinator() {
    }

}