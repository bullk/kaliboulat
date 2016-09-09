//#include <typeinfo>
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

//======================================================================

void mainMenu (bool enabled, bool * main_switch, AudioModule * audio, Project * project)
{
	static bool about_open = false;
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
		if (ImGui::BeginMenu("New"))
		{
			if (ImGui::MenuItem("Audio Track", NULL, false, enabled)) project -> addTrack ( "AudioTrack" );
			if (ImGui::MenuItem("MIDI Track", NULL, false, false)) {}
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

void ProjectScreen (bool* main_switch, MidiTrack* miditrack, Project* project)
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
	
	bool enabled = not(project -> getClock() -> getState());
	mainMenu (enabled, main_switch, project -> getAudio (), project);
	
	ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
	ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.00f));

	ImGui::BeginChild("Master Controls", ImVec2(0, 50), true);
	//TODO : Intégrer les images de commandes
	if ( project -> getClock() -> getState() )
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(1/7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(1/7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(1/7.0f, 0.8f, 0.8f));
		if (ImGui::Button("STOP")) project -> getClock() -> stop();
		ImGui::PopStyleColor(3);
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(2/7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(2/7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(2/7.0f, 0.8f, 0.8f));
		if (ImGui::Button("PLAY")) project -> getClock() -> start();
		ImGui::PopStyleColor(3);
	}
	// Clock
	ImGui::SameLine(); ImGui::TextColored(ImColor(255,255,0), "%02d:%02d:%02d", project -> getClock() -> getHour(), project -> getClock() -> getMinute(), project -> getClock() -> getSecond());
	//ImGui::SameLine(); ImGui::TextColored(ImColor(127,127,127), "%d", mcDelta);

	// MIDI Clock
	ImGui::SameLine(); ImGui::TextColored(ImColor(0,255,255), "%02d:%02d:%03d", project -> getClock() -> getBar(), project -> getClock() -> getBeat(), project -> getClock() -> getTick());
	ImGui::EndChild();
	
	// MAIN BOARD
	ImGui::BeginChild("Clips", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 0), true);
	ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 0.0f);

	// Audio Clips
	for ( unsigned int i = 0; i < project -> getAudio () -> getClipSet() -> size(); i++ )
	{
		AudioClip * daClip = project -> getAudio () -> getClipSet() -> at(i);
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

		if ( ImGui::BeginPopup("popup-clip") )
		{
			if ( ImGui::Selectable("delete this clip") )
			{
				//details.audioclip = 0;
				details.context = Screen::NONE;
				project -> getAudio () -> deleteClip (i);
			}
			ImGui::EndPopup ();
		}

		ImGui::EndChild ();
		if ( lock ) ImGui::PopStyleColor ();
		ImGui::PopID ();
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
		ImGui::ProgressBar(progress, ImVec2(100, 0.f), "");
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
	if ( project -> getAudio () -> getClipSet() -> size() )
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
						project -> getAudio () -> addClip ( project->getAudioDir() + "/" + list -> at(i).c_str() );
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
			AudioClip * daClip = project -> getAudio () -> getClipSet() -> at(details.audioclip);
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

void ConsoleScreen (bool* main_switch, MidiTrack* miditrack, Project* project) 
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
	
	bool enabled = not(project -> getClock() -> getState());
	mainMenu (enabled, main_switch, project -> getAudio (), project);
	
	ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 0.0f);
	ImGui::BeginChild("Title", ImVec2(305, 90));
	ImGui::Text(" ####  ###  #   #  ###   ###  #     #####");
	ImGui::Text("#     #   # ##  # #     #   # #     #");
	ImGui::Text("#     #   # # # #  ###  #   # #     ####");
	ImGui::Text("#     #   # #  ##     # #   # #     #");
	ImGui::Text(" ####  ###  #   #  ###   ###  ##### #####");
	ImGui::EndChild();
	ImGui::PopStyleVar();
	
	ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.00f));
	ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);

	ImGui::SameLine();
	ImGui::BeginChild("Master Controls", ImVec2(0, 85), true);
	//TODO : Intégrer les images de commandes
	if ( project -> getClock() -> getState () )
	{
		ImGui::PushStyleColor (ImGuiCol_Button, ImColor::HSV(1/7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor (ImGuiCol_ButtonHovered, ImColor::HSV(1/7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor (ImGuiCol_ButtonActive, ImColor::HSV(1/7.0f, 0.8f, 0.8f));
		if (ImGui::Button ("STOP")) project -> getClock() -> stop();
		ImGui::PopStyleColor (3);
	}
	else
	{
		ImGui::PushStyleColor (ImGuiCol_Button, ImColor::HSV(2/7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor (ImGuiCol_ButtonHovered, ImColor::HSV(2/7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor (ImGuiCol_ButtonActive, ImColor::HSV(2/7.0f, 0.8f, 0.8f));
		if (ImGui::Button ("PLAY")) project -> getClock() -> start();
		ImGui::PopStyleColor(3);
	}
	// Clock
	ImGui::SameLine (); ImGui::TextColored (ImColor(255,255,0), "%02d:%02d:%02d", project -> getClock() -> getHour(), project -> getClock() -> getMinute(), project -> getClock() -> getSecond());
	//ImGui::SameLine (); ImGui::TextColored (ImColor(127,127,127), "%d", mcDelta);

	// MIDI Clock
	ImGui::SameLine (); ImGui::TextColored (ImColor(0,255,255), "%02d:%02d:%03d", project -> getClock() -> getBar(), project -> getClock() -> getBeat(), project -> getClock() -> getTick());
	ImGui::EndChild ();

	// TODO : Afficher les pistes
	ImGui::BeginChild("Board", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
	for (unsigned int i=0; i < project -> nTracks (); i++)
	{
		ImGui::PushStyleVar (ImGuiStyleVar_ChildWindowRounding, 0.0f);
		ImGui::PushID (2048 + i);
		if ( i > 0 ) ImGui::SameLine ();
		ImGui::BeginChild (2048 + i, ImVec2(120, 0), true);

		if ( ImGui::Button("X") ) project -> deleteTrack (i);
		
		if ( i > 0 ) 
		{
			ImGui::SameLine ();
			if ( ImGui::Button("<") ) { project -> swapTracks (i, i-1); }
		}
		
		if ( i < project->nTracks()-1 ) 
		{
			ImGui::SameLine ();
			if ( ImGui::Button(">") ) { project -> swapTracks (i, i+1); }
		}
		
		ImGui::Text ( "%s", project -> getTrack(i) -> getName().c_str() );
		if (ImGui::BeginPopupContextItem("rename test"))
		{
			char buf[20];
			sprintf (buf, "%s", project -> getTrack(i) -> getName().c_str());
			if ( ImGui::InputText ("track name", buf, IM_ARRAYSIZE(buf), ImGuiInputTextFlags_AutoSelectAll|ImGuiInputTextFlags_EnterReturnsTrue) ) 
			{
				project -> getTrack(i) -> setName(buf);
				ImGui::CloseCurrentPopup ();
			}
			ImGui::EndPopup ();
		}

		ImGui::EndChild ();
		ImGui::PopID ();
		ImGui::PopStyleVar ();
	}
	ImGui::EndChild ();

	ImGui::PopStyleColor ();
	ImGui::PopStyleVar ();

	ImGui::End ();
}

void SequencerScreen (bool* main_switch, MidiTrack* miditrack, Project* project) 
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
	
	bool enabled = not(project -> getClock() -> getState());
	mainMenu (enabled, main_switch, project -> getAudio (), project);
	
	ImGui::Text(" XXX  XXXXX  XXX  X   X XXXXX X   X  XXXX XXXXX XXXX");
	ImGui::Text("X     X     X   X X   X X     XX  X X     X     X   X");
	ImGui::Text(" XXX  XXXX  X   X X   X XXXX  X X X X     XXXX  XXX");
	ImGui::Text("    X X     X  X  X   X X     X  XX X     X     X  X");
	ImGui::Text(" XXX  XXXXX  XXXX  XXX  XXXXX X   X  XXXX XXXXX X   X");
	ImGui::End();
}

//======================================================================


void GUI_Main(bool* main_switch, MidiTrack* miditrack, Project* project)
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
			ProjectScreen (main_switch, miditrack, project);
			break;
			
		case Screen::CONSOLE:
			ConsoleScreen (main_switch, miditrack, project);
			break;
		
		case Screen::SEQUENCER:
			SequencerScreen (main_switch, miditrack, project);
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

