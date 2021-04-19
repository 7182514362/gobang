#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "Global.h"
/* #ifdef _WIN32
#include <windows.h>
#include <winsock2.h>
#elif defined(__linux__)
#include <sys/socket.h>
#include <arpa/inet.h>
#endif */

class Player {
public:
    Player();
    ~Player();
private:
    std::string name;
    // piece type
    GOBANG_PIECE_TYPE type;
};

#endif