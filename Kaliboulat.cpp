#include <stdio.h>
#include <string>
//#include <time.h>

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

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

using namespace stk;
using namespace std;

// MIDI
void midiInit();

// Audio Rendering
int tick();
void audioInit();
void audioClose();
void clockStart ();
void clockPause ();
void clockContinue ();
void clockStop ();

// GUI
int GUI_Init();
void GUI_Close();


//----------------------------------------------------------------------
// MIDI FUNCTIONS
//----------------------------------------------------------------------

void midiInit()
{
	midiMaster.addAclip (midiClipDir + "/" + midiClipLs[0]);
}


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


void audioInit ()
{
	Stk::setSampleRate (GLOBAL_SAMPLE_RATE);
	Stk::showWarnings (true);
	
	// Figure out how many bytes in an StkFloat and setup the RtAudio stream.
	RtAudio::StreamParameters parameters;
	parameters.deviceId = dac.getDefaultOutputDevice();
	parameters.nChannels = 1;
	RtAudioFormat format = ( sizeof(StkFloat) == 8 ) ? RTAUDIO_FLOAT64 : RTAUDIO_FLOAT32;
	//RtAudioFormat format = RTAUDIO_SINT16;

	unsigned int bufferFrames = RT_BUFFER_SIZE;

	audioMaster.addAclip (sampleDir + "/" + sampleLs[0]);
	audioMaster.addAclip (sampleDir + "/" + sampleLs[1]);
	audioMaster.addAclip (sampleDir + "/" + sampleLs[2]);

	try {
		dac.openStream( &parameters, NULL, format, (unsigned int)Stk::sampleRate(), &bufferFrames, &tick, (void *)&audioMaster );
	}
	catch ( RtError &error ) {
		error.printMessage();
	}

	try {
		dac.startStream();
	}
	catch ( RtError &error ) {
		error.printMessage();
	}

} 

void audioClose ()
{
	try {
		dac.stopStream();
	}
	catch ( RtError &error ) {
		error.printMessage();
	}	
	
	try {
		dac.closeStream();
	}
	catch ( RtError &error ) {
		error.printMessage();
	}
}

void clockStart ()
{
	mcStartTime = chrono::system_clock::now();
	mcState = true;
}

void clockPause ()
{
}

void clockContinue ()
{
}
	
