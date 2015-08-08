/* 
 * File:   socketserver.cpp
 * Author: newworld
 * 
 * Created on 5 Август 2015 г., 2:32
 */

#include <iostream>

#include "socketserver.h"
namespace socketserver {

    socketserver::socketserver(map <string, string> config) {
        this->s_port = stoi(config["port"]);
        this->max_clients = stoi(config["clients"]);
        this->threadscount = stoi(config["threadscount"]);
        this->len_block = stoi(config["len_block"]);
        this->s_server = socket(PF_INET, SOCK_STREAM, 0);
        std::cout << this->s_server << std::endl;
        this->max_socket = this->s_server;
        this->s_address.sin_family = PF_INET;
        this->s_address.sin_port = htons(this->s_port);
        this->s_address.sin_addr.s_addr = INADDR_ANY;
        if (::bind(this->s_server, (sockaddr*) & this->s_address, sizeof (this->s_address)) != 0) {
            std::cout << "bind failed" << std::endl;
        } else {
            std::cout << "bind ok" << std::endl;
        }
    }

    socketserver::socketserver(const socketserver& orig) {
    }

    void socketserver::play() {
        this->started = true;
        socketserver * self = this;
        if (listen(this->s_server, this->max_clients) != 0) {
            std::cout << "listen failed" << std::endl;
            throw (new exception());
        } else {
            std::cout << "listen ok" << std::endl;
        }
        for (int id_thread = 0; id_thread<this->threadscount; id_thread++) {
            std::cout << "started 'this' socket server is:" << this->s_server << std::endl;
            std::cout << "started 'self' socket server is:" << self->s_server << std::endl;
            thread* th = new thread(this->doThread, id_thread, self);
            this->threads.insert(this->threads.end(), th);
            std::cout << "started thread ok:" << id_thread << "/" << this->threadscount - 1 << std::endl;
        }

    }

    void socketserver::stop() {
        this->started = false;
        std::cout << "socketserver::stop()" << std::endl;
        for (thread* &th : this->threads) {
            th->join();
        }
        ::close(this->s_server);

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
        while ((len_recv = ::recv(s_recv, &buf[0], buf.size(), 0)) > 0) {
            buf.resize(len_recv);
            recv_data.insert(recv_data.end(), buf.begin(), buf.end());
        }
        std::cout << "read ok:" << recv_data.size() << std::endl;
        this->onRead(s_recv, recv_data);
        this->doRead(s_recv, recv_data);
        return recv_data;
    }

    vector<unsigned char> socketserver::read(int s_recv, int size) {
        vector<unsigned char> recv_data(size);
        int len_recv = 0;
        if (::recv(s_recv, &recv_data[0], size, 0) == size) {
            std::cout << "read ok:" << recv_data.size() << std::endl;
        }
        this->onRead(s_recv, recv_data);
        this->doRead(s_recv, recv_data);
        return recv_data;
    }

    void socketserver::write(int s_send, vector<unsigned char> data) {
        this->write_guards[s_send]->lock();
        ::send(s_send, &data[0], data.size(), 0);
        this->write_guards[s_send]->unlock();
    }

    void socketserver::doSocket(int s_client) {
        this->read(s_client);
    }

    void socketserver::doRead(int s_recv, vector<unsigned char> data) {

    }

    void socketserver::doThread(int id_thread, socketserver* self) {
        std::cout << "doThread init" << self->s_server << std::endl;
        fd_set readset, writeset;
        sockaddr_in c_addr;
        socklen_t lenaddr = sizeof (c_addr);
        std::cout << "doThread init ok" << std::endl;

        while (self->started) {
            timeval timeout;
            timeout.tv_sec = 1;
            timeout.tv_usec = 0;
            std::cout << "doThread circle" << std::endl;
            FD_ZERO(&readset);
            FD_ZERO(&writeset);
            FD_SET(self->s_server, &readset);
            if (self->sockets.size() > 0) {
                for (int client : self->sockets) {
                    std::cout << "client to set:" << client << std::endl;
                    FD_SET(client, &readset);
                }
            }
            std::cout << "doThread circle set ok" << std::endl;
            int mx = 0;
            std::cout << "doThread circle start set mx" << std::endl;
            if (self->sockets.size() > 0) {
                std::cout << "doThread circle self->sockets.size() > 0" << std::endl;
                mx = std::max(self->s_server, *std::max_element(self->sockets.begin(), self->sockets.end()));
            } else {
                std::cout << "doThread circle self->sockets.size() > 0 else" << std::endl;
                mx = self->s_server;
            }
            std::cout << "doThread circle max ok:" << mx << std::endl;
            std::cout << "doThread circle FD_SETSIZE ok:" << FD_SETSIZE << std::endl;
            if (::select(mx + 1, &readset, NULL, NULL, &timeout) < 1) {
                std::cout << "::select(FD_SETSIZE, &readset, NULL, NULL, &timeout)<1" << std::endl;
                continue;
            };
            std::cout << "doThread circle select ok" << std::endl;
            if (FD_ISSET(self->s_server, &readset) && self->guard_s_server.try_lock()) {
                std::cout << "accepted client" << std::endl;
                int s_client = ::accept(self->s_server, (sockaddr*) & c_addr, &lenaddr);
                std::cout << "accepted client:" << s_client << std::endl;
                fcntl(s_client, F_SETFL, O_NONBLOCK);
                self->doClient(c_addr, s_client);
                self->guard_s_server.unlock();
            }
            std::cout << "doThread isset server ok" << std::endl;
            for (int client : self->sockets) {
                std::cout << "doThread circle to test client:" << client << std::endl;
                if (FD_ISSET(client, &readset) && self->guards[client]->try_lock()) {
                    std::cout << "doThread circle to doSocket client:" << client << std::endl;
                    self->doSocket(client);
                    self->guards[client]->unlock();
                }
            }
            std::cout << "doThread circle ok" << std::endl;
        }

    }

    void socketserver::doClient(sockaddr_in address, int s_client) {
        this->sockets.insert(this->sockets.end(), s_client);
        mutex *buf_m = new mutex();
        std::pair<int, mutex*>buf(s_client, (mutex*) buf_m);

        this->guards.insert(this->guards.end(), buf);
        mutex *buf_m_w = new mutex();
        std::pair<int, mutex*>buf_w(s_client, (mutex*) buf_m_w);
        this->write_guards.insert(this->guards.end(), buf_w);
    }

    socketserver::~socketserver() {
    }
}