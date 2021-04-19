#ifndef SERVER_H
#define SERVER_H


#ifdef _WIN32
#include <windows.h>
#include <winsock2.h>
#elif defined(__linux__)
#include <arpa/inet.h>
#include <sys/socket.h>
#endif

class Server {
public:
private:
    SOCKET serverSocket;
};


#endif /* SERVER_H */