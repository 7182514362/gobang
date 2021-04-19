
#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <SDL2/SDL.h>

#include "Global.h"
#include "Window.h"
#include "Texture.h"

class BoardData;

/**
 * @brief Board 表示棋盘纹理
 * 包含棋盘背景和网格
 */
class Board : public Drawable
{
public:
    Board(SDL_Renderer *, BoardData *);
    ~Board();
    void init();

    void draw(int w, int h) override;

    void handleMouseLBtnDown(SDL_Point) override;
    void handleMouseRBtnDown(SDL_Point) override;
    void handleMouseMotion(SDL_Point) override;

private:
    const int MARGIN;       //65
    const int GAP;          //55
    const int PIECE_RADIUS; //20
    BoardData *mData;
    SDL_Renderer *mRenderer;
    Texture *mBackground;
    Texture *mPieceWhite;
    Texture *mPieceBlack;
    Texture *mCursor;

    SDL_Point mMousePos;

    //std::vector<Texture *> *mTextureList;
};

/**
 * @brief BoardData store game data.
 * BoardData store data which represent current state of a on-going gobang game.
 */
class BoardData
{
public:
    BoardData();
    ~BoardData();

    struct GridPos
    {
        int row;
        int col;
    };

    void setState(int, int, GOBANG_PIECE_TYPE);
    GOBANG_PIECE_TYPE getState(int, int) const;

    void setNextTurn(GOBANG_PIECE_TYPE);
    GOBANG_PIECE_TYPE getNextTurn() const;

    void setLastGridPos(int r, int c);
    GridPos getLastGridPos() const;

    // who wins
    GOBANG_PIECE_TYPE judge();

private:
    /// next turn
    GOBANG_PIECE_TYPE nextTurn;
    GOBANG_PIECE_TYPE lastTurn;
    GridPos lastGridPos;
    /// Board state
    GOBANG_PIECE_TYPE mState[BOARD_GRID_LINES_W][BOARD_GRID_LINES_H];
};

#endif