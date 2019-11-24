#include <math.h>
#include <SDL2/SDL.h>

#include "game.h"
#include "rendering.h"

const SDL_Color GRID_COLOR = { .r = 255, .g = 255, .b = 255};
const SDL_Color PLAYER_X_COLOR = { .r = 255, .g = 0, .b = 0};
const SDL_Color PLAYER_O_COLOR = { .r = 0, .g = 0, .b = 255};
const SDL_Color TIE_COLOR = { .r = 100, .g = 100, .b = 100};

void render_board(SDL_Renderer *gRenderer, const int *table, const SDL_Color *player_x_color, const SDL_Color *player_o_color)
{
	for(int i = 0; i < N; i++)
	{
		for(int j = 0; j < N; j++)
		{
			switch(table[i * N +j])
			{
				case PLAYER_X:
					render_X(gRenderer, i, j, player_x_color);
					break;

				case PLAYER_O:
					render_O(gRenderer, j, i, player_o_color);
					break;
			}
		}
	}
}

void render_grid(SDL_Renderer *gRenderer, const SDL_Color *color)
{
	SDL_SetRenderDrawColor( gRenderer, color->r, color->g, color->b, 255 );	
	for(int i = 1; i < N; ++i)
	{
		SDL_RenderDrawLine( gRenderer, 0, CELL_HEIGHT * i, SCREEN_WIDTH, CELL_HEIGHT * i);
		SDL_RenderDrawLine( gRenderer, CELL_WIDTH * i, 0, CELL_WIDTH * i, SCREEN_HEIGHT);	
	}		
}

void render_running_state(SDL_Renderer *gRenderer, const game_t *game)
{
	render_grid(gRenderer, &GRID_COLOR);
	render_board(gRenderer, game->table, &PLAYER_X_COLOR, &PLAYER_O_COLOR);
}

void render_game_over_state(SDL_Renderer *gRenderer,const game_t *game, const SDL_Color *color)
{
	render_grid(gRenderer, color);
	render_board(gRenderer, game->table, color, color);
}

void render_game(SDL_Renderer *renderer, const game_t *game) // const because game will not be modified
{
	switch(game->state) 
	{
		case RUNNING_STATE_FRIEND:
		case RUNNING_STATE_AI:
			render_running_state(renderer, game);
			break;

		case PLAYER_X_WON_STATE:
			render_game_over_state(renderer, game, &PLAYER_X_COLOR);
			break;

		case PLAYER_O_WON_STATE:
			render_game_over_state(renderer, game, &PLAYER_O_COLOR);
			break;

		case TIE_STATE:
			render_game_over_state(renderer, game, &TIE_COLOR);
			break;
	}
}

void render_X(SDL_Renderer *gRenderer, int row, int column, const SDL_Color *color)
{
	const float half_box_side = fmin(CELL_WIDTH, CELL_HEIGHT) * 0.25;
	const float center_x = CELL_WIDTH * 0.5 + column * CELL_WIDTH;
	const float center_y = CELL_HEIGHT * 0.5 + row * CELL_HEIGHT;

	SDL_SetRenderDrawColor( gRenderer, color->r, color->g, color->b, 255 );		
	SDL_RenderDrawLine( gRenderer, center_x - half_box_side,
							 	   center_y - half_box_side,
							 	   center_x + half_box_side,
							 	   center_y + half_box_side);
	SDL_RenderDrawLine( gRenderer, center_x + half_box_side,
							 	   center_y - half_box_side,
							 	   center_x - half_box_side,
							 	   center_y + half_box_side);

}

void render_O(SDL_Renderer *gRenderer, int x, int y, const SDL_Color *color)
{
	int radius = (CELL_HEIGHT/3);
	SDL_SetRenderDrawColor( gRenderer, color->r, color->g, color->b, 0 );
	for(int w= 0; w< radius *2; ++w)
	{
		for(int h = 0; h < radius * 2; ++h)
		{
			int dx = radius - w;
			int dy = radius - h;
			if ((dx*dx + dy*dy) <= (radius*radius))
			{
				SDL_RenderDrawPoint(gRenderer, dx + ( CELL_WIDTH*x ) + SCREEN_WIDTH/6, dy + ( CELL_HEIGHT*y ) + SCREEN_HEIGHT/6 );
			}
		}
	}
	radius = (CELL_HEIGHT/5); // Inner circle not rendering correctly
	SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 255 );
	for(int w= 0; w< radius *2; ++w)
	{
		for(int h = 0; h < radius * 2; ++h)
		{
			int dx = radius - w;
			int dy = radius - h;
			if ((dx*dx + dy*dy) <= (radius*radius))
			{
				SDL_RenderDrawPoint(gRenderer, dx + ( CELL_WIDTH*x ) + SCREEN_WIDTH/6, dy + ( CELL_HEIGHT*y ) + SCREEN_HEIGHT/6 );
			}
		}
	}
}