#ifndef INC_MYGUI_H
#define INC_MYGUI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "imgui.h"
#include "imgui_impl_sdl.h"

#include "Clock.h"
#include "AudioGroup.h"
#include "MidiGroup.h"


#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

int GUI_Init();
void GUI_Close();
void GUI_Main(bool*, Clock*, AudioGroup*, MidiGroup*);


#endif
