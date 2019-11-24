#pragma once

// size of the table
const int N = 3; 

// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int CELL_WIDTH = SCREEN_WIDTH / N;
const int CELL_HEIGHT = SCREEN_HEIGHT / N;

// Types of elements in board/table
const int EMPTY = 0;
const int PLAYER_X = 1;
const int PLAYER_O = 2;

// States
const int RUNNING_STATE_FRIEND = 0;
const int RUNNING_STATE_AI = 1;
const int PLAYER_X_WON_STATE = 2;
const int PLAYER_O_WON_STATE = 3;
const int TIE_STATE = 4;
const int MENU_STATE = 5;
const int QUIT_STATE = 6;

// Holds the state of the game
typedef struct {
	int table[N * N];
	int player;
	int state;
} game_t;
