#ifndef DEFS_H
#define DEFS_H

#define TARGET_FPS 60
#define FRAME_TIME (1000 / TARGET_FPS)

#define WND_WIDTH 1280
#define WND_HEIGHT 960

#define BOARD_WIDTH 20
#define BOARD_HEIGHT 20
#define CELL_SIZE 35

#define BOARD_OFFSET_X ((WND_WIDTH / 2) - ((BOARD_WIDTH * CELL_SIZE) / 2))
#define BOARD_OFFSET_Y ((WND_HEIGHT / 2) - ((BOARD_HEIGHT * CELL_SIZE) / 2))

typedef enum CELL{
    EMPTY, SNAKE, APPLE
}CELL;

typedef enum DIRECTION{
    UP, RIGHT, DOWN, LEFT
}DIRECTION;

#endif