#include <typeinfo>
#include <unistd.h> // sleep
#include "GUI.hpp"

using namespace std;

SDL_GLContext glcontext;
SDL_Window * window = NULL;
ImVec4 clear_color;
Screen details = { Screen::CONSOLE, Screen::RESSOURCES, 0, 0, "", NULL, NULL };

int width1, width2, width3, width4, width5;

bool ressources_panel = true;
bool action_drag_clip = false;

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

void BeginScreen ()
{
	ImGuiWindowFlags flags = 0;
	flags |= ImGuiWindowFlags_NoTitleBar;
	flags |= ImGuiWindowFlags_NoResize;
	flags |= ImGuiWindowFlags_NoMove;
	flags |= ImGuiWindowFlags_NoCollapse;
	flags |= ImGuiWindowFlags_MenuBar;
	
	ImGui::SetNextWindowSize(ImVec2((int)ImGui::GetIO().DisplaySize.x,(int)ImGui::GetIO().DisplaySize.y));
	ImGui::SetNextWindowPos(ImVec2(0,0));
	
	ImGui::Begin("main window", NULL, flags);
	ImGui::PushStyleColor (ImGuiCol_ChildWindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.00f));
	ImGui::PushStyleVar (ImGuiStyleVar_ChildWindowRounding, 5.0f);
	
	//width1 = (int)ImGui::GetIO().DisplaySize.x / 6;
	//width2 = (int)ImGui::GetIO().DisplaySize.x / 3;
	//width3 = (int)ImGui::GetIO().DisplaySize.x / 2;
	//width4 = 2 * (int)ImGui::GetIO().DisplaySize.x / 3;
	//width5 = 5 * (int)ImGui::GetIO().DisplaySize.x / 6;
	int w = (int) ImGui::GetWindowContentRegionWidth();
	width1 = w / 6;
	width2 = w / 3;
	width3 = w / 2;
	width4 = 2 * w / 3;
	width5 = 5 * w / 6;
}

void EndScreen ()
{
	ImGui::PopStyleColor ();
	ImGui::PopStyleVar ();
	ImGui::End();
}

