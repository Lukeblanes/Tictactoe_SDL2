#include "game.h"
#include "logic.h"
#include "minmax.h"

int returnAnyEmpty(const game_t *game) 
{
	for(int i = 0; i < N*N; ++i)
	{
		if(game->table[i] == EMPTY)
			return i;
	}
	return -1;
}

// Entry point for minimax algorithm
int minMaxMove(const game_t *game)
{	
	game_t newGame = *game;
	// The pair is to store (value, position)
	std::pair<int,int> result = minimax(&newGame, true); 

	if(result.second != -1)
		return result.second;
	else
		return returnAnyEmpty(game);
}

int getValueState(int currentState) // X is alway AI
{
	switch ( currentState )
	{
		case PLAYER_X_WON_STATE:
			return 100;
		case PLAYER_O_WON_STATE:
			return -100;
		default:
			return 0;
	}	
}

std::pair<int,int> minimax(game_t *game, bool maximisePlayer) 
{
	std::pair<int,int> bestMoveScore;
	std::pair<int,int> currentScore; // we will only want the score from this pair
	game_t newG;
	
	int currentState = evaluateBoard(game);
	if(currentState == PLAYER_O_WON_STATE || currentState == PLAYER_X_WON_STATE || currentState == TIE_STATE) // game is finished
	{
		int valueResult = getValueState(currentState);
		bestMoveScore = std::make_pair(valueResult, -1); // It doesn't matter what we put as the position so I put -1
		return bestMoveScore;
	}

	if(maximisePlayer)
	{	
		bestMoveScore = std::make_pair(-1000, -1);

		for(int i = 0; i < N*N; ++i)
		{
			if(game->table[i] == EMPTY)
			{
				newG = *game;
				newG.table[i] = PLAYER_X; // AI is always X
				currentScore = minimax(&newG, false);
				if(currentScore.first > bestMoveScore.first)
				{
					bestMoveScore.first = currentScore.first;
					bestMoveScore.second = i;
				}
			}
		}
		return bestMoveScore;
	}
	else
	{
		bestMoveScore = std::make_pair(1000, -1);
		for(int i = 0; i < N*N; ++i)
		{
			if(game->table[i] == EMPTY)
			{
				newG = *game;
				newG.table[i] = PLAYER_O;
				currentScore = minimax(&newG, true);
				if(currentScore.first < bestMoveScore.first)
				{
					bestMoveScore.first = currentScore.first;
					bestMoveScore.second = i;
				}
			}
		}
		return bestMoveScore;
	}
}
