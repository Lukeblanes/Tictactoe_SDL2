#pragma once

void render_game(SDL_Renderer *renderer, const game_t *game);
void render_game_over_state(SDL_Renderer *gRenderer,const game_t *game, const SDL_Color *color);
void render_running_state(SDL_Renderer *renderer, const game_t *game);
void render_grid(SDL_Renderer *gRenderer, const SDL_Color *color);
void render_board(SDL_Renderer *gRenderer, const int *board, const SDL_Color *player_x_color, const SDL_Color *player_o_color);
void render_X(SDL_Renderer *gRenderer, int x, int y, const SDL_Color *color);
void render_O(SDL_Renderer *gRenderer, int x, int y, const SDL_Color *color);
void drawBoard(SDL_Renderer *gRenderer, const game_t *game);
