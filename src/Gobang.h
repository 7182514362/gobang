#ifndef GOBANG_H
#define GOBANG_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Window.h"
#include "Global.h"
#include "Board.h"

class Gobang
{
public:
    Gobang();
    ~Gobang() = default;
    bool init();
    void loop();
    void start();

private:
    Window *mWindow;
    SDL_Renderer *mRenderer;
    Board *mBoard;
    BoardData *mBoardData;
};

#endif