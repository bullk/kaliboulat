#include "GUI.h"

SDL_Window * gWindow = NULL;
SDL_Renderer * gRenderer = NULL;
GUI_AudioClip * myGUI_Clip = NULL;
TTF_Font* myFont = NULL;

//----------------------------------------------------------------------
// SDL FUNCTIONS
//----------------------------------------------------------------------

bool GUI_init()
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
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "2" ) )
		{
			printf( "Warning: Anisotropic texture filtering not enabled!" );
			if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )	printf( "Warning: Linear texture filtering not enabled!" );
		}
		
		//Create window
		//gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, INIT_SCREEN_WIDTH, INIT_SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			//gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );

				//Initialize PNG loading
				//int imgFlags = IMG_INIT_PNG;
				//if( !( IMG_Init( imgFlags ) & imgFlags ) )
				//{
					//printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					//success = false;
				//}

				 //Initialize SDL_ttf
				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool GUI_loadMedia()
{
	//Loading success flag
	myFont = TTF_OpenFont("./FreeSans.ttf", 24); //this opens a font style and sets a size
	if( myFont == NULL ) printf( "Failed to load FreeSans font! SDL_ttf Error: %s\n", TTF_GetError() );

	
	bool success = true;

	//Nothing to load
	return success;
}

void GUI_close()
{
	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

void GUI_main()
{
	//Start up SDL and create window
	if( !GUI_init() ) printf( "Failed to initialize!\n" );
	else
	{
		//Load media
		if( !GUI_loadMedia() ) printf( "Failed to load media!\n" );
		else
		{	
			
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					switch( e.type )
					{
						case SDL_QUIT:
							quit = true;
							break;
						case SDL_KEYDOWN: /* Si appui sur une touche */
							quit = true;
							break;
					}
				}
				
				//Clear screen to black
				SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
				SDL_RenderClear( gRenderer );
								
				myGUI_Clip->display( gRenderer );
				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	GUI_close();	
}
