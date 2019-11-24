#pragma once

void click_on_cell(game_t *game, int row, int column);
void reset_game(game_t *game);
void switchPlayer(game_t *game);
int evaluateBoard(const game_t *game);
int mousePress(int x, int y);
