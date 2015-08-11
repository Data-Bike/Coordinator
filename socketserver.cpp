/* 
 * File:   socketserver.cpp
 * Author: newworld
 * 
 * Created on 5 Август 2015 г., 2:32
 */

#include <iostream>
#include <bits/algorithmfwd.h>

#include "socketserver.h"
namespace socketserver {

    socketserver::socketserver(map <string, string> config) {
        this->s_port = stoi(config["port"]);
        this->max_clients = stoi(config["clients"]);
        this->threadscount = stoi(config["threadscount"]);
        this->len_block = stoi(config["len_block"]);
        this->sockets = vector<int>(0);
        this->s_server = socket(PF_INET, SOCK_STREAM, 0);
        //std::cout << this->s_server << std::endl;
        this->max_socket = this->s_server;
        this->s_address.sin_family = PF_INET;
        this->s_address.sin_port = htons(this->s_port);
        this->s_address.sin_addr.s_addr = INADDR_ANY;
        if (::bind(this->s_server, (sockaddr*) & this->s_address, sizeof (this->s_address)) != 0) {
            //std::cout << "bind failed" << std::endl;
            std::terminate();
        } else {
            //std::cout << "bind ok" << std::endl;
        }
    }

    socketserver::socketserver(const socketserver& orig) {
    }

    void socketserver::play() {
        this->started = true;
        if (listen(this->s_server, this->max_clients) != 0) {
            //std::cout << "listen failed" << std::endl;
            throw (new exception());
            return;
        } else {
            //std::cout << "listen ok" << std::endl;
        }

        socketserver * self = this;
        for (int id_thread = 0; id_thread<this->threadscount; id_thread++) {
            //std::cout << "started 'this' socket server is:" << this->s_server << std::endl;
            //std::cout << "started 'self' socket server is:" << this->s_server << std::endl;
            thread* th = new thread(&socketserver::doThread, this, id_thread);
            this->threads.insert(this->threads.end(), th);
            //std::cout << "started thread ok:" << id_thread << "/" << this->threadscount - 1 << std::endl;
        }

    }

    void socketserver::stop() {
        this->started = false;
        //std::cout << "socketserver::stop()" << std::endl;
        for (thread* &th : this->threads) {
            th->join();
        }
        //std::cout << "all threads join to main" << std::endl;
        ::close(this->s_server);
        //std::cout << "close server socket" << std::endl;

    }

    void socketserver::restart() {

    }

    void socketserver::on(string action, function<void(int, vector<unsigned char>) > doing) {
        if (action == "read") {
            this->onRead = doing;
        } else if (action == "client") {
            this->onClient = doing;
        }
    }

    void socketserver::resetConfig(map <string, string> config) {

    }

    vector<unsigned char> socketserver::read(int s_recv) {
        vector<unsigned char> buf(this->len_block);
        vector<unsigned char> recv_data(0);
        int len_recv = 0;
        std::cout << "read params buf.size()=" << buf.size() << " this->len_block=" << this->len_block << std::endl;
        while ((len_recv = ::recv(s_recv, &buf[0], buf.size(), 0)) > 0) {
            buf.resize(len_recv);
            recv_data.insert(recv_data.end(), buf.begin(), buf.end());
        }
        if (len_recv < 0 && recv_data.size() == 0) {
            std::cout << "read failed remove client:" << s_recv << std::endl;
            std::cout << "read failed remove client:" << s_recv << " recv_data.size()=" << recv_data.size() << std::endl;
            this->removeClient(s_recv);
            return recv_data;
        }
        std::cout << "read ok:" << recv_data.size() << std::endl;
        this->onRead(s_recv, recv_data);
        this->doRead(s_recv, recv_data);
        return recv_data;
    }

    vector<unsigned char> socketserver::read(int s_recv, int size) {
        vector<unsigned char> recv_data(size);
        int len_recv = 0;
        if ((len_recv = ::recv(s_recv, &recv_data[0], size, 0)) == size) {
            std::cout << "read ok:" << recv_data.size() << std::endl;
        } else {
            std::cout << "read fix size failed client:" << s_recv << " recv_size=" << len_recv << " size=" << size << std::endl;
            this->removeClient(s_recv);
            return recv_data;
        }
        this->onRead(s_recv, recv_data);
        this->doRead(s_recv, recv_data);
        return recv_data;
    }

