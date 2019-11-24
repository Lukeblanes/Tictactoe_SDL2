#pragma once

#include <utility>

void switchP(game_t *game);
int minMaxMove(const game_t *game);
std::pair<int,int> minimax(game_t *game, bool maximisePlayer);
bool isGameOver(const game_t *game);
int returnAnyEmpty(const game_t *game);
int evaluateCurrentBoard(const game_t *game);
void displayGame(const game_t *game);
