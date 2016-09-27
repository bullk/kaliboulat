#ifndef INC_MYGUI_H
#define INC_MYGUI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"

#include "Project.hpp"
#include "Listener.hpp"


#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))


struct Screen
{
	enum Type { TITLE, PROJECT, CONSOLE, SEQUENCER };
	enum Context { NONE, RESSOURCES, AUDIOCLIP, MIDICLIP }; 
	Type type;
	Context context;
	//unsigned int audioclip, midiclip;
	AudioFile * dragged_audio_file;
	//Clip * selected_clip;
	//Track * selected_track;
	
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
