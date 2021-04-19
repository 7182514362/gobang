#include "Client.h"

#include <iostream>
using namespace std;

//char send_buf[128];
#define BUF_SIZE 128

int main(int argc, char** argv) {
    WORD sockVersion = MAKEWORD(2, 2);
    WSADATA data;
    if (WSAStartup(sockVersion, &data) != 0) {
        cout << "init failed!" << endl;
        return 0;
    }
    Client client;
    if (!client.Connect("192.168.195.129", 2222)) {
        cout << "connect failed!" << endl;
        return 0;
    }
    if (!client.setNBIO()) {
        printf("setNBIO failed!\n");
    }
    char recv_buf[BUF_SIZE];
    const char* msg = "test msg";
    int count = 0;
    while (1) {
        if (client.sendDataNoBlock(msg, strlen(msg)) < 0) {
            cout << "send data error!" << endl;
            break;
        }
        //cout << "send success!" << endl;
        Sleep(1000);
        int ret = client.recvDataNoBlock(recv_buf, BUF_SIZE);
        if (ret < 0) {
            cout << "recv data error!" << endl;
            break;
        } else if (ret > 0) {
            recv_buf[ret] = '\0';
            cout << recv_buf << endl;
        }
    }
    int err = WSAGetLastError();
    cout << err << endl;
    client.close();
    WSACleanup();
    return 0;
}