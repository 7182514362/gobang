#include "Board.h"
#include <iostream>

////////////////////////////// Board //////////////////////////////

Board::Board(SDL_Renderer *r, BoardData *d) : mRenderer(r),
                                              mBackground(new Texture(r)),
                                              mPieceWhite(new Texture(r)),
                                              mPieceBlack(new Texture(r)),
                                              mCursor(new Texture(r)),
                                              mData(d), MARGIN(65), GAP(55), PIECE_RADIUS(20)
{
    init();
}

Board::~Board()
{
    delete mBackground;
    delete mPieceWhite;
    delete mPieceBlack;
}

void Board::init()
{
    mBackground->loadFromFile("../resource/background.png");
    mPieceWhite->loadFromFile("../resource/gobang_piece_white.png");
    mPieceBlack->loadFromFile("../resource/gobang_piece_black.png");
    mCursor->loadFromFile("../resource/cursor.png");
}

void Board::draw(int w, int h)
{
    mPosX = (w - mBackground->w) / 2;
    mPosY = (h - mBackground->h) / 2;

    //draw board background
    mBackground->dstRect.x = mPosX;
    mBackground->dstRect.y = mPosY;

    SDL_RenderCopy(mRenderer, mBackground->mTexture, NULL, &mBackground->dstRect);

    // draw board grid
    // MARGIN 65 , GAP 55
    SDL_SetRenderDrawColor(mRenderer, 0x0, 0x0, 0x0, 0xFF);
    int startX = mPosX + MARGIN;
    int startY = mPosY + MARGIN;
    // initialized with left most vertical line
    SDL_Point lineV[2] = {{startX, startY}, {startX, startY + 14 * GAP}};
    // topmost horizontal line
    SDL_Point lineH[2] = {{startX, startY}, {startX + 14 * GAP, startY}};
    for (int i = 0; i < 15; ++i)
    {
        SDL_RenderDrawLines(mRenderer, lineV, 2);
        SDL_RenderDrawLines(mRenderer, lineH, 2);
        lineV[0].x += GAP;
        lineV[1].x += GAP;
        lineH[0].y += GAP;
        lineH[1].y += GAP;
    }

    // draw pieces
    for (int i = 0; i < BOARD_GRID_LINES; ++i)
    {
        for (int j = 0; j < BOARD_GRID_LINES; ++j)
        {
            if (mData->getState(i, j) == GOBANG_PIECE_TYPE::WHITE)
            {
                mPieceWhite->dstRect.x = startX + j * GAP - PIECE_RADIUS;
                mPieceWhite->dstRect.y = startY + i * GAP - PIECE_RADIUS;
                SDL_RenderCopy(mRenderer, mPieceWhite->mTexture, NULL, &mPieceWhite->dstRect);
            }
            else if (mData->getState(i, j) == GOBANG_PIECE_TYPE::BLACK)
            {
                mPieceBlack->dstRect.x = startX + j * GAP - PIECE_RADIUS;
                mPieceBlack->dstRect.y = startY + i * GAP - PIECE_RADIUS;
                SDL_RenderCopy(mRenderer, mPieceBlack->mTexture, NULL, &mPieceBlack->dstRect);
            }
        }
    }

    //draw cursor
    mCursor->dstRect.x = mMousePos.x - 10;
    mCursor->dstRect.y = mMousePos.y - 10;
    mCursor->setAlpha(0x3F);
    SDL_RenderCopy(mRenderer, mCursor->mTexture, NULL, &mCursor->dstRect);
}

void Board::handleMouseLBtnDown(SDL_Point aPoint)
{
    int x = aPoint.x - mPosX - MARGIN;
    int y = aPoint.y - mPosY - MARGIN;

    if(x<0 || y<0){
        return;
    }

    int r = y / GAP;
    int c = x / GAP;

    int offsetX = x % GAP;
    int offsetY = y % GAP;

    if (offsetX > (GAP / 3) && offsetX < (GAP / 3) * 2)
    {
        return;
    }
    if (offsetY > (GAP / 3) && offsetY < (GAP / 3) * 2)
    {
        return;
    }

    if (offsetX > (GAP >> 2))
    {
        c += 1;
    }

    if (offsetY > (GAP >> 2))
    {
        r += 1;
    }
    //std::cout << "(" << r << "," << c << ")\n";

    mData->setState(r, c, GOBANG_PIECE_TYPE::BLACK);
}

void Board::handleMouseRBtnDown(SDL_Point aPoint)
{
}

void Board::handleMouseMotion(SDL_Point aPos)
{
    mMousePos.x = aPos.x;
    mMousePos.y = aPos.y;
}

////////////////////////////// BoardData //////////////////////////////

BoardData::BoardData() : nextTurn(GOBANG_PIECE_TYPE::BLANK), lastGridPos({0, 0})
{
    memset(mState, 0, BOARD_GRID_LINES_W * BOARD_GRID_LINES_H * sizeof(int));
}

void BoardData::setState(int r, int c, GOBANG_PIECE_TYPE t)
{
    mState[r][c] = t;
}

GOBANG_PIECE_TYPE BoardData::getState(int r, int c) const
{
    return mState[r][c];
}

void BoardData::setNextTurn(GOBANG_PIECE_TYPE turn)
{
    lastTurn = nextTurn;
    nextTurn = turn;
}

GOBANG_PIECE_TYPE BoardData::getNextTurn() const
{
    return nextTurn;
}

void BoardData::setLastGridPos(int r, int c)
{
    lastGridPos.row = r;
    lastGridPos.col = c;
}

BoardData::GridPos BoardData::getLastGridPos() const
{
    return lastGridPos;
}

GOBANG_PIECE_TYPE BoardData::judge()
{
    // count for 4 directions
    int count[4] = {0};
    GOBANG_PIECE_TYPE flag = lastTurn;
    int r = lastGridPos.row;
    int c = lastGridPos.col;

    // 2 left, 3 right, 4 up, 5 down, 6 upper left, 7 lower right, 8 lower left, 9 upper right
    int direction = 2;
    while (direction <= 10)
    {
        if (r >= 0 && r < BOARD_GRID_LINES && c >= 0 && c < BOARD_GRID_LINES && mState[r][c] == flag)
        {
            ++count[direction / 2];
        }
        else
        {
            ++direction;
            r = lastGridPos.row;
            c = lastGridPos.col;
        }

        switch (direction)
        {
        case 2:
            --c;
            break;
        case 3:
            ++c;
            break;
        case 4:
            ++r;
            break;
        case 5:
            --r;
            break;
        case 6:
            ++r;
            --c;
            break;
        case 7:
            --r;
            ++c;
            break;
        case 8:
            --r;
            --c;
            break;
        case 9:
            ++r;
            ++c;
            break;
        default:
            std::cerr << "unkown direction\n";
            exit(1);
            break;
        }
    }

    for (int i = 0; i < 4; ++i)
    {
        if (count[i] >= 5)
        {
            return flag;
        }
    }
    return GOBANG_PIECE_TYPE::BLANK;
}