#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

#include "game.h"
#include "logic.h"
#include "rendering.h"

// Function declarations.
bool init(); // Starts up SDL and creates window
bool loadMedia(); // Loads media
void close(); // Frees media and shuts down SDL
SDL_Texture* loadTexture( std::string path ); // Loads individual image as texture

SDL_Window* gWindow = NULL; // The window we'll be rendering to
SDL_Renderer* gRenderer = NULL; // The window renderer
SDL_Texture* gTexture = NULL; // Current displayed texture

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia( std::string path )
{
	//Loading success flag
	bool success = true;

	//Load texture
	gTexture = loadTexture( path );
	if( gTexture == NULL )
	{
		printf( "Failed to load playAI image!\n" );
		success = false;
	}

	//Nothing to load
	return success;
}

void close()
{
	//Free loaded image
	SDL_DestroyTexture( gTexture );
	gTexture = NULL;

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture( std::string path )
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return newTexture;
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		game_t game = {
			.table = {EMPTY, EMPTY, EMPTY, 
					  EMPTY, EMPTY, EMPTY, 
					  EMPTY, EMPTY, EMPTY},
			.player = PLAYER_O,
			.state = MENU_STATE
		};
		//Main loop flag
		bool quit = false;

		//Event handler
		SDL_Event e;

		//While application is running
		while( game.state != QUIT_STATE )
		{
			//Handle events on queue
			while( SDL_PollEvent( &e ) != 0 )
			{
				switch(e.type) {
				case SDL_QUIT: // User requests quit
					game.state = QUIT_STATE;
					break;
					
				case SDL_MOUSEBUTTONDOWN:
					if(game.state != MENU_STATE)
						click_on_cell(&game, e.button.x, e.button.y);
					else if(game.state == MENU_STATE && e.button.y < SCREEN_HEIGHT/2)
						game.state = RUNNING_STATE_FRIEND;
					else if(game.state == MENU_STATE && e.button.y > SCREEN_HEIGHT/2)
						game.state = RUNNING_STATE_AI;
        			break;
				}
			}	
				
			if (game.state == MENU_STATE)
			{
				//Clear screen
	            SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, 0 );
	            SDL_RenderClear( gRenderer );	

	            if( !loadMedia("res/PlayFriend.png"))
	            {
	            	printf( "Unable to load texture from! SDL Error: %s\n", SDL_GetError() );	
	            }
	            else
	            {
	            	//Top viewport
					SDL_Rect topViewport;
					topViewport.x = 0;
					topViewport.y = 0;
					topViewport.w = SCREEN_WIDTH;
					topViewport.h = SCREEN_HEIGHT / 2;
					SDL_RenderSetViewport( gRenderer, &topViewport );
					SDL_RenderCopy( gRenderer, gTexture, NULL, NULL );
	            }

	            if( !loadMedia("res/PlayAI.png"))
	            {
	            	printf( "Unable to load texture from! SDL Error: %s\n", SDL_GetError() );	
	            }
	            else
	            {
	            	//Bottom viewport
					SDL_Rect bottomViewport;
					bottomViewport.x = 0;
					bottomViewport.y = SCREEN_HEIGHT / 2;
					bottomViewport.w = SCREEN_WIDTH;
					bottomViewport.h = SCREEN_HEIGHT / 2;
					SDL_RenderSetViewport( gRenderer, &bottomViewport );
					SDL_RenderCopy( gRenderer, gTexture, NULL, NULL );
	            }

				//Update screen
				SDL_RenderPresent( gRenderer );

				SDL_Rect resetViewport;
				resetViewport.x = 0;
				resetViewport.y = 0;
				resetViewport.w = SCREEN_WIDTH;
				resetViewport.h = SCREEN_HEIGHT;
				SDL_RenderSetViewport( gRenderer, &resetViewport );
				
			}
			else if(game.state != MENU_STATE)
			{
				//Clear screen
	            SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 255 );
	            SDL_RenderClear( gRenderer );	
				//Clear screen
				render_game(gRenderer, &game);

				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}