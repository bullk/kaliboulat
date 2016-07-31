#include <stdio.h>
#include <string>
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
void audioInit();

//GUI
int GUI_Init();
void GUI_Close();

//----------------------------------------------------------------------
// AUDIO FUNCTIONS
//----------------------------------------------------------------------

int tick( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
		 double streamTime, RtAudioStreamStatus status, void *dataPointer )
{
	AudioGroup * audiogroup = (AudioGroup *) dataPointer;
	register StkFloat * samples = (StkFloat *) outputBuffer;

	for ( unsigned int i=0; i<nBufferFrames; i++ )
	{
		*samples = 0;
		for ( unsigned int j = 0; j < audiogroup->getClipSet()->size(); j++)
			if ( audiogroup->getClipSet()->at(j)->getState() == CS_PLAYING )
				*samples += audiogroup->getClipSet()->at(j)->tick() * *(audiogroup->getClipSet()->at(j)->getVolume());
		samples++;
	}
	
	return 0;
}


void audioInit (void)
{
	sampleLs[0] = "bar.wav";
	sampleLs[1] = "hellosine.wav";

	Stk::setSampleRate (GLOBAL_SAMPLE_RATE);
	Stk::showWarnings (true);
	
} 

void startClock (void)
{
	try {
		dac.startStream();
		mcState = true;
	}
	catch ( RtError &error ) {
		error.printMessage();
		//goto cleanup;
	}
}
	
void stopClock (void)
{
	try {
		dac.stopStream();
		mcState = false;
	}
	catch ( RtError &error ) {
		error.printMessage();
		//goto cleanup;
	}
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
	static int details = 0;
		
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

	audioMaster.addAclip (sampleDir + "/" + sampleLs[0]);
	audioMaster.addAclip (sampleDir + "/" + sampleLs[1]);

	// Main loop
	bool go_on = true;
	while (go_on)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSdl_ProcessEvent(&event);
			if (event.type == SDL_QUIT)
				go_on = false;
		}
		ImGui_ImplSdl_NewFrame(window);

		// 1. Show a simple window
		// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
		{
			ImGuiWindowFlags flags = 0;
			//flags |= ImGuiWindowFlags_NoTitleBar;
			flags |= ImGuiWindowFlags_NoResize;
			flags |= ImGuiWindowFlags_NoMove;
			flags |= ImGuiWindowFlags_NoCollapse;
			//flags |= ImGuiWindowFlags_MenuBar;
			//ImGui::SetNextWindowSize(ImVec2(1920,1080));
			ImGui::SetNextWindowSize(ImVec2((int)ImGui::GetIO().DisplaySize.x,(int)ImGui::GetIO().DisplaySize.y));
			ImGui::Begin("Clips", &go_on, flags);
			
            ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
            ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.00f));

            ImGui::BeginChild("Master Controls", ImVec2(0, 50), true);
			// Clock based on RtAudio
			double clockd = dac.getStreamTime();
			int is = (int) clockd;
			int im = is / 60;
			int h = im / 60;
			int m = im % 60;
			int s = is % 60;
			int ds = (int) (clockd * 10) - (is * 10);
			ImGui::TextColored(ImColor(255,255,0), "%02d:%02d:%02d.%d", h, m, s, ds);
			
            //TODO : Intégrer les images de commandes
			//ImGui::Button("STOP")
			if ( mcState )
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(1/7.0f, 0.6f, 0.6f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(1/7.0f, 0.7f, 0.7f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(1/7.0f, 0.8f, 0.8f));
				if (ImGui::Button("STOP")) { stopClock(); }
				ImGui::PopStyleColor(3);
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(2/7.0f, 0.6f, 0.6f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(2/7.0f, 0.7f, 0.7f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(2/7.0f, 0.8f, 0.8f));
				if (ImGui::Button("PLAY")) { startClock(); }
				ImGui::PopStyleColor(3);
			}
			ImGui::EndChild();
			
			// Audio Clips
            //ImGui::BeginChild("Audio Clips", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
            ImGui::BeginChild("Audio Clips", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 0), true);
			float progress = 0.0f;
			for ( unsigned int i = 0; i < audioMaster.getClipSet()->size(); i++ )
			{
				AudioClip * daClip = audioMaster.getClipSet()->at(i);
				if ( daClip->getState() == CS_PLAYING )
				{
					ImGui::PushID(i);
					ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(1/7.0f, 0.6f, 0.6f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(1/7.0f, 0.7f, 0.7f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(1/7.0f, 0.8f, 0.8f));
					if (ImGui::Button("STOP")) { daClip->setState(CS_STOPPED); }
					ImGui::PopStyleColor(3);
					ImGui::PopID();
				}
				else
				{
					ImGui::PushID(i);
					ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(2/7.0f, 0.6f, 0.6f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(2/7.0f, 0.7f, 0.7f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(2/7.0f, 0.8f, 0.8f));
					if (ImGui::Button("PLAY")) { daClip->setState(CS_PLAYING); }
					ImGui::PopStyleColor(3);
					ImGui::PopID();
				}
				ImGui::SameLine(); // Progress bar
				progress = daClip->getTime() / daClip->getLength();
 				ImGui::ProgressBar(progress, ImVec2(100, 0.f),"");
 				const char * clip_name = daClip->getName().c_str(); // Clip Name
				ImGui::SameLine(); if (ImGui::Button(clip_name)) { details = i+1; }
			}
            ImGui::EndChild();

            ImGui::SameLine();

            ImGui::BeginChild("Details", ImVec2(0,0), true);
            //ImGui::Text("Details");
            if (details) 
            {
				AudioClip * daClip = audioMaster.getClipSet()->at(details-1);
				ImGui::PushItemWidth(100);
				ImGui::TextColored(ImColor(255,255,0), "%s", daClip->getName().c_str());
				ImGui::SameLine(); ImGui::Text("%s", daClip->getPath().c_str());
				ImGui::SliderFloat("volume", daClip->getVolume(), 0.0f, 1.0f, "%.3f");
				ImGui::SliderFloat("rate", daClip->getGUIRateP(), 0.125f, 8.0f, "%.3f"); daClip->updateRate();
				ImGui::SliderInt("pitch", daClip->getGUIPitchP(), -24, 24); daClip->updatePitch();
				ImGui::PopItemWidth();
			}
            ImGui::EndChild();
            ImGui::PopStyleColor();
            ImGui::PopStyleVar();

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

