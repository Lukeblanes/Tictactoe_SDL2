#include <SDL2/SDL.h>

#include "game.h"
#include "logic.h"
#include "minmax.h"

void reset_game(game_t *game)
{
	game->player = PLAYER_O;
	game->state = MENU_STATE;
	for(int i = 0; i < N*N; i++)
		game->table[i] = EMPTY;
}

void switchPlayer(game_t *game)
{
	if(game->player == PLAYER_X)
    	game->player = PLAYER_O;
    else if(game->player == PLAYER_O)
  	  	game->player = PLAYER_X;
}

void click_on_cell(game_t *game, int row, int column) 
{
	if(game->state == RUNNING_STATE_FRIEND)
	{
		int pos = mousePress(row, column);
		if(game->table[pos] == EMPTY)
		{
			game->table[pos] = game->player;
			switchPlayer(game);
	  		game->state = evaluateBoard(game);
	  	}
	}
	else if(game->state == RUNNING_STATE_AI) // TODO
	{
		int pos = mousePress(row, column);
		if(game->table[pos] == EMPTY)
		{
			int oldState = game->state; 
			game->table[pos] = game->player; // update board
			switchPlayer(game);
			game->state = evaluateBoard(game); // see how it's changed
			if(game->state == oldState) // Game is still going on
			{
				int move = minMaxMove(game);
				game->table[move] = game->player; // update board
				switchPlayer(game);
				game->state = evaluateBoard(game); // see how it's changed
			}
	  	}
	}
	else
	{
		reset_game(game);
	}
}

int mousePress(int x, int y) 
{
	if(y < SCREEN_HEIGHT/N && x < SCREEN_WIDTH /N)
		return 0;
	else if(y < SCREEN_HEIGHT/N && x > SCREEN_WIDTH /N && x < (SCREEN_WIDTH/N) *2 )
		return 1;
	else if(y < SCREEN_HEIGHT/N &&  x > (SCREEN_WIDTH/N) *2 )
		return 2;
	else if(y > SCREEN_HEIGHT/N && y < (SCREEN_HEIGHT/N)*2 && x < SCREEN_WIDTH /N)
		return 3;
	else if(y > SCREEN_HEIGHT/N && y < (SCREEN_HEIGHT/N)*2 && x > SCREEN_WIDTH /N && x < (SCREEN_WIDTH/N) *2 )
		return 4;
	else if(y > SCREEN_HEIGHT/N && y < (SCREEN_HEIGHT/N)*2 && x > (SCREEN_WIDTH/N) *2 )
		return 5;
	else if(y > SCREEN_HEIGHT/N && x < SCREEN_WIDTH /N)
		return 6;
	else if(y > SCREEN_HEIGHT/N && x > SCREEN_WIDTH /N && x < (SCREEN_WIDTH/N) *2 )
		return 7;
	else if(y > SCREEN_HEIGHT/N && x > SCREEN_WIDTH /N )
		return 8;
  
}

// Currently not dynamic. If I change N it won't work.
int evaluateBoard(const game_t *game) 
{
	for(int i = 0; i < N; ++i)
	{
		// Check horizontal and vertical
		if( (game->table[0 + 3*i] == PLAYER_X && game->table[1 + 3*i] == PLAYER_X && game->table[2 + 3*i] == PLAYER_X) or 
			(game->table[0 + i] == PLAYER_X && game->table[3 + i] == PLAYER_X && game->table[6+i] == PLAYER_X) )
			return PLAYER_X_WON_STATE;
		else if( (game->table[0 + 3*i] == PLAYER_O && game->table[1 + 3*i] == PLAYER_O && game->table[2 + 3*i] == PLAYER_O )  or 
				 (game->table[0 + i] == PLAYER_O && game->table[3 + i] == PLAYER_O && game->table[6+i] == PLAYER_O))
			return PLAYER_O_WON_STATE;
	}

	// 0-4-8 and 6-4-2 diagonals.
	if( (game->table[0] == PLAYER_X && game->table[4] == PLAYER_X && game->table[8] == PLAYER_X) or 
		(game->table[6] == PLAYER_X && game->table[4] == PLAYER_X && game->table[2] == PLAYER_X) )
		return PLAYER_X_WON_STATE;
	else if( (game->table[0] == PLAYER_O && game->table[4] == PLAYER_O && game->table[8] == PLAYER_O )  or 
			 (game->table[6] == PLAYER_O && game->table[4] == PLAYER_O && game->table[2] == PLAYER_O))
		return PLAYER_O_WON_STATE;

	// Check Tie
	int counter = 0;
	for(int i = 0; i < N*N; ++i)
	{
		if(game->table[i] == EMPTY)
			return game->state;
	}
	return TIE_STATE; // if there are no empty spaces
}