void mainMenu (Project * project)
{
	static bool about_open = false;
	static bool new_audio_track = false;
	static bool new_midi_track = false;
	bool menu_mask = not(project -> getClock() -> getState());
	
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New", "Ctrl+N", false, menu_mask)) {}
			if (ImGui::MenuItem("Open", "Ctrl+O", false, menu_mask)) {}
			if (ImGui::MenuItem("Save", "Ctrl+S", false, menu_mask)) {}
			if (ImGui::MenuItem("Save As..", NULL, false, menu_mask)) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Options")) {}
			if (ImGui::MenuItem("Quit", "Alt+F4")) State::getInstance() -> halt();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Import"))
		{
			if (ImGui::MenuItem("Audio File", NULL, false, menu_mask)) {}
			if (ImGui::MenuItem("MIDI File", NULL, false, menu_mask)) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("New"))
		{
			//if (ImGui::MenuItem("Audio Track", "", false, menu_mask)) project -> addAudioTrack ( "AudioTrack" );
			//if (ImGui::MenuItem("MIDI Track", "", false, menu_mask)) project -> addMidiTrack ( "MidiTrack" );
			if (ImGui::MenuItem("Audio Track", "", false, menu_mask)) new_audio_track = true;
			if (ImGui::MenuItem("MIDI Track", "", false, menu_mask)) new_midi_track = true;
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			ImGui::MenuItem("About this software", NULL, &about_open);
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	
	if (new_audio_track) ImGui::OpenPopup("New Audio Track");
	if (new_midi_track) ImGui::OpenPopup("New Midi Track");
	if (about_open) ImGui::OpenPopup("About");
	
	if (ImGui::BeginPopupModal("New Audio Track", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		char buf[20];
		sprintf (buf, "%s", "AudioTrack");
		ImGui::SetKeyboardFocusHere ();
		if ( ImGui::InputText ("track name", buf, IM_ARRAYSIZE(buf), ImGuiInputTextFlags_AutoSelectAll|ImGuiInputTextFlags_EnterReturnsTrue) ) 
		{
			project -> addAudioTrack (buf);
			ImGui::CloseCurrentPopup ();
			new_audio_track = false;
		}
		//if (ImGui::Button("Close", ImVec2(200,0))) 
		//{
			//ImGui::CloseCurrentPopup();
			//new_audio_track = false;
		//}
		ImGui::EndPopup();
	}
	
	if (ImGui::BeginPopupModal("New Midi Track", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		char buf[20];
		sprintf (buf, "%s", "MidiTrack");
		ImGui::SetKeyboardFocusHere ();
		if ( ImGui::InputText ("track name", buf, IM_ARRAYSIZE(buf), ImGuiInputTextFlags_AutoSelectAll|ImGuiInputTextFlags_EnterReturnsTrue) ) 
		{
			project -> addMidiTrack (buf);
			ImGui::CloseCurrentPopup ();
			new_midi_track = false;
		}
		ImGui::EndPopup();
	}
	
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

void ControlPanel (Project * project, void (*title_func)())
// TODO : factoriser le panneau de commande (boutons, horloges...)
{
	ImGui::BeginChild ("Master Controls", ImVec2(0, 64), true);

	ImGui::Columns (8);
	title_func ();
	ImGui::NextColumn ();
	
	//TODO : Intégrer les images de boutons
	if ( project -> getClock() -> getState () )
	{ // STOP button
		ImGui::PushStyleColor (ImGuiCol_Button, ImColor::HSV(1/7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor (ImGuiCol_ButtonHovered, ImColor::HSV(1/7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor (ImGuiCol_ButtonActive, ImColor::HSV(1/7.0f, 0.8f, 0.8f));
		if (ImGui::Button ("STOP")) project -> getClock() -> stop();
		ImGui::PopStyleColor (3);
	}
	else
	{ // PLAY button
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
	if ( project -> ctrlPressed () ) ImGui::TextColored (ImColor(192,64,64), "CTRL");
	ImGui::NextColumn ();
	
	ImGui::Checkbox ("Ressources panel", &ressources_panel);
	ImGui::NextColumn ();
	
	ImGui::Text("width1 : %d", width1); ImGui::SameLine (); ImGui::Text("width4 : %d", width4);
	ImGui::Text("width2 : %d", width2); ImGui::SameLine (); ImGui::Text("width5 : %d", width5);
	ImGui::Text("width3 : %d", width3);
	ImGui::Columns(1);

	ImGui::EndChild ();
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

void displayMidiClipDetails (MidiClip * clip)
{
	unsigned short beats_per_bar = 4;
	ImGui::PushItemWidth(100);
	ImGui::TextColored(ImColor(255,255,0), "%s", clip -> getName().c_str());
	ImGui::Text("Location : %s", clip -> getPath().c_str());
	ImGui::Text("size : %lu events", clip -> getSize());
	ImGui::Text("division value : %d ticks/beat", clip -> getDivision());
	ImGui::Text("length : %lu ticks", clip -> getLength());
	ImGui::Text("time : %lu", clip -> getTime());
	ImGui::Separator();
	ImGui::Text("time (BBT)  "); ImGui::SameLine();
	ImGui::Text("Status  "); ImGui::SameLine();
	ImGui::Text("DATA"); 
	ImGui::Separator();

	ScheduledMidiMessage * event = NULL;
	for (unsigned long i=0; i < clip -> getSize(); i++)
	{
		event = clip -> getEvent(i);
		int nbeats = event -> getTime() / clip -> getDivision();
		int tick = event -> getTime() % clip -> getDivision();
		int beat = 1 + nbeats % beats_per_bar;
		int bar = 1 + nbeats / beats_per_bar;
		
		bool selected = ( clip -> getIndex () == i );
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

void displayAudioClipDetails (AudioClip * clip)
{
	ImGui::PushItemWidth(100);
	ImGui::TextColored(ImColor(255,255,0), "%s", clip -> getName().c_str());
	ImGui::Text("Location : %s", clip -> getPath().c_str());
	ImGui::Separator();
	ImGui::SliderFloat("volume", clip -> getVolume(), 0.0f, 1.0f, "%.3f");
	ImGui::SliderFloat("rate", clip -> getGUIRateP(), 0.125f, 8.0f, "%.3f"); clip -> updateRate();
	ImGui::SliderInt("pitch", clip -> getGUIPitchP(), -12, 12); clip -> updatePitch();
	ImGui::PopItemWidth();
	//ImGui::PlotLines("DATA", clip -> getGUIData(), IM_ARRAYSIZE(clip -> getGUIData()), 0, NULL, -1.0f, 1.0f, ImVec2(0,80));
	//ImGui::Text("size %d", IM_ARRAYSIZE(clip -> getGUIData()));
}

int displayAudioClipSet (Project* project, int selected)
{
	vector<std::string> * list = project -> getAudioFiles ();
	int res = selected;
	for (unsigned int i=0; i < list -> size(); i++ )
		if ( ImGui::Selectable(list -> at(i).c_str(), int(i)==selected) ) res = i;
	return res;
}
	
void displayMidiClipSet (Project* project)
{
	vector<std::string> * list = NULL;
	ImGui::SetNextTreeNodeOpen(true);
	if (ImGui::CollapsingHeader("MIDI Files"))
	{
		list = project -> getMIDIFiles ();
		for ( unsigned int i=0; i < list -> size(); i++ )
			if ( ImGui::Selectable(list -> at(i).c_str()) )
			{
				//MidiFile * midifile = new MidiFile ( project->getMIDIDir() + "/" + list -> at(i).c_str() );
				//midifile -> parse (miditrack);
				//delete midifile;
			}
	}
}

bool ChooseButton () 
{
	ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(2/7.0f, 0.4f, 0.4f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(2/7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(2/7.0f, 0.8f, 0.8f));
	bool res = ImGui::Button("CHOOSE");
	ImGui::PopStyleColor(3);
	return res;
}

bool HearButton ()
{
	ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(4/7.0f, 0.4f, 0.4f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(4/7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(4/7.0f, 0.8f, 0.8f));
	bool res = ImGui::Button("HEAR");
	ImGui::PopStyleColor(3);
	return res;
}

static void DragClipOverlay (std::string, bool* p_open)
{
    ImGui::SetNextWindowPos (ImVec2(ImGui::GetIO().MousePos.x + 5, ImGui::GetIO().MousePos.y + 5));
    if (!ImGui::Begin("Drag Clip Overlay", p_open, ImVec2(0,0), 0.3f, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::End();
        return;
    }
    ImGui::Text("%s", details.dragged_audio_file.c_str());
    ImGui::End();
}

void RessourcesPanel (Project* project)
{
	ImGui::BeginChild ("Ressources", ImVec2(width1,0), true);
	
	switch (details.context)
	{
		case Screen::RESSOURCES:
			ImGui::SetNextTreeNodeOpen(true);
			if (ImGui::CollapsingHeader("Audio Files"))
			{
				vector<std::string> * list = project -> getAudioFiles ();
				for ( unsigned int i=0; i < list -> size(); i++ )
				{
					if ( ImGui::Selectable (list -> at(i).c_str()) )
					{
						cout << "selecting " << list -> at(i).c_str() << endl;
						Listener::openFile (project -> getAudioDir() + "/" + list -> at(i));
					}
					
					if ( ImGui::IsItemActive() )
					{
						if ( ImGui::IsMouseDragging() )
						{
							action_drag_clip = true;
							details.dragged_audio_file = list -> at(i);
						}
						else
						{
							action_drag_clip = false;
							details.dragged_audio_file = "";
						}
					}
				}
			}
			//displayMidiClipSet (project);
			break;
		default:
			break;
	}
	ImGui::EndChild();
}

void TitleScreen () {}

void ProjectTitle ()
{
	ImGui::Text("   ##  ##  #   # ###  ## ###");
	ImGui::Text("  # # ##  # #  # #   #    #");
	ImGui::Text("  #   # #  #  #   ##  ##  #");
}

void ProjectScreen (Project* project)
{
	BeginScreen ();
	mainMenu (project);
	ControlPanel (project, ProjectTitle);
	
	// MAIN BOARD
	ImGui::BeginChild("Clips", ImVec2(0, 0), true);
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
	for ( unsigned int i = 0; i < project -> getMIDI () -> getClipSet() -> size(); i++ )
	{
		MidiClip * daClip = project -> getMIDI () -> getClipSet() -> at(i);
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
				project -> getMIDI () -> deleteClip (i);
			}
			ImGui::EndPopup();
		}

		ImGui::EndChild();
		if ( lock ) ImGui::PopStyleColor();
		ImGui::PopID();
	}
	
	ImGui::PopStyleVar ();
	ImGui::EndChild ();

	EndScreen();
}

void ConsoleTitle ()
{
	ImGui::Text("   ##  #   #    #  #  #  ##");
	ImGui::Text("  #   # # # #  #  # # #  #  ");
	ImGui::Text("   ##  #  # # #    #  ## ##");
}

void ConsoleScreen (Project* project) 
{	
	BeginScreen ();
	mainMenu (project);
	ControlPanel (project, ConsoleTitle);
	
	if ( ressources_panel )
	{
		RessourcesPanel (project);
		ImGui::SameLine ();
	}
	int w = ressources_panel * width5;
	ImGui::BeginChild ("Board", ImVec2(w, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
	Track * asking_track = NULL;
	for (unsigned int i=0; i < project -> nTracks (); i++)
	{ // Tracks
		
		//switch ( track -> getType () )
		//{
		//case AUDIO:
			//break;
		//case MIDI:
			//break;
		//default:
			//break;
		//}
		
		Track * track = project -> getTrack(i);
		std::string type_str = " " + track -> getTypeStr () + " ";
		float value = 0.2f;
		if ( details.selected_track == track ) value = 0.4f;
		ImColor track_bg = ImColor::HSV(track -> getHue (), 0.5f, value, 1.00f);
		char child_id[32];
		sprintf(child_id, "Track%03d", i*5731);
		
		if ( i > 0 ) ImGui::SameLine ();

		ImGui::PushStyleVar (ImGuiStyleVar_ChildWindowRounding, 0.0f);
		ImGui::PushStyleColor (ImGuiCol_ChildWindowBg, track_bg);
		ImGui::BeginChild (child_id, ImVec2(150, 0), true);
		
		//if ( action_drag_clip and ImGui::IsMouseHoveringWindow () ) asking_track = track;
		if ( ImGui::IsMouseHoveringWindow () ) asking_track = track;
		 
		if ( ImGui::Button("X") ) project -> deleteTrack (i);
		else
		{ // Display Track
			ImGui::SameLine (); ImGui::Text("%02d", i+1);
			ImGui::SameLine (); ImGui::Text("%s", type_str.c_str());
			ImGui::SameLine ();
			if ( i == 0 ) 
				ImGui::Button(" ");
			else
				if ( ImGui::Button("<") ) project -> swapTracks (i, i-1);

			ImGui::SameLine ();
			if ( i == project->nTracks()-1 ) 
				ImGui::Button(" ");
			else
				if ( ImGui::Button(">") ) project -> swapTracks (i, i+1);
			
			ImGui::Text ( "%s", track -> getName().c_str() );
			if ( ImGui::BeginPopupContextItem ("rename track") )
			{
				char buf[20];
				sprintf (buf, "%s", track -> getName().c_str());
				if ( ImGui::InputText ("track name", buf, IM_ARRAYSIZE(buf), ImGuiInputTextFlags_AutoSelectAll|ImGuiInputTextFlags_EnterReturnsTrue) ) 
				{
					track -> setName(buf);
					ImGui::CloseCurrentPopup ();
				}
				ImGui::EndPopup ();
			}
			
			//ImGui::SliderFloat ("Color", project->getTrack(i)->getHueP(), 0.0f, 1.0f);
			ImGui::Separator ();
			// Clips
			for ( unsigned int j=0; j < track -> nClips(); j++)
			{
				Clip * clip = track -> getClip(j);
				ImGui::PushID (j);
				if ( ImGui::Button(">") ) clip -> arm() ; 
				ImGui::SameLine (); 
				if ( ImGui::Button (clip -> getName().c_str(), ImVec2(-1.0f, 0.0f)) ) 
				{
					State::getInstance() -> setTrack (track); 
					State::getInstance() -> setClip (clip); 
					ressources_panel = true;
				}
				ImGui::PopID ();
			}
		}
		
		ImGui::EndChild ();
		ImGui::PopStyleColor ();
		ImGui::PopStyleVar ();
	}
	if ( details.selected_track != asking_track ) details.selected_track = asking_track;
	if ( ImGui::IsMouseReleased(0) )
	{
		cout << "mouse released" << endl;
		if ( action_drag_clip )
		{
			cout << "releasing clip" << endl;
			if ( details.selected_track )
			{
				std::string path = project -> getAudioDir() + "/" + details.dragged_audio_file.c_str();
				const std::type_info &t=typeid(*details.selected_track);
				std::cout << "adding Audioclip " << path << " to " << t.name() << " " << details.selected_track -> getName () << endl;
				AudioClip * clip = new AudioClip (path);
				details.selected_track -> addClip (clip);
				std::cout << "Track has " << details.selected_track -> nClips() << " clips" << endl;
			}
			details.dragged_audio_file = "";
			action_drag_clip = false;
		}
	}
	if ( action_drag_clip ) DragClipOverlay (details.dragged_audio_file, &action_drag_clip);

	ImGui::EndChild ();
	
	EndScreen ();
}

void SequencerTitle ()
{
	ImGui::Text("  # ##  #  # # ## ##   ## ##  ##");
	ImGui::Text(" #  #  # # # # #  # # #   #  ##");
	ImGui::Text("#   ##  ## ##  ## # #  ## ## # #");
}

void SequencerScreen (Project* project) 
{
	BeginScreen ();
	mainMenu (project);
	ControlPanel (project, SequencerTitle);

	if ( ressources_panel )
	{
		RessourcesPanel (project);
		ImGui::SameLine ();
	}
	// TODO : Afficher les pistes
	int w = ressources_panel * width5;
	ImGui::BeginChild ("Board", ImVec2(w, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
	ImGui::EndChild ();
	
	EndScreen ();
}

//======================================================================


void GUI_Main(Project* project)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSdl_ProcessEvent(&event);
		switch ( event.type )
		{
			case SDL_QUIT:
				State::getInstance() -> halt();
				break;
				
			case SDL_KEYDOWN:
				switch ( event.key.keysym.scancode )
				{
					case SDL_SCANCODE_F1:
						details.type = Screen::PROJECT;
						break;
					case SDL_SCANCODE_F2:
						details.type = Screen::CONSOLE;
						break;
					case SDL_SCANCODE_F3:
						details.type = Screen::SEQUENCER;
						break;
					case SDL_SCANCODE_F5:
						//if ( project -> ctrlPressed () ) project -> addAudioTrack ("AudioTrack");
						project -> addAudioTrack ("AudioTrack");
						break;
					case SDL_SCANCODE_F6:
						//if ( project -> ctrlPressed () ) project -> addMidiTrack ("MidiTrack");
						project -> addMidiTrack ("MidiTrack");
						break;
					case SDL_SCANCODE_LCTRL:
					case SDL_SCANCODE_RCTRL:
						project -> ctrlDown ();
						break;
					default:
						break;
				}
				break;
			
			case SDL_KEYUP:
				switch ( event.key.keysym.scancode )
				{
					case SDL_SCANCODE_LCTRL:
					case SDL_SCANCODE_RCTRL:
						project -> ctrlUp ();
						break;
					default:
						break;
				}
		}
	}

	ImGui_ImplSdl_NewFrame(window);
	
	// Main window
	switch ( details.type )
	{
		case Screen::RESSOURCES:
			ProjectScreen (project);
			break;
			
		case Screen::CONSOLE:
			ConsoleScreen (project);
			break;
		
		case Screen::SEQUENCER:
			SequencerScreen (project);
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

