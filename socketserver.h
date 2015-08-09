/* 
 * File:   socketserver.h
 * Author: newworld
 *
 * Created on 5 Август 2015 г., 2:32
 */

#ifndef SOCKETSERVER_H
#define	SOCKETSERVER_H
#include <map>
#include <vector>
#include <string>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <exception>
#include <fcntl.h>
#include <thread>
#include <mutex>
#include <algorithm>
#include <unistd.h> 
#include <sys/select.h>
using namespace std;
namespace socketserver {

    class socketserver {
    public:
        socketserver(map <string, string> config);
        socketserver(const socketserver& orig);
        void play();
        void stop();
        void restart();
        void resetConfig(map <string, string> config);
        void on(string action, function<void(int, vector<unsigned char>) > doing);

        vector<unsigned char> read(int s_recv);
        vector<unsigned char> read(int s_recv, int size);
        void write(int s_send, vector<unsigned char> data);

        virtual void doSocket(int s_client);
        virtual void doRead(int s_recv, vector<unsigned char> data);
        virtual void doClient(sockaddr_in address, int s_client);

        virtual ~socketserver();
    protected:

        virtual void addClient(int s_client);
        virtual void removeClient(int s_client);
    private:
        static void doThread(int id_thread, socketserver* self);
        vector<int> sockets;
        vector<thread*> threads;
        map<int, mutex*> guards;
        map<int, mutex*> write_guards;
        mutex guard_s_server;
        bool started;
        int s_server;
        sockaddr_in s_address;
        int s_port;
        int max_clients;
        int threadscount;
        int max_socket;
        int len_block;
        function<void(int, vector<unsigned char>) > onRead = [](int s, vector<unsigned char> d)->void {

        };
        function<void(int, vector<unsigned char>) > onClient = [](int s, vector<unsigned char> d)->void {

        };
    };
}
#endif	/* SOCKETSERVER_H */