    void socketserver::write(int s_send, vector<unsigned char> data) {
        std::cout << "write to notlock:" << s_send << std::endl;
        this->write_guards[s_send]->lock();
        std::cout << "write to:" << s_send << std::endl;
        ::send(s_send, &data[0], data.size(), 0);
        this->write_guards[s_send]->unlock();
    }

    void socketserver::doSocket(int s_client) {
        this->read(s_client);
    }

    void socketserver::doRead(int s_recv, vector<unsigned char> data) {

    }

    void socketserver::doThread(int id_thread) {
        std::cout << "doThread init" << this->s_server << std::endl;
        fd_set readset, writeset, errorset;
        sockaddr_in c_addr;
        socklen_t lenaddr = sizeof (c_addr);
        std::cout << "doThread init ok" << std::endl;
        std::cout << "doThread started" << this->started << std::endl;

        while (this->started) {
            timeval timeout;
            timeout.tv_sec = 1;
            timeout.tv_usec = 0;
            std::cout << "doThread circle" << std::endl;
            FD_ZERO(&readset);
            FD_ZERO(&writeset);
            FD_ZERO(&errorset);
            FD_SET(this->s_server, &readset);
            if (this->sockets.size() > 0) {
                for (int client : this->sockets) {
                    std::cout << "client to set:" << client << std::endl;
                    FD_SET(client, &readset);
                    FD_SET(client, &writeset);
                    FD_SET(client, &errorset);
                }
            }
            std::cout << "doThread circle set ok" << std::endl;
            int mx = 0;
            std::cout << "doThread circle start set mx" << std::endl;
            if (this->sockets.size() > 0) {
                std::cout << "doThread circle this->sockets.size() > 0" << std::endl;
                mx = std::max(this->s_server, *std::max_element(this->sockets.begin(), this->sockets.end()));
            } else {
                std::cout << "doThread circle this->sockets.size() > 0 else" << std::endl;
                mx = this->s_server;
            }
            std::cout << "doThread circle max ok:" << mx << std::endl;
            std::cout << "doThread circle FD_SETSIZE ok:" << FD_SETSIZE << std::endl;
            if (::select(mx + 1, &readset, &writeset, &errorset, &timeout) < 0) {
                std::cout << "::select(FD_SETSIZE, &readset, NULL, NULL, &timeout)<1" << std::endl;
                                continue;
            };
            std::cout << "doThread circle select ok" << std::endl;
            if (FD_ISSET(this->s_server, &readset) && this->guard_s_server.try_lock()) {
                std::cout << "accepted client" << std::endl;
                int s_client = ::accept(this->s_server, (sockaddr*) & c_addr, &lenaddr);
                std::cout << "accepted client:" << s_client << std::endl;
                fcntl(s_client, F_SETFL, O_NONBLOCK);
                this->doClient(c_addr, s_client);
                this->guard_s_server.unlock();
            }
            std::cout << "doThread isset server ok" << std::endl;
            for (int client : this->sockets) {
                std::cout << "doThread circle to test client:" << client << std::endl;
                if (FD_ISSET(client, &readset) && FD_ISSET(client, &writeset) && !FD_ISSET(client, &errorset) && this->guards[client]->try_lock()) {
                    std::cout << "doThread circle to doSocket client:" << client << " thid:" << id_thread << std::endl;
                    this->doSocket(client);
                    this->guards[client]->unlock();
                } else if (!FD_ISSET(client, &writeset) || FD_ISSET(client, &errorset)) {
                    std::cout << "removeClient" << std::endl;
                    this->removeClient(client);
                }
            }
            //std::cout << "doThread circle ok" << std::endl;
        }

    }

    void socketserver::doClient(sockaddr_in address, int s_client) {
        this->addClient(s_client);
    }

    void socketserver::addClient(int s_client) {
        this->sockets.insert(this->sockets.end(), s_client);
        mutex *buf_m = new mutex();
        std::pair<int, mutex*>buf(s_client, (mutex*) buf_m);

        this->guards.insert(this->guards.end(), buf);
        mutex *buf_m_w = new mutex();
        std::pair<int, mutex*>buf_w(s_client, (mutex*) buf_m_w);
        this->write_guards.insert(this->guards.end(), buf_w);

    }

    void socketserver::removeClient(int s_client) {
        this->guard_s_server.lock();
        this->sockets.erase(remove(this->sockets.begin(), this->sockets.end(), s_client), this->sockets.end());
        this->guards.erase(s_client);
        this->write_guards.erase(s_client);
//        ::close(s_client);
        this->guard_s_server.unlock();
    }

    socketserver::~socketserver() {
    }
}