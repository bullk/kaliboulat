#include "GUI.hpp"

using namespace std;


SDL_GLContext glcontext;
SDL_Window * window = NULL;
ImVec4 clear_color;
Screen details = { Screen::TITLE, Screen::PROJECT, 0, 0 };

int GUI_Init ()
{
	// Setup SDL
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

void clipPlayButton (Clip * clip)
{
	if ( clip -> isPlaying () )
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(1/7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(1/7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(1/7.0f, 0.8f, 0.8f));
		if (ImGui::Button("STOP")) clip -> stop ();
		ImGui::PopStyleColor(3);
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(2/7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(2/7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(2/7.0f, 0.8f, 0.8f));
		if (ImGui::Button("PLAY")) clip -> play ();
		ImGui::PopStyleColor(3);
	}
}

void displayMidiClipDetails (MidiClip * daClip)
{
	unsigned short beats_per_bar = 4;
	ImGui::PushItemWidth(100);
	ImGui::TextColored(ImColor(255,255,0), "%s", daClip -> getName().c_str());
	ImGui::Text("Location : %s", daClip -> getPath().c_str());
	ImGui::Text("size : %lu events", daClip -> getSize());
	ImGui::Text("division value : %d ticks/beat", daClip -> getDivision());
	ImGui::Text("length : %lu ticks", daClip -> getLength());
	ImGui::Text("time : %lu", daClip -> getTime());
	ImGui::Separator();
	ImGui::Text("time (BBT)  "); ImGui::SameLine();
	ImGui::Text("Status  "); ImGui::SameLine();
	ImGui::Text("DATA"); 
	ImGui::Separator();

	ScheduledMidiMessage * event = NULL;
	for (unsigned long i=0; i < daClip -> getSize(); i++)
	{
		event = daClip -> getEvent(i);
		int nbeats = event -> getTime() / daClip -> getDivision();
		int tick = event -> getTime() % daClip -> getDivision();
		int beat = 1 + nbeats % beats_per_bar;
		int bar = 1 + nbeats / beats_per_bar;
		
		bool selected = ( daClip -> getIndex () == i );
		char bbt[13];
		sprintf (bbt, "%02d:%02d:%03d   ", bar, beat, tick);
		ImGui::Selectable(bbt, selected); ImGui::SameLine();
		ImGui::Text("%x      ", event -> getData() -> at (0)); ImGui::SameLine();
		for (unsigned int j=1; j < event -> getData() -> size(); j++)
		{
			if (j>1) ImGui::SameLine();
			ImGui::Text("%x", event -> getData() -> at (j));
		}
	}
	ImGui::Separator();
}


//======================================================================

void TitleScreen () {}

void ProjectScreen (bool* main_switch, Clock* main_clock, AudioTrack* audiotrack, MidiTrack* miditrack, Project* project)
{
	ImGuiWindowFlags flags = 0;
	flags |= ImGuiWindowFlags_NoTitleBar;
	flags |= ImGuiWindowFlags_NoResize;
	flags |= ImGuiWindowFlags_NoMove;
	flags |= ImGuiWindowFlags_NoCollapse;
	flags |= ImGuiWindowFlags_MenuBar;
	
	//ImGui::SetNextWindowSize(ImVec2(1920,1080));
	ImGui::SetNextWindowSize(ImVec2((int)ImGui::GetIO().DisplaySize.x,(int)ImGui::GetIO().DisplaySize.y));
	ImGui::SetNextWindowPos(ImVec2(0,0));
	
	char buf[64];
	sprintf (buf, "%s - Kaliboulat", project -> getName().c_str());
	ImGui::Begin(buf, main_switch, flags);
	
	static bool about_open = false;
	bool enabled = not(main_clock -> getState());

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
			if (ImGui::MenuItem("Quit", "Alt+F4")) *main_switch = false;
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
	
	ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
	ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.00f));

	ImGui::BeginChild("Master Controls", ImVec2(0, 50), true);
	//TODO : Intégrer les images de commandes
	if ( main_clock -> getState() )
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(1/7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(1/7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(1/7.0f, 0.8f, 0.8f));
		if (ImGui::Button("STOP")) main_clock -> stop();
		ImGui::PopStyleColor(3);
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(2/7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(2/7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(2/7.0f, 0.8f, 0.8f));
		if (ImGui::Button("PLAY")) main_clock -> start();
		ImGui::PopStyleColor(3);
	}
	// Clock
	ImGui::SameLine(); ImGui::TextColored(ImColor(255,255,0), "%02d:%02d:%02d", main_clock -> getHour(), main_clock -> getMinute(), main_clock -> getSecond());
	//ImGui::SameLine(); ImGui::TextColored(ImColor(127,127,127), "%d", mcDelta);

	// MIDI Clock
	ImGui::SameLine(); ImGui::TextColored(ImColor(0,255,255), "%02d:%02d:%03d", main_clock -> getBar(), main_clock -> getBeat(), main_clock -> getTick());
	ImGui::EndChild();
	
	// MAIN BOARD
	ImGui::BeginChild("Clips", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 0), true);
	ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 0.0f);

	// Audio Clips
	for ( unsigned int i = 0; i < audiotrack -> getClipSet() -> size(); i++ )
	{
		AudioClip * daClip = audiotrack -> getClipSet() -> at(i);
		bool lock = false;
		float progress = 0.0f;
		ImGui::PushID(i);
		if ( details.audioclip == i ) 
		{ 
			ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImColor::HSV(3/7.0f, 0.4f, 0.4f));
			lock = true;
		}
		ImGui::BeginChild(i, ImVec2(0, 30)); 
		if ( ImGui::IsMouseHoveringWindow () )
		{
			if ( ImGui::IsMouseClicked (0) )
			{
				details.context = Screen::AUDIOCLIP;
				details.audioclip = i;
			}
			else if ( ImGui::IsMouseClicked (1) ) 
			{
				details.context = Screen::AUDIOCLIP;
				details.audioclip = i;
				ImGui::OpenPopup("popup-clip");
			}
		}
		clipPlayButton ( daClip );
		ImGui::SameLine(); // Progress bar
		progress = daClip -> getTime() / daClip -> getLength();
		ImGui::ProgressBar(progress, ImVec2(100, 0.f),""); 
		ImGui::SameLine(); ImGui::Text("%s", daClip -> getName().c_str());

		if (ImGui::BeginPopup("popup-clip"))
		{
			if ( ImGui::Selectable("delete this clip") )
			{
				//details.audioclip = 0;
				details.context = Screen::NONE;
				audiotrack -> deleteClip (i);
			}
			ImGui::EndPopup();
		}

		ImGui::EndChild();
		if ( lock ) ImGui::PopStyleColor();
		ImGui::PopID();
	}
	
	// MIDI Clips
	for ( unsigned int i = 0; i < miditrack -> getClipSet() -> size(); i++ )
	{
		MidiClip * daClip = miditrack -> getClipSet() -> at(i);
		bool lock = false;
		float progress = 0.0f;
		ImGui::PushID(4096 + i);
		if ( details.midiclip == i ) 
		{ 
			ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImColor::HSV(6/7.0f, 0.4f, 0.4f));
			lock = true;
		}
		ImGui::BeginChild(4096 + i, ImVec2(0, 30)); 
		if ( ImGui::IsMouseHoveringWindow () )
		{
			if ( ImGui::IsMouseClicked (0) )
			{
				details.context = Screen::MIDICLIP;
				details.midiclip = i;
			}
			else if ( ImGui::IsMouseClicked (1) ) 
			{
				details.context = Screen::MIDICLIP;
				details.midiclip = i;
				ImGui::OpenPopup("popup-midiclip");
			}
		}
		clipPlayButton (daClip);
		ImGui::SameLine(); // Progress bar
		progress = (float) daClip -> getTime() / daClip -> getLength();
		ImGui::ProgressBar(progress, ImVec2(100, 0.f),"");
		ImGui::SameLine(); ImGui::Text("%s", daClip -> getName().c_str());
		
		if (ImGui::BeginPopup("popup-midiclip"))
		{
			if ( ImGui::Selectable("delete this clip") )
			{
				//details.midiclip = 0;
				details.context = Screen::NONE;
				miditrack -> deleteClip (i);
			}
			ImGui::EndPopup();
		}

		ImGui::EndChild();
		if ( lock ) ImGui::PopStyleColor();
		ImGui::PopID();
	}
	
	ImGui::PopStyleVar ();
	ImGui::EndChild ();

	ImGui::SameLine ();

	// DETAILS WINDOW
	ImGui::BeginChild ("Details", ImVec2(0,0), true);
	if (ImGui::Button ("Files")) details.context = Screen::PROJECT; ImGui::SameLine();
	if ( audiotrack -> getClipSet() -> size() )
		if (ImGui::Button ("Audio Clip")) details.context = Screen::AUDIOCLIP; ImGui::SameLine();
	if ( miditrack -> getClipSet() -> size() )
		if (ImGui::Button ("MIDI Clip")) details.context = Screen::MIDICLIP;
	ImGui::Separator();
	
	switch (details.context)
	{
		case Screen::PROJECT:
		{
			vector<std::string> * list = NULL;
			string str;
			ImGui::SetNextTreeNodeOpen(true);
			if (ImGui::CollapsingHeader("Audio Files (click to import)"))
			{
				list = project -> getAudioFiles ();
				for ( unsigned int i=0; i < list -> size(); i++ )
					if ( ImGui::Selectable(list -> at(i).c_str()) )
						audiotrack -> addClip ( project->getAudioDir() + "/" + list -> at(i).c_str() );
			}
			ImGui::SetNextTreeNodeOpen(true);
			if (ImGui::CollapsingHeader("MIDI Files (click to import)"))
			{
				list = project -> getMIDIFiles ();
				for ( unsigned int i=0; i < list -> size(); i++ )
					if ( ImGui::Selectable(list -> at(i).c_str()) )
					{
						MidiFile * midifile = new MidiFile ( project->getMIDIDir() + "/" + list -> at(i).c_str() );
						midifile -> parse (miditrack);
						delete midifile;
					}
			}
		}	
			break;
		case Screen::AUDIOCLIP:
		{
			AudioClip * daClip = audiotrack -> getClipSet() -> at(details.audioclip);
			ImGui::PushItemWidth(100);
			ImGui::TextColored(ImColor(255,255,0), "%s", daClip -> getName().c_str());
			ImGui::Text("Location : %s", daClip -> getPath().c_str());
			ImGui::Separator();
			ImGui::SliderFloat("volume", daClip -> getVolume(), 0.0f, 1.0f, "%.3f");
			ImGui::SliderFloat("rate", daClip -> getGUIRateP(), 0.125f, 8.0f, "%.3f"); daClip -> updateRate();
			ImGui::SliderInt("pitch", daClip -> getGUIPitchP(), -12, 12); daClip -> updatePitch();
			ImGui::PopItemWidth();
			//ImGui::PlotLines("DATA", daClip -> getGUIData(), IM_ARRAYSIZE(daClip -> getGUIData()), 0, NULL, -1.0f, 1.0f, ImVec2(0,80));
			//ImGui::Text("size %d", IM_ARRAYSIZE(daClip -> getGUIData()));
		}
			break;
		case Screen::MIDICLIP:
			displayMidiClipDetails (miditrack -> getClipSet() -> at(details.midiclip));
			break;
		default:
			break;
	}
	ImGui::EndChild();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();

	ImGui::End();
}

