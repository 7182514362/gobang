#ifndef CLIENT_H
#define CLIENT_H

#ifdef _WIN32
#include <windows.h>
#include <winsock2.h>
#elif defined(__linux__)
#include <arpa/inet.h>
#include <sys/socket.h>
#endif

class Client {
public:
    Client() = default;
    bool Connect(const char* addr, u_short port);
    bool isConnected();
    int sendData(const char* data, int);
    int sendDataNoBlock(const char*, int);
    int recvData(char*, int);
    int recvDataNoBlock(char*, int);
    // no block io
    bool setNBIO();
    void close();

private:
    SOCKET clientSocket;
    sockaddr_in server_addr;
    bool connected;
    //char *send_buf;
    //char *recv_buf;
};

#endif /* CLIENT_H */