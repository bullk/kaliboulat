#ifndef INC_GUI_H
#define INC_GUI_H

//#include <SDL2/SDL.h>
//#include <SDL2/SDL_image.h>
//#include <SDL2/SDL2_gfxPrimitives.h>
//#include <SDL2/SDL_ttf.h>
#include "GUI_Clip.h"

//The window we'll be rendering to
extern SDL_Window * gWindow;

//The window renderer
extern SDL_Renderer * gRenderer;

extern GUI_AudioClip * myGUI_Clip;

//Starts up SDL and creates window
bool GUI_init();

//Loads media
bool GUI_loadMedia();

//Frees media and shuts down SDL
void GUI_close();

void GUI_main();


#endif
