#ifndef INC_MYGUI_H
#define INC_MYGUI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"

#include "globals.h"
#include "Engine.hpp"

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

#define CLIP_SATURATION 0.8f
#define TRACK_SATURATION 0.5f
#define TRACK_VALUE 0.25f
#define ACTIVE_TRACK_SATURATION 0.6f
#define ACTIVE_TRACK_VALUE 0.5f


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
