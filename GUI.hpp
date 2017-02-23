#ifndef INC_MYGUI_H
#define INC_MYGUI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"

#include "globals.h"
#include "Engine.hpp"

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))


class RessourceFile;
class Project;
class Clip;

struct Screen
{
	enum Type { TITLE, PROJECT, CONSOLE, SEQUENCER, ORGASAMPLE };
	enum Context { NONE, RESSOURCES, AUDIOCLIP, MIDICLIP }; 
	Type type;
	Context context;
	RessourceFile * dragged_file;
	//std::shared_ptr<Clip> dragged_clip;
	
	struct source_clip {
		DataType dt;
		std::string path;
		int track;
	} dragged_clip;
};

class GUI
{
	public:
	
	protected:

};

int GUI_Init();
void GUI_Close();
void GUI_Main(std::shared_ptr<Project> project);


#endif
