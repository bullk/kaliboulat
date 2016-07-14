#include <stdio.h>
#include <cmath>

//#include <SDL2/SDL.h>
//#include <SDL2/SDL_image.h>
//#include <SDL2/SDL2_gfxPrimitives.h>
//#include <SDL2/SDL_ttf.h>

#include "globals.h"
//#include "GUI.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>


using namespace stk;
using namespace std;


//Audio Rendering
int tick();
int GUI_Init();
void GUI_Close();

//----------------------------------------------------------------------
// AUDIO FUNCTIONS
//----------------------------------------------------------------------

int tick( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
		 double streamTime, RtAudioStreamStatus status, void *dataPointer )
{
	AudioGroup * audiogroup = (AudioGroup *) dataPointer;
	register StkFloat *samples = (StkFloat *) outputBuffer;

	for ( unsigned int i=0; i<nBufferFrames; i++ )
	{
		*samples = 0;
		for ( unsigned int j = 0; j < audiogroup->getClipSet()->size(); j++)
			*samples += audiogroup->getClipSet()->at(j)->tick();
		samples++;
	}
	
	return 0;
}


void audioInit (void)
{
	sampleLs[0] = "bar.wav";
	sampleLs[1] = "hellosine.wav";

	Stk::setSampleRate (44100.0);
	Stk::showWarnings (true);
	
} 

//----------------------------------------------------------------------
// GUI
//----------------------------------------------------------------------

SDL_GLContext glcontext;
SDL_Window *window = NULL;
ImVec4 clear_color;

int GUI_Init ()
{
    // Setup SDL
    //if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0)
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // Setup window
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_DisplayMode current;
    SDL_GetCurrentDisplayMode(0, &current);
    //window = SDL_CreateWindow("kaliboulat", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
    window = SDL_CreateWindow("kaliboulat", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL|SDL_WINDOW_FULLSCREEN_DESKTOP);
    SDL_GLContext glcontext = SDL_GL_CreateContext(window);

    // Setup ImGui binding
    ImGui_ImplSdl_Init(window);

    clear_color = ImColor(114, 144, 154);

    return 0;
}

void GUI_Close()
{
    // Cleanup
    ImGui_ImplSdl_Shutdown();
    SDL_GL_DeleteContext(glcontext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

//----------------------------------------------------------------------

int main( int argc, char* args[] )
{

    if (GUI_Init() != 0)	return -1;
        
	audioInit();

	// Figure out how many bytes in an StkFloat and setup the RtAudio stream.
	RtAudio::StreamParameters parameters;
	parameters.deviceId = dac.getDefaultOutputDevice();
	parameters.nChannels = 1;
	RtAudioFormat format = ( sizeof(StkFloat) == 8 ) ? RTAUDIO_FLOAT64 : RTAUDIO_FLOAT32;
	//RtAudioFormat format = RTAUDIO_SINT16;

	unsigned int bufferFrames = RT_BUFFER_SIZE;

	try {
		dac.openStream( &parameters, NULL, format, (unsigned int)Stk::sampleRate(), &bufferFrames, &tick, (void *)&audioMaster );
	}
	catch ( RtError &error ) {
		error.printMessage();
		//goto cleanup;
	}

	try {
		dac.startStream();
	}
	catch ( RtError &error ) {
		error.printMessage();
		//goto cleanup;
	}

	audioMaster.addAclip (sampleDir + "/" + sampleLs[0]);

    // Main loop
    bool done = false;
    while (!done)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSdl_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
        }
        ImGui_ImplSdl_NewFrame(window);

        // 1. Show a simple window
        // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
        {
            ImGui::Begin("Audio Clips");
            ImGui::Text("Hello");
			// Use functions to generate output
			// FIXME: This is rather awkward because current plot API only pass in indices. We probably want an API passing floats and user provide sample rate/count.
			struct Funcs
			{
				static float Sin(void*, int i) { return sinf(i * 0.1f); }
				static float Saw(void*, int i) { return (i & 1) ? 1.0f : 0.0f; }
			};
			static int func_type = 0, display_count = 70;
			ImGui::Separator();
			ImGui::PushItemWidth(100); ImGui::Combo("func", &func_type, "Sin\0Saw\0"); ImGui::PopItemWidth();
			ImGui::SameLine();
			ImGui::SliderInt("Sample count", &display_count, 1, 500);
			float (*func)(void*, int) = (func_type == 0) ? Funcs::Sin : Funcs::Saw;
			ImGui::PlotLines("Lines", func, NULL, display_count, 0, NULL, -1.0f, 1.0f, ImVec2(0,80));
			ImGui::PlotHistogram("Histogram", func, NULL, display_count, 0, NULL, -1.0f, 1.0f, ImVec2(0,80));
			ImGui::Separator();
            ImGui::End();
        }

        // Rendering
        glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui::Render();
        SDL_GL_SwapWindow(window);
    }


	// Shut down the output stream.
	try {
		dac.closeStream();
	}
	catch ( RtError &error ) {
		error.printMessage();
	}

	cleanup:
		GUI_Close();
		return 0; /* ISO C requires main to return int. */
}


