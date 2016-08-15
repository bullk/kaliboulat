#include "GUI.h"
#include "MidiFile.h"

using namespace std;

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

void displayMidiFile ()
{
	//unsigned short int beats_per_bar = 4;
	//MidiFile * daFile;
	//unsigned int ntracks = daFile->getNumberOfTracks();
	//ImGui::PushItemWidth(100);
	//ImGui::TextColored(ImColor(255,255,0), "%s", daFile->getName().c_str());
	//ImGui::Text("Location : %s", daFile->getPath().c_str());
	//ImGui::Text("MIDI file format %d", daFile->getFileFormat());
	//ImGui::Text("%d tracks", ntracks);
	//ImGui::Text("division value %d", daFile->getDivision());
	//ImGui::Text("length : %lu", daFile->getLength());
	//ImGui::Text("time : %lu", daFile->getTime());
	//for (unsigned int i=0; i<ntracks; i++)
	//{
		//if (ImGui::TreeNode((void*)(intptr_t)i, "Track %d", i))
		//{
			//ImGui::Text ("%f µs / tick", 1000000 * daFile->getTickSeconds(i)); ImGui::SameLine();
			//ImGui::Text ("-> %.02f BPM", 60 / (daFile->getTickSeconds(i) * daFile->getDivision()));
			//ImGui::Separator();
			//ImGui::Text("time (BBT)  "); ImGui::SameLine();
			//ImGui::Text("Status  "); ImGui::SameLine();
			//ImGui::Text("DATA"); 
			//ImGui::Separator();
//
			//daFile->rewindTrack(i);
			//vector< unsigned char > * event = new vector<unsigned char>();
			//unsigned long abs_time = 0, delta_time = 0;
			//
			//delta_time = daFile->getNextEvent(event, i);
			//while ( event->size() > 0 )
			//{
				//abs_time += delta_time;
				//int nbeats = abs_time / daFile->getDivision();
				//int tick = abs_time % daFile->getDivision();
				//int beat = 1 + nbeats % beats_per_bar;
				//int bar = 1 + nbeats / beats_per_bar;
				//ImGui::Text("%02d:%02d:%03d   ", bar, beat, tick); ImGui::SameLine();
				//ImGui::Text("%x      ", event->at(0)); ImGui::SameLine();
				//for (unsigned int i=1; i<event->size(); i++)
				//{
					//if (i>1) ImGui::SameLine();
					//ImGui::Text("%x", event->at(i));
				//}
				//delta_time = daFile->getNextEvent(event, i);
			//}
			//ImGui::Separator();
			//delete event;
//
			//ImGui::TreePop();
		//}
	//}
}


void GUI_Main(bool* main_switch_p, Clock* main_clock_p, AudioGroup* audiogroup_p, MidiGroup* midigroup_p, Project* project_p)
{
	static int details = 0;
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSdl_ProcessEvent(&event);
		if (event.type == SDL_QUIT)
			*main_switch_p = false;
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
		ImGui::Begin("Software", main_switch_p, flags);
		
		static bool about_open = false;
		bool enabled = not(main_clock_p->getState());

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N", false, enabled)) {}
				if (ImGui::MenuItem("Open", "Ctrl+O", false, enabled)) {}
				if (ImGui::MenuItem("Save", "Ctrl+S", false, enabled)) {}
				if (ImGui::MenuItem("Save As..", NULL, false, enabled)) {}
				ImGui::Separator();
				if (ImGui::MenuItem("Options")) {}
				if (ImGui::MenuItem("Quit", "Alt+F4")) *main_switch_p = false;
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Import"))
			{
				if (ImGui::MenuItem("Audio File", NULL, false, enabled)) {}
				if (ImGui::MenuItem("MIDI File", NULL, false, enabled)) {}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Help"))
			{
				ImGui::MenuItem("About this software", NULL, &about_open);
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		
		ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
		ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.00f));

		ImGui::BeginChild("Master Controls", ImVec2(0, 50), true);

		// Clock
		ImGui::TextColored(ImColor(255,255,0), "%02d:%02d:%02d", main_clock_p->getHour(), main_clock_p->getMinute(), main_clock_p->getSecond());
		//ImGui::SameLine(); ImGui::TextColored(ImColor(127,127,127), "%d", mcDelta);

		// MIDI Clock
		ImGui::SameLine(); ImGui::TextColored(ImColor(0,255,255), "%02d:%02d:%03d", main_clock_p->getBar(), main_clock_p->getBeat(), main_clock_p->getTick());
		
		
		if (about_open) 
			ImGui::OpenPopup("About");
		if (ImGui::BeginPopupModal("About", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("      Kaliboulat / Fullbox Project\n\n    Alien sequencer by BullK Studio\nCoded by Olivier Cadiou aka Al Kali Boul\n");
			ImGui::Columns(3, NULL, false); ImGui::NextColumn();
			if (ImGui::Button("Close", ImVec2(80,0))) 
			{
				ImGui::CloseCurrentPopup();
				about_open = false;
			}
			ImGui::NextColumn(); ImGui::NextColumn(); ImGui::Columns(1);
			ImGui::EndPopup();
		}
		
		//TODO : Intégrer les images de commandes

		if ( main_clock_p->getState() )
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(1/7.0f, 0.6f, 0.6f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(1/7.0f, 0.7f, 0.7f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(1/7.0f, 0.8f, 0.8f));
			if (ImGui::Button("STOP"))
			{
				audiogroup_p->stopAll();
				main_clock_p->stop();
			}
			ImGui::PopStyleColor(3);
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(2/7.0f, 0.6f, 0.6f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(2/7.0f, 0.7f, 0.7f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(2/7.0f, 0.8f, 0.8f));
			if (ImGui::Button("PLAY")) main_clock_p->start();
			ImGui::PopStyleColor(3);
		}
		ImGui::EndChild();
		
		ImGui::BeginChild("Clips", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 0), true);
		float progress = 0.0f;

		// Audio Clips
		for ( unsigned int i = 0; i < audiogroup_p->getClipSet()->size(); i++ )
		{
			AudioClip * daClip = audiogroup_p->getClipSet()->at(i);
			if ( daClip->getState() == CS_PLAYING )
			{
				ImGui::PushID(i);
				ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(1/7.0f, 0.6f, 0.6f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(1/7.0f, 0.7f, 0.7f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(1/7.0f, 0.8f, 0.8f));
				if (ImGui::Button("STOP")) daClip->setState(CS_STOPPED);
				ImGui::PopStyleColor(3);
				ImGui::PopID();
			}
			else
			{
				ImGui::PushID(i);
				ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(2/7.0f, 0.6f, 0.6f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(2/7.0f, 0.7f, 0.7f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(2/7.0f, 0.8f, 0.8f));
				if (ImGui::Button("PLAY"))
				{
					daClip->setState(CS_PLAYING);
					main_clock_p->start();
				}
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
		for ( unsigned int i = 0; i < midigroup_p->getClipSet()->size(); i++ )
		{
			MidiClip * daClip = midigroup_p->getClipSet()->at(i);
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
				if (ImGui::Button("PLAY"))
				{
					daClip->setState(CS_PLAYING);
					main_clock_p->start();
				}
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
			AudioClip * daClip = audiogroup_p->getClipSet()->at(details-1);
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

