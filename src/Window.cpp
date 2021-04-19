
#include "Window.h"

#include <functional>
#include <iostream>
#include <sstream>
/* 
Window::Window() {
    //Initialize non-existant window
    mWindow = nullptr;
    mRenderer = nullptr;
    mMouseFocus = false;
    mKeyboardFocus = false;
    mFullScreen = false;
    mMinimized = false;
    mShown= false;
    mWidth = 0;
    mHeight = 0;
} */

Window::Window(int w, int h) : mWindow(nullptr), mRenderer(nullptr), mWindowID(0), mWidth(w),
                               mHeight(h), mMouseFocus(true), mKeyboardFocus(true), mFullScreen(false),
                               mMinimized(false), mShown(true), mMousePos({0, 0}) { init(); }

bool Window::init()
{
    //Create window
    mWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                               mWidth, mHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    if (mWindow == nullptr)
    {
        std::cout << "Failed to create window.\n";
        exit(1);
    }
    mRenderer = SDL_CreateRenderer(
        mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!mRenderer)
    {
        std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    //Initialize renderer color
    SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    //Grab window identifier
    mWindowID = SDL_GetWindowID(mWindow);
    //Flag as opened
    //mShown = true;

    mDrawableList = new std::vector<Drawable *>();

    return true;
}

SDL_Renderer *Window::createRenderer()
{
    if (mRenderer)
        return mRenderer;
    else
        mRenderer = SDL_CreateRenderer(mWindow, -1,
                                       SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    return mRenderer;
}

void Window::handleEvent(SDL_Event &e)
{
    //Window event occured
    if (e.type == SDL_WINDOWEVENT && e.window.windowID == mWindowID)
    {
        //Caption update flag
        bool updateCaption = false;
        switch (e.window.event)
        {
        //Window appeared
        case SDL_WINDOWEVENT_SHOWN:
            mShown = true;
            break;
        //Window disappeared
        case SDL_WINDOWEVENT_HIDDEN:
            mShown = false;
            break;
        //Get new dimensions and repaint
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            mWidth = e.window.data1;
            mHeight = e.window.data2;
            //
            // printf("windows width: %d\nwindow height: %d\n", mWidth, mHeight);
            SDL_RenderPresent(mRenderer);
            break;
        //Repaint on expose
        case SDL_WINDOWEVENT_EXPOSED:
            SDL_RenderPresent(mRenderer);
            break;
        //Mouse enter
        case SDL_WINDOWEVENT_ENTER:
            mMouseFocus = true;
            updateCaption = true;
            break;
        //Mouse exit
        case SDL_WINDOWEVENT_LEAVE:
            mMouseFocus = false;
            updateCaption = true;
            break;
        //Keyboard focus gained
        case SDL_WINDOWEVENT_FOCUS_GAINED:
            mKeyboardFocus = true;
            updateCaption = true;
            break;
        //Keyboard focus lost
        case SDL_WINDOWEVENT_FOCUS_LOST:
            mKeyboardFocus = false;
            updateCaption = true;
            break;
        //Window minimized
        case SDL_WINDOWEVENT_MINIMIZED:
            mMinimized = true;
            break;
        //Window maxized
        case SDL_WINDOWEVENT_MAXIMIZED:
            mMinimized = false;
            break;
        //Window restored
        case SDL_WINDOWEVENT_RESTORED:
            mMinimized = false;
            break;
        //Hide on close
        case SDL_WINDOWEVENT_CLOSE:
            SDL_HideWindow(mWindow);
            break;
        }
        //Update window caption with new data
        if (updateCaption)
        {
            std::stringstream caption;
            caption << "SDL Tutorial - MouseFocus:" << ((mMouseFocus) ? "On" : "Off")
                    << " KeyboardFocus:" << ((mKeyboardFocus) ? "On" : "Off");
            SDL_SetWindowTitle(mWindow, caption.str().c_str());
        }
    }

    SDL_GetMouseState(&mMousePos.x, &mMousePos.y);
    
    if (e.type == SDL_MOUSEMOTION)
    {
        for (auto i : *mDrawableList)
        {
            i->handleMouseMotion(mMousePos);
        }
    }

    if (e.type == SDL_MOUSEBUTTONDOWN)
    {
        for (auto i : *mDrawableList)
        {
            i->handleMouseLBtnDown(mMousePos);
        }
    }
}

void Window::free()
{
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
}

void Window::focus()
{
    //Restore window if needed
    if (!mShown)
    {
        SDL_ShowWindow(mWindow);
    }
    //Move window forward
    SDL_RaiseWindow(mWindow);
}

void Window::render()
{
    if (!mMinimized)
    {
        //Clear screen
        SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(mRenderer);
        for (auto i : *mDrawableList)
        {
            i->draw(mWidth, mHeight);
        }
        //Update screen
        SDL_RenderPresent(mRenderer);
    }
}

double Window::getWindowZoomLevel()
{
    // 获取窗口当前显示的监视器
    // 使用桌面的句柄.
    HWND hWnd = GetDesktopWindow();
    HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);

    // 获取监视器逻辑宽度与高度
    MONITORINFOEX miex;
    miex.cbSize = sizeof(miex);
    GetMonitorInfo(hMonitor, &miex);
    int cxLogical = (miex.rcMonitor.right - miex.rcMonitor.left);
    int cyLogical = (miex.rcMonitor.bottom - miex.rcMonitor.top);

    // 获取监视器物理宽度与高度
    DEVMODE dm;
    dm.dmSize = sizeof(dm);
    dm.dmDriverExtra = 0;
    EnumDisplaySettings(miex.szDevice, ENUM_CURRENT_SETTINGS, &dm);
    int cxPhysical = dm.dmPelsWidth;
    int cyPhysical = dm.dmPelsHeight;
    //std::cout << "cxPhysical: " << cxPhysical << std::endl;
    //std::cout << "cxLogical: " << cxLogical << std::endl;

    double horzScale = ((double)cxPhysical / (double)cxLogical);
    //double vertScale = ((double)cyPhysical / (double)cyLogical);
    return horzScale;
}

void Window::addDrawable(Drawable *aDrawable)
{
    mDrawableList->push_back(aDrawable);
}

int Window::getWidth()
{
    return mWidth;
}

int Window::getHeight()
{
    return mHeight;
}

SDL_Window *Window::getWindow()
{
    return mWindow;
}

SDL_Renderer *Window::getRenderer()
{
    return mRenderer;
}

bool Window::hasMouseFocus()
{
    return mMouseFocus;
}

bool Window::hasKeyboardFocus()
{
    return mKeyboardFocus;
}

bool Window::isMinimized()
{
    return mMinimized;
}

bool Window::isShown()
{
    return mShown;
}