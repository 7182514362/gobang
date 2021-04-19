/*

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <stdio.h>

#include <functional>
#include <sstream>
#include <iostream>
#include <vector>

#include <windows.h>

#include "Global.h"
#include "Texture.h"
#include "Board.h"
#include "Timer.h"
#include "Window.h"


// current mouse position
SDL_Point mousePos = {0, 0};
// board grid
SDL_Point board_column[30];
SDL_Point board_row[30];

typedef struct
{
    GOBANG_PIECE_TYPE type;
    SDL_Point pos;
} Piece;
// game progress
Piece piece_graph[15][15];

Window *gWindow = nullptr;
SDL_Renderer *gRenderer = nullptr;
Texture *gBackground = nullptr;
Texture *gSpriteSheetTexture = nullptr;
SDL_Rect spriteRect[2] = {{0, 0, GOBANG_PIECE_W, GOBANG_PIECE_H},
                          {GOBANG_PIECE_W, 0, GOBANG_PIECE_W, GOBANG_PIECE_H}};

double getWindowZoomLevel()
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
    std::cout << "cxPhysical: " << cxPhysical << std::endl;
    std::cout << "cxLogical: " << cxLogical << std::endl;

    // 缩放比例计算  实际上使用任何一个即可
    double horzScale = ((double)cxPhysical / (double)cxLogical);
    //double vertScale = ((double)cyPhysical / (double)cyLogical);
    return horzScale;
}

bool init()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
    {
        printf("Warning: Linear texture filtering not enabled!");
    }
    // Create window
    gWindow = new Window();
    if (!gWindow || !gWindow->init(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT))
    {
        return false;
    }
    int imgFlags = IMG_INIT_PNG; //|IMG_INIT_JPG
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        printf("SDL_image could not initialize! SDL_image Error: %s\n",
               IMG_GetError());
        return false;
    }
    gRenderer = gWindow->getRenderer();

    // set scale
    float zoomLevel = getWindowZoomLevel();
    std::cout << "zoomLevel: " << zoomLevel << std::endl;
    SDL_RenderSetScale(gRenderer, 1 / zoomLevel, 1 / zoomLevel);

    gSpriteSheetTexture = new Texture(gRenderer);
    gBackground = new Texture(gRenderer);
    return true;
}

bool loadMedia()
{
    bool success = true;
    if (!gSpriteSheetTexture ||
        !gSpriteSheetTexture->loadFromFile("../resource/gobang_piece.png"))
    {
        printf("Unable to load gobang texture!\n");
        success = false;
    }
    if (!gBackground ||
        !gBackground->loadFromFile("../resource/background.png"))
    {
        printf("Unable to load background texture!\n");
        success = false;
    }
    return success;
}

void close()
{
    // Free loaded images
    gSpriteSheetTexture->free();
    gBackground->free();
    // Destroy window
    gWindow->free();

    // Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

void inline setPoint(int x, int y, SDL_Point *point)
{
    point->x = x;
    point->y = y;
}

// draw board grid
void drawBoard(int w, int h, int offset_x, int offset_y)
{
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    setPoint(offset_x, offset_y, &board_column[0]);
    setPoint(offset_x, offset_y + BOARD_HEIGHT, &board_column[1]);
    setPoint(offset_x, offset_y, &board_row[0]);
    setPoint(offset_x + BOARD_WIDTH, offset_y, &board_row[1]);

    for (int i = 0; i < 14; i++)
    {
        SDL_RenderDrawLines(gRenderer, &board_column[i << 1], 2);
        SDL_RenderDrawLines(gRenderer, &board_row[i << 1], 2);

        setPoint(board_column[i << 1].x + LINE_INTERVAL, board_column[0].y,
                 &board_column[(i << 1) + 2]);
        setPoint(board_column[(i << 1) + 2].x, board_column[1].y,
                 &board_column[(i << 1) + 3]);
        setPoint(board_row[0].x, board_row[i << 1].y + LINE_INTERVAL,
                 &board_row[(i << 1) + 2]);
        setPoint(board_row[1].x, board_row[(i << 1) + 2].y,
                 &board_row[(i << 1) + 3]);
    }
    SDL_RenderDrawLines(gRenderer, &board_column[28], 2);
    SDL_RenderDrawLines(gRenderer, &board_row[28], 2);
}

void inline drawPiece(int x, int y, GOBANG_PIECE_TYPE type)
{
    if (type == BLACK)
        gSpriteSheetTexture->render(x, y, &spriteRect[1]);
    else
        gSpriteSheetTexture->render(x, y, &spriteRect[0]);
}

void addToGraph(int x, int y, GOBANG_PIECE_TYPE type, int column, int row)
{
    if (piece_graph[row][column].type == BLANK)
    {
        setPoint(x, y, &piece_graph[row][column].pos);
        piece_graph[row][column].type = type;
    }
}

void updateGraph(int w, int h, int offset_x, int offset_y)
{
    int x = -GOBANG_PIECE_W / 2 + offset_x;
    int y = -GOBANG_PIECE_H / 2 + offset_y;
    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            if (piece_graph[i][j].type != BLANK)
            {
                setPoint(x, y, &piece_graph[i][j].pos);
            }
            x += LINE_INTERVAL;
        }
        y += LINE_INTERVAL;
        x = -GOBANG_PIECE_W / 2 + offset_x;
    }
}
// draw all pieces
void drawPieces()
{
    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            if (piece_graph[i][j].type != BLANK)
            {
                drawPiece(piece_graph[i][j].pos.x, piece_graph[i][j].pos.y,
                          piece_graph[i][j].type);
            }
        }
    }
}

void handleMouseEvent(SDL_Event &e, SDL_Point &spawn_point, int &column,
                      int &row)
{
    if (e.type == SDL_MOUSEBUTTONDOWN)
    {
        SDL_GetMouseState(&mousePos.x, &mousePos.y);

        int spawn_point_x = 0, spawn_point_y = 0;
        int x = mousePos.x;
        int y = mousePos.y;
        int lower_bound = -LINE_INTERVAL / 2;
        int upper_bound = 0;
        int left_bound = -LINE_INTERVAL / 2;
        int right_bound = 0;
        // search the nearest spawn point to the current mouse position
        for (int i = 0; i < 30; i += 2)
        {
            if (x < board_column[i].x)
            {
                right_bound = board_column[i].x;
                column = i / 2;
                break;
            }
            else
            {
                left_bound = board_column[i].x;
            }
        }
        for (int i = 0; i < 30; i += 2)
        {
            if (y < board_row[i].y)
            {
                upper_bound = board_row[i].y;
                row = i / 2;
                break;
            }
            else
            {
                lower_bound = board_row[i].y;
            }
        }
        if (lower_bound > upper_bound || left_bound > right_bound)
        {
            // printf("\n");
            return;
        }
        int middle_v = (left_bound + right_bound) / 2;
        int middle_h = (upper_bound + lower_bound) / 2;

        if (x >= middle_v)
        {
            spawn_point_x = right_bound;
        }
        else
        {
            spawn_point_x = left_bound;
            --column;
        }
        if (y >= middle_h)
        {
            spawn_point_y = upper_bound;
        }
        else
        {
            spawn_point_y = lower_bound;
            --row;
        }
        spawn_point.x = spawn_point_x - GOBANG_PIECE_W / 2;
        spawn_point.y = spawn_point_y - GOBANG_PIECE_H / 2;
    }
}

int main(int argc, char **argv)
{
    bool quit = false;
    SDL_Event e;

    // Start up SDL and create window
    if (!init())
    {
        printf("Failed to initialize!\n");
        return -1;
    }
    else
    {
        // Load media
        if (!loadMedia())
        {
            printf("Failed to load media!\n");
            return -1;
        }
    }
    // windows width & height
    int w = 0;
    int h = 0;
    // board grid render offset
    int offset_x = 0;
    int offset_y = 0;
    // graph index
    int column = 0, row = 0;
    SDL_Point spawnPoint = {0, 0};

    Board board(gRenderer, nullptr);

    while (!quit)
    {
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0)
        {
            // User requests quit
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            gWindow->handleEvent(e);
            //handleMouseEvent(e, spawnPoint, column, row);
            //addToGraph(spawnPoint.x, spawnPoint.y, WHITE, column, row);

            w = gWindow->getWidth();
            h = gWindow->getHeight();
            offset_x = (w - BOARD_WIDTH) / 2;
            offset_y = (h - BOARD_HEIGHT) / 2;

            board.setPos((w - 900) / 2, (h - 900) / 2);

            // updateGraph(w, h, offset_x, offset_y);
            // Only draw when not minimized
            if (!gWindow->isMinimized())
            {
                SDL_SetRenderDrawColor(gRenderer, 0xF0, 0xF0, 0xF0, 0xFF);
                SDL_RenderClear(gRenderer);
                board.render();
                
        //gBackground->render((w - gBackground->getWidth()) / 2,(h - gBackground->getHeight()) / 2);
                            
                //drawBoard(w, h, offset_x, offset_y);
                //drawPieces();
                // Update screen
                SDL_RenderPresent(gRenderer);
            }
        }
    }

    return 0;
}

*/

/////////////////////////////////////// Gobang ////////////////////////////////////

#include "Gobang.h"
#include <iostream>

Gobang::Gobang() : mWindow(nullptr), mRenderer(nullptr), mBoard(nullptr), mBoardData(nullptr)
{
    init();
}

bool Gobang::init()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
    {
        printf("Warning: Linear texture filtering not enabled!");
        return false;
    }
    // Create window
    mWindow = new Window(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
    if (!mWindow)
    {
        return false;
    }
    int imgFlags = IMG_INIT_PNG; //|IMG_INIT_JPG
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }
    mRenderer = mWindow->getRenderer();

    // set scale
    float zoomLevel = mWindow->getWindowZoomLevel();
    if (zoomLevel < 0.1)
    {
        printf("zoomLevel==0");
        exit(1);
    }
    //std::cout << "zoomLevel: " << zoomLevel << std::endl;

    //SDL_RenderSetScale(mRenderer, 1 / zoomLevel, 1 / zoomLevel);

    mBoardData = new BoardData();
    mBoard = new Board(mRenderer, mBoardData);

    mWindow->addDrawable(mBoard);

    return true;
}

void Gobang::loop()
{
    bool quit = false;
    SDL_Event e;

    while (!quit)
    {
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0)
        {
            // User requests quit
            if (e.type == SDL_QUIT)
            {
                quit = true;
                break;
            }
            mWindow->handleEvent(e);
            mWindow->render();
        }
    }

    // free SDL_Window SDL_Texture
    mWindow->free();
}

void Gobang::start()
{
    loop();
}