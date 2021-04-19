#ifndef WINDOW_H
#define WINDOW_H

#include <vector>
#include <SDL2/SDL.h>
#include <windows.h>
#include <functional>
#include "Texture.h"

class Drawable;

class Window
{
public:
    //Intializes internals
    Window() = delete;
    Window(int, int);
    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;
    //Creates window
    bool init();
    //Creates renderer from internal window
    SDL_Renderer *createRenderer();
    //Handles window events
    void handleEvent(SDL_Event &e);
    void focus();
    void render();
    void free();

    void addDrawable(Drawable *);

    //Window dimensions
    int getWidth();
    int getHeight();
    SDL_Window *getWindow();
    SDL_Renderer *getRenderer();

    bool hasMouseFocus();
    bool hasKeyboardFocus();
    bool isMinimized();
    bool isShown();

    double getWindowZoomLevel();

private:
    //Window data
    SDL_Window *mWindow;
    SDL_Renderer *mRenderer;
    int mWindowID;
    //Window dimensions
    int mWidth;
    int mHeight;
    //Window focus
    bool mMouseFocus;
    bool mKeyboardFocus;
    bool mFullScreen;
    bool mMinimized;
    bool mShown;

    SDL_Point mMousePos;

    std::vector<Drawable *> *mDrawableList;
};

class Drawable
{
public:
    Drawable() = default;
    ~Drawable() = default;

    virtual void draw(int,int) = 0;
    virtual void handleMouseLBtnDown(SDL_Point) = 0;
    virtual void handleMouseRBtnDown(SDL_Point) = 0;
    virtual void handleMouseMotion(SDL_Point) = 0;

    // top left of the drawable texture
    int mPosX;
    int mPosY;

private:
};

#endif