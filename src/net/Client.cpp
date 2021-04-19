
#include "Client.h"

// Client::Client() {
//     //send_buf = new char[100];
//     //recv_buf = new char[100];
// }

bool Client::Connect(const char* addr, u_short port) {
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        return false;
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.S_un.S_addr = inet_addr(addr);
    if (connect(clientSocket, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        return false;
    }
    connected = true;
    return true;
}

int Client::sendData(const char* data, int size) {
    if (!connected) {
        return -1;
    }
    int nSent = 0;
    nSent = send(clientSocket, data, size, 0);
    if (nSent < 0) {
        return -1;
    }

    return size;
}

int Client::sendDataNoBlock(const char* data, int size) {
    if (!connected) {
        return -1;
    }
    int ret = send(clientSocket, data, size, 0);
    if (ret == SOCKET_ERROR) {
        int err = WSAGetLastError();
        if (err == WSAEWOULDBLOCK) {
            //Sleep(100);
            return 0;
        } else {
            return -1;
        }
    }
    return ret;
}

int Client::recvData(char* buf, int buf_size) {
    if (!buf || buf_size <= 0 || !connected) {
        return -1;
    }
    int num = recv(clientSocket, buf, buf_size, 0);
    if (num > 0) {
        buf[num] = '\0';
    } else {
        return -1;
    }
    return num;
}

int Client::recvDataNoBlock(char* buf, int buf_size) {
    if (!buf || buf_size <= 0 || !connected) {
        return -1;
    }
    int ret = recv(clientSocket, buf, buf_size, 0);
    if (ret == SOCKET_ERROR) {
        int err = WSAGetLastError();
        if (err == WSAEWOULDBLOCK) {
            //Sleep(100);
            return 0;
        } else {
            return -1;
        }
    }
    return ret;
}

bool Client::setNBIO() {
    u_long mode = 1;
    int ret = ioctlsocket(clientSocket, FIONBIO, &mode);
    if (ret != NO_ERROR) {
        return false;
    }
    return true;
}

bool Client::isConnected() {
    return connected;
}

void Client::close() {
    closesocket(clientSocket);
}