void ConsoleScreen (bool* main_switch, Clock* main_clock, AudioTrack* audiotrack, MidiTrack* miditrack, Project* project) 
{	
	ImGuiWindowFlags flags = 0;
	flags |= ImGuiWindowFlags_NoTitleBar;
	flags |= ImGuiWindowFlags_NoResize;
	flags |= ImGuiWindowFlags_NoMove;
	flags |= ImGuiWindowFlags_NoCollapse;
	//flags |= ImGuiWindowFlags_MenuBar;

	//ImGui::SetNextWindowSize(ImVec2(1920,1080));
	ImGui::SetNextWindowSize(ImVec2((int)ImGui::GetIO().DisplaySize.x,(int)ImGui::GetIO().DisplaySize.y));
	ImGui::SetNextWindowPos(ImVec2(0,0));

	char buf[64];
	sprintf (buf, "%s - Kaliboulat", project -> getName().c_str());
	ImGui::Begin(buf, main_switch, flags);
	
	ImGui::Text(" OOOO  OOO  O   O  OOO   OOO  OO    OOOOO");
	ImGui::Text("O     O   O OO  O O     O   O O     O");
	ImGui::Text("O     O   O O O O  OOO  O   O O     OOOO");
	ImGui::Text("O     O   O O  OO     O O   O O     O");
	ImGui::Text(" OOOO  OOO  O   O  OOO   OOO  OOOOO OOOOO");
	
	ImGui::End();
}

