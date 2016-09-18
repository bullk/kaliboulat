#ifndef INC_MYGUI_H
#define INC_MYGUI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"

#include "Clock.hpp"
#include "Modules.hpp"
#include "MidiTrack.hpp"
#include "Project.hpp"
#include "MidiFile.hpp"


#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))


struct Screen
{
	enum Type { TITLE, RESSOURCES, CONSOLE, SEQUENCER };
	enum Context { NONE, PROJECT, AUDIOCLIP, MIDICLIP }; 
	Type type;
	Context context;
	unsigned int audioclip, midiclip;
};

class GUI
{
	public:
	
	protected:

};

int GUI_Init();
void GUI_Close();
void GUI_Main(Project * project);


#endif
