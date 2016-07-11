#ifndef INC_GUI_CONST_H
#define INC_GUI_CONST_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>

//TTF Colors
const SDL_Color TTFBlack = {0x00, 0x00, 0x00};
const SDL_Color TTFDarkGray = {0x40, 0x40, 0x40};
const SDL_Color TTFMidGray = {0x80, 0x80, 0x80};
const SDL_Color TTFLightGray = {0xC0, 0xC0, 0xC0};
const SDL_Color TTFWhite = {0xFF, 0xFF, 0xFF};
const SDL_Color TTFColorDark1 = {0x33, 0x00, 0x00};
const SDL_Color TTFColorMid1 = {0x87, 0x00, 0x00};
const SDL_Color TTFColorLight1 = {0xDB, 0x00, 0x00};
 
//GFX Colors
const Uint32 gfxBlack = 0xFF000000;
const Uint32 gfxMidGray = 0xFF7F7F7F;
const Uint32 gfxLightGray = 0xFFC0C0C0;
const Uint32 gfxColorWhite = 0xFFFFFFFF;
const Uint32 gfxColorDark1 = 0xFF000033;
const Uint32 gfxColorMid1 = 0xFF000087;
const Uint32 gfxColorLight1 = 0xFF0000DB;

//Screen dimension constants
const int INIT_SCREEN_WIDTH = 960;
const int INIT_SCREEN_HEIGHT = 540;


//The base font
extern TTF_Font* myFont;


#endif