void SequencerScreen (bool* main_switch, Clock* main_clock, AudioTrack* audiotrack, MidiTrack* miditrack, Project* project) 
{
	ImGuiWindowFlags flags = 0;
	flags |= ImGuiWindowFlags_NoTitleBar;
	flags |= ImGuiWindowFlags_NoResize;
	flags |= ImGuiWindowFlags_NoMove;
	flags |= ImGuiWindowFlags_NoCollapse;
	//flags |= ImGuiWindowFlags_MenuBar;

	//ImGui::SetNextWindowSize(ImVec2(1920,1080));
	ImGui::SetNextWindowSize(ImVec2((int)ImGui::GetIO().DisplaySize.x,(int)ImGui::GetIO().DisplaySize.y));
	ImGui::SetNextWindowPos(ImVec2(0,0));

	char buf[64];
	sprintf (buf, "%s - Kaliboulat", project -> getName().c_str());
	ImGui::Begin(buf, main_switch, flags);
	
	ImGui::Text(" OOO  OOOOO  OOO  O   O OOOOO O   O  OOOO OOOOO OOOO");
	ImGui::Text("O     O     O   O O   O O     OO  O O     O     O   O");
	ImGui::Text(" OOO  OOOO  O   O O   O OOOO  O O O O     OOOO  OOO");
	ImGui::Text("    O O     O  O  O   O O     O  OO O     O     O  O");
	ImGui::Text(" OOO  OOOOO  OOOO  OOO  OOOOO O   O  OOOO OOOOO O   O");
	ImGui::End();
}