void clockStop ()
{
	mcState = false;
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
	//SDL_GLContext glcontext = SDL_GL_CreateContext(window);
	glcontext = SDL_GL_CreateContext(window);

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
	
	long unsigned int previous_ticks=0, nticks=0;
	unsigned int ticks_delta=0;
	int is=0, im=0, h=0, m=0, s=0;

	int tempo = 120;
	int ticks_per_beat = 960;
	int beats_per_bar = 4;
	float tick_d = 60.0f / (tempo * ticks_per_beat);
	int nbeats=0, bar=1, beat=1, tick=0;
		
	audioInit();
	midiInit();

	// Main loop
	bool go_on = true;
	while (go_on)
	{
		// Clock
		if (mcState) {
			// h:m:s
			mcNow = chrono::system_clock::now();
			mcDelta = std::chrono::duration_cast<std::chrono::microseconds> (mcNow - mcStartTime).count();
			is = mcDelta / 1000000;
			im = is / 60;
			h = im / 60;
			m = im % 60;
			s = is % 60;

			// MIDI (bar, beat, tick)
			nticks = (long unsigned int) (mcDelta / (1000000*tick_d));
			if (nticks < previous_ticks)
				previous_ticks = 0;
			ticks_delta = nticks - previous_ticks;
			if ((previous_ticks == 0) and (ticks_delta == 0))
				midiMaster.tick();
			if (ticks_delta > 0)
				for (unsigned int i=0; i<ticks_delta; i++) 
					midiMaster.tick();

			nbeats = nticks / ticks_per_beat;
			tick = nticks % ticks_per_beat;
			beat = 1 + nbeats % beats_per_bar;
			bar = 1 + nbeats / beats_per_bar;
		}

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSdl_ProcessEvent(&event);
			if (event.type == SDL_QUIT)
				go_on = false;
		}

		ImGui_ImplSdl_NewFrame(window);
		// Main window
		{
			ImGuiWindowFlags flags = 0;
			flags |= ImGuiWindowFlags_NoTitleBar;
			flags |= ImGuiWindowFlags_NoResize;
			flags |= ImGuiWindowFlags_NoMove;
			flags |= ImGuiWindowFlags_NoCollapse;
			flags |= ImGuiWindowFlags_MenuBar;
			//ImGui::SetNextWindowSize(ImVec2(1920,1080));
			ImGui::SetNextWindowSize(ImVec2((int)ImGui::GetIO().DisplaySize.x,(int)ImGui::GetIO().DisplaySize.y));
			ImGui::Begin("Software", &go_on, flags);
			
			static bool show_app_about = false;
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("New", "Ctrl+N", false, not(mcState))) {}
					if (ImGui::MenuItem("Open", "Ctrl+O", false, not(mcState))) {}
					if (ImGui::MenuItem("Save", "Ctrl+S", false, not(mcState))) {}
					if (ImGui::MenuItem("Save As..", NULL, false, not(mcState))) {}
					ImGui::Separator();
					if (ImGui::MenuItem("Options")) {}
					if (ImGui::MenuItem("Quit", "Alt+F4")) go_on = false;
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Import"))
				{
					if (ImGui::MenuItem("Audio File", NULL, false, not(mcState))) {}
					if (ImGui::MenuItem("MIDI File", NULL, false, not(mcState))) {}
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Help"))
				{
					ImGui::MenuItem("About ImGui", NULL, &show_app_about);
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}
			
            ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
            ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.00f));

            ImGui::BeginChild("Master Controls", ImVec2(0, 50), true);

			// Clock
			ImGui::TextColored(ImColor(255,255,0), "%02d:%02d:%02d", h, m, s);
			//ImGui::SameLine(); ImGui::TextColored(ImColor(127,127,127), "%d", mcDelta);

			// MIDI Clock
			ImGui::SameLine(); ImGui::TextColored(ImColor(0,255,255), "%02d:%02d:%03d", bar, beat, tick);
			
			//ImGui::SameLine(); ImGui::TextColored(ImColor(0,255,255), "%d", value);
			
            //TODO : Intégrer les images de commandes
			//ImGui::Button("STOP")
			if ( mcState )
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(1/7.0f, 0.6f, 0.6f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(1/7.0f, 0.7f, 0.7f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(1/7.0f, 0.8f, 0.8f));
				if (ImGui::Button("STOP")) { clockStop(); }
				ImGui::PopStyleColor(3);
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(2/7.0f, 0.6f, 0.6f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(2/7.0f, 0.7f, 0.7f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(2/7.0f, 0.8f, 0.8f));
				if (ImGui::Button("PLAY")) { clockStart(); }
				ImGui::PopStyleColor(3);
			}
			ImGui::EndChild();
			
            ImGui::BeginChild("Clips", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 0), true);
			float progress = 0.0f;

			// Audio Clips
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
			
			// MIDI Clips
			for ( unsigned int i = 0; i < midiMaster.getClipSet()->size(); i++ )
			{
				MidiClip * daClip = midiMaster.getClipSet()->at(i);
				if ( daClip->getState() == CS_PLAYING )
				{
					ImGui::PushID(4096 + i);
					ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(1/7.0f, 0.6f, 0.6f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(1/7.0f, 0.7f, 0.7f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(1/7.0f, 0.8f, 0.8f));
					if (ImGui::Button("STOP")) daClip->setState(CS_STOPPED);
					ImGui::PopStyleColor(3);
					ImGui::PopID();
				}
				else
				{
					ImGui::PushID(4096 + i);
					ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(2/7.0f, 0.6f, 0.6f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(2/7.0f, 0.7f, 0.7f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(2/7.0f, 0.8f, 0.8f));
					if (ImGui::Button("PLAY")) daClip->setState(CS_PLAYING);
					ImGui::PopStyleColor(3);
					ImGui::PopID();
				}
				ImGui::SameLine(); // Progress bar
				progress = (float) daClip->getTime() / daClip->getLength(); //ImGuiCol_PlotHistogram
				ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(6/7.0f, 0.6f, 0.6f));
 				ImGui::ProgressBar(progress, ImVec2(100, 0.f),"");
 				ImGui::PopStyleColor();
 				const char * clip_name = daClip->getName().c_str(); // Clip Name
				ImGui::SameLine(); if (ImGui::Button(clip_name)) { details = -1-i; }
			}

            ImGui::EndChild();

            ImGui::SameLine();

            ImGui::BeginChild("Details", ImVec2(0,0), true);
            //ImGui::Text("Details");
            if (details > 0) 
            {
				AudioClip * daClip = audioMaster.getClipSet()->at(details-1);
				ImGui::PushItemWidth(100);
				ImGui::TextColored(ImColor(255,255,0), "%s", daClip->getName().c_str());
				ImGui::Text("Location : %s", daClip->getPath().c_str());
				ImGui::SliderFloat("volume", daClip->getVolume(), 0.0f, 1.0f, "%.3f");
				ImGui::SliderFloat("rate", daClip->getGUIRateP(), 0.125f, 8.0f, "%.3f"); daClip->updateRate();
				ImGui::SliderInt("pitch", daClip->getGUIPitchP(), -12, 12); daClip->updatePitch();
				ImGui::PopItemWidth();
				//ImGui::PlotLines("DATA", daClip->getGUIData(), IM_ARRAYSIZE(daClip->getGUIData()), 0, NULL, -1.0f, 1.0f, ImVec2(0,80));
				//ImGui::Text("size %d", IM_ARRAYSIZE(daClip->getGUIData()));
			}
			else if (details < 0)
			{
				MidiClip * daClip = midiMaster.getClipSet()->at(-details-1);
				unsigned int ntracks = daClip->getNumberOfTracks();
				ImGui::PushItemWidth(100);
				ImGui::TextColored(ImColor(255,255,0), "%s", daClip->getName().c_str());
				ImGui::Text("Location : %s", daClip->getPath().c_str());
				ImGui::Text("MIDI file format %d", daClip->getFileFormat());
				ImGui::Text("%d tracks", ntracks);
				ImGui::Text("division value %d", daClip->getDivision());
				ImGui::Text("length : %lu", daClip->getLength());
				ImGui::Text("time : %lu", daClip->getTime());
				for (unsigned int i=0; i<ntracks; i++)
				{
		            if (ImGui::TreeNode((void*)(intptr_t)i, "Track %d", i))
		            {
						ImGui::Text ("%f µs / tick", 1000000 * daClip->getTickSeconds(i)); ImGui::SameLine();
						ImGui::Text ("-> %.02f BPM", 60 / (daClip->getTickSeconds(i) * daClip->getDivision()));
						ImGui::Separator();
						ImGui::Text("time (BBT)  "); ImGui::SameLine();
						ImGui::Text("Status  "); ImGui::SameLine();
						ImGui::Text("DATA"); 
						ImGui::Separator();
						//------------------------
						daClip->rewindTrack(i);
						vector< unsigned char > * event = new vector<unsigned char>();
						unsigned long abs_time = 0, delta_time = 0;
						
						delta_time = daClip->getNextEvent(event, i);
						while ( event->size() > 0 )
						{
							abs_time += delta_time;
							nbeats = abs_time / daClip->getDivision();
							tick = abs_time % daClip->getDivision();
							beat = 1 + nbeats % beats_per_bar;
							bar = 1 + nbeats / beats_per_bar;
							ImGui::Text("%02d:%02d:%03d   ", bar, beat, tick); ImGui::SameLine();
							ImGui::Text("%x      ", event->at(0)); ImGui::SameLine();
							for (unsigned int i=1; i<event->size(); i++)
							{
								if (i>1) ImGui::SameLine();
								ImGui::Text("%x", event->at(i));
							}
							//ImGui::Text("%u", event->size());
							delta_time = daClip->getNextEvent(event, i);
						}
						ImGui::Separator();
						delete event;
						//------------------------
                        ImGui::TreePop();
		            }
				}
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
		
		// MIDI Clock tracker
		previous_ticks = nticks;
	}
	
	// Close application
	GUI_Close();
	audioClose();
	return 0; /* ISO C requires main to return int. */

}

