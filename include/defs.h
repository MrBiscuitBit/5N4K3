#ifndef DEFS_H
#define DEFS_H

#define TARGET_FPS 60
#define FRAME_TIME (1000 / TARGET_FPS)

#define WND_WIDTH 1280
#define WND_HEIGHT 960

#define BOARD_WIDTH 16
#define BOARD_HEIGHT 16
#define CELL_SIZE 35

#define BOARD_OFFSET_X ((WND_WIDTH / 2) - ((BOARD_WIDTH * CELL_SIZE) / 2))
#define BOARD_OFFSET_Y ((WND_HEIGHT / 2) - ((BOARD_HEIGHT * CELL_SIZE) / 2))

#define MAX_STATES 5

typedef enum CELL{
    EMPTY, SNAKE, APPLE
}CELL;

typedef enum DIRECTION{
    UP, RIGHT, DOWN, LEFT
}DIRECTION;

typedef enum GAMESTATES{
    STATE_MAIN_MENU,
    STATE_GAME_PLAYING,
    STATE_COUNT
}GAMESTATES;

typedef enum GAMESTATEACTIONS{
    STATE_ACTION_NONE,
    STATE_ACTION_PUSH,
    STATE_ACTION_POP,
    STATE_ACTION_SWITCH
}GAMESTATEACTIONS;

#endif