//======================================================================


void GUI_Main(bool* main_switch, Clock* main_clock, AudioTrack* audiotrack, MidiTrack* miditrack, Project* project)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSdl_ProcessEvent(&event);
		switch ( event.type )
		{
			case SDL_QUIT:
				*main_switch = false;
				break;
				
			case SDL_KEYUP:
				switch ( event.key.keysym.scancode )
				{
					case SDL_SCANCODE_F1:
						details.type = Screen::RESSOURCES;
						break;
					case SDL_SCANCODE_F2:
						details.type = Screen::CONSOLE;
						break;
					case SDL_SCANCODE_F3:
						details.type = Screen::SEQUENCER;
						break;
					default:
						break;
				}
				break;
		}
	}

	ImGui_ImplSdl_NewFrame(window);
	
	// Main window
	switch ( details.type )
	{
		case Screen::RESSOURCES:
			ProjectScreen (main_switch, main_clock, audiotrack, miditrack, project);
			break;
			
		case Screen::CONSOLE:
			ConsoleScreen (main_switch, main_clock, audiotrack, miditrack, project);
			break;
		
		case Screen::SEQUENCER:
			SequencerScreen (main_switch, main_clock, audiotrack, miditrack, project);
			break;
		
		default:
			TitleScreen ();
			break;
	}

	// Rendering
	glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui::Render();
	SDL_GL_SwapWindow(window);
		
}

