/**
 * @file Global.h
 * @brief 简介
 * @details 细节
 * @mainpage 工程概览
 * @author 作者
 * @email 邮箱
 * @version 版本号
 * @date 年-月-日
 * @license 版权
 */

#ifndef GLOBAL_H
#define GLOBAL_H

#define BOARD_GRID_LINES (15)
#define BOARD_GRID_LINES_H (15)
#define BOARD_GRID_LINES_W (15)

// Screen dimension constants
const int DEFAULT_SCREEN_WIDTH = 1000;
const int DEFAULT_SCREEN_HEIGHT = 1000;
const int GOBANG_PIECE_W = 54;
const int GOBANG_PIECE_H = 54;
const int LINE_INTERVAL = 60;
const int OFFSET = (LINE_INTERVAL - GOBANG_PIECE_W) / 2;
const int BOARD_WIDTH = LINE_INTERVAL * 14;
const int BOARD_HEIGHT = BOARD_WIDTH;
/// Gobang piece type
enum class GOBANG_PIECE_TYPE {
    BLANK = 0,
    BLACK = 1,
    WHITE = 2
};

#endif