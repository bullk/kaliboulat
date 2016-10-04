#include <typeinfo>
#include <unistd.h> // sleep
#include "GUI.hpp"

using namespace std;

SDL_GLContext glcontext;
SDL_Window * window = NULL;
ImVec4 clear_color;
//Screen ui = { Screen::CONSOLE, Screen::RESSOURCES, 0, 0, NULL, NULL, NULL };
Screen ui = { Screen::CONSOLE, Screen::RESSOURCES, NULL, NULL };

unsigned int width1, width2, width3, width4, width5;
unsigned int control_panel_columns;

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
	ImGui::PushStyleVar (ImGuiStyleVar_ChildWindowRounding, 0.0f);
	
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

	control_panel_columns = (int) ImGui::GetWindowContentRegionWidth() / 230;
	ImGui::Columns (control_panel_columns);
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

void displayAudioFiles (Project* project)
{
	ImGui::Spacing();
	static bool audio_tree_open = true;
	ImGui::SetNextTreeNodeOpen(audio_tree_open);
	if (ImGui::CollapsingHeader("Audio Files"))
	{
		vector<AudioFile *> * list = project -> getAudioFiles ();
		for ( unsigned int i=0; i < list -> size(); i++ )
		{
			AudioFile * file = list -> at(i);
			if ( ImGui::Selectable (file -> getName().c_str()) )
			{
				cout << "selecting " << file -> getName().c_str() << endl;
				Listener::openFile (file -> getPath());
			}
			
			if ( ImGui::IsItemActive() )
			{
				if ( ImGui::IsMouseDragging() )
				{
					action_drag_clip = true;
					//ui.dragged_clip = list -> at(i);
					ui.dragged_clip = new AudioClip (file -> getPath());
				}
				else
				{
					action_drag_clip = false;
					//ui.dragged_clip = NULL;
					ui.dragged_clip = NULL;
				}
			}
		}
	}
}
	
void displayMidiFiles (Project* project)
{
	vector<MidiFile *> * list = project -> getMIDIFiles ();
	static bool midi_tree_open = true;
	ImGui::Spacing();
	ImGui::SetNextTreeNodeOpen(midi_tree_open);
	if (ImGui::CollapsingHeader("MIDI Files"))
	{
		for ( unsigned int i=0; i < list -> size(); i++ )
		{
			MidiFile * midifile = list -> at(i);
			if ( ImGui::TreeNode(midifile -> getName().c_str()) )
			{
				midifile -> parse ();
				switch ( midifile->getFileFormat() )
				{
					case 1:
					{
						ImGui::Text("%d tracks | %d ticks / beat",
							midifile -> getNumberOfTracks(),
							midifile -> getDivision());
						ImGui::Text("%s", "Master");
						for ( unsigned int j=0; j < midifile -> nClips(); j++ )
						{
							ImGui::Selectable(midifile -> getTrack(j) -> getName().c_str());
							if ( ImGui::IsItemActive() )
							{
								if ( ImGui::IsMouseDragging() )
								{
									action_drag_clip = true;
									//ui.dragged_clip = list -> at(i);
									ui.dragged_clip = midifile -> getTrack(j);
								}
								else
								{
									action_drag_clip = false;
									//ui.dragged_clip = NULL;
									ui.dragged_clip = NULL;
								}
							}
						}
					}
				}
				ImGui::TreePop();
			}
			//delete midifile;
		}
		ImGui::Columns(1);
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

static void DragClipOverlay (bool* p_open)
{
    ImGui::SetNextWindowPos (ImVec2(ImGui::GetIO().MousePos.x + 5, ImGui::GetIO().MousePos.y + 5));
    if (!ImGui::Begin("Drag Clip Overlay", p_open, ImVec2(0,0), 0.3f, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::End();
        return;
    }
    ImGui::Text("%s", ui.dragged_clip -> getName().c_str());
    ImGui::End();
}

void RessourcesPanel (Project* project)
{
	ImGui::BeginChild ("Ressources", ImVec2(width1,0), true);
	if ( ImGui::Button("Files") ) ui.context = Screen::RESSOURCES;
	ImGui::SameLine();
	if ( ImGui::Button("Audio") ) ui.context = Screen::AUDIOCLIP;
	ImGui::SameLine();
	if ( ImGui::Button("MIDI") ) ui.context = Screen::MIDICLIP;
	ImGui::Separator();
	
	switch (ui.context)
	{
		case Screen::RESSOURCES:
			ImGui::Spacing();
			ImGui::Checkbox("LISTENER", Listener::getOnOffP());
			displayAudioFiles (project);
			displayMidiFiles (project);
			break;
		case Screen::AUDIOCLIP:
		if ( State::getInstance() -> getClip() )
		{
			AudioClip * clip = (AudioClip *)State::getInstance() -> getClip();
			ImGui::Text("%s", clip -> getName().c_str());
			ImGui::Text("%s", clip -> getPath().c_str());
			
		}
			break;
		case Screen::MIDICLIP:
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
	ImGui::BeginChild ("MIDI input log", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
	std::vector<MidiRawMessage *> * midilog = Waiter::getInstance()->getMidiLog();
	for ( unsigned int i=0; i<midilog->size(); i++ )
	{
		MidiRawMessage * m = midilog -> at(i);
		ImGui::Text("MIDI IN : ");
		for ( std::vector<unsigned char>::iterator c=m->begin(); c != m->end(); c++ )
		{
			ImGui::SameLine();
			ImGui::Text("%02x ", *c);
		}
	}
	ImGui::EndChild ();
	
	EndScreen();
}

void ConsoleTitle ()
{
	ImGui::Text("   ##  #   #    #  #  #  ##");
	ImGui::Text("  #   # # # #  #  # # #  #  ");
	ImGui::Text("   ##  #  # # #    #  ## ##");
}

void ConsoleClip (Clip * clip, int id, float hue)
{
	ImGui::PushID (id);
	ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(hue, 0.4f, 0.4f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(hue, 0.4f, 0.4f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(hue, 0.4f, 0.4f));
	if ( ImGui::Button(">") ) clip -> arm() ; 
	ImGui::SameLine ();
	if ( ImGui::Button (clip -> getName().c_str(), ImVec2(-1.0f, 0.0f)) ) 
	{
		State::getInstance() -> setClip (clip); 
		ressources_panel = true;
		ui.context = Screen::AUDIOCLIP;
	}
	ImGui::PopStyleColor(3);
	ImGui::PopID ();
}

void ConsoleScreen (Project * project) 
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
	ImGui::PushStyleVar (ImGuiStyleVar_WindowPadding, ImVec2(2, 2));
	ImGui::BeginChild ("Board", ImVec2(w, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
	ImGui::PushStyleVar (ImGuiStyleVar_ItemSpacing, ImVec2(2, 2));
	// DISPLAYING TRACKS
	Track * asking_track = NULL;
	for (unsigned int i=0; i < project -> nTracks (); i++)
	{
		Track * track = project -> getTrack(i);
		// track name/id
		char child_id[32];
		sprintf(child_id, "Track%03d", i);
		
		if ( i > 0 ) ImGui::SameLine ();
		if ( action_drag_clip and ui.dragged_clip->dataType() != track->dataType() ) 
		{
			ImGui::PushStyleColor (ImGuiCol_ChildWindowBg, ImColor::HSV(0.0f, 0.0f, 0.15f));
			ImGui::BeginChild (child_id, ImVec2(150, 0), true);
			ImGui::EndChild ();
			ImGui::PopStyleColor ();
		}
		else
		{
			// background color
			float saturation = 0.3f;
			float value = 0.1f;
			float alpha = 1.00f;
			if ( State::getTrack() == track )
			{
				saturation = 1.0f;
				value = 0.2f;
			}
			ImColor color_bg = ImColor::HSV(track -> getHue (), saturation, value, alpha);
			// begining TrackUI
			ImGui::PushStyleVar (ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
			ImGui::PushStyleColor (ImGuiCol_ChildWindowBg, color_bg);
			ImGui::BeginChild (child_id, ImVec2(150, 0), true);
			ImGui::PushStyleVar (ImGuiStyleVar_ItemSpacing, ImVec2(8, 8));

			if ( ImGui::Button("X") ) project -> deleteTrack (i); // track delete button
			else
			{	// main track UI
				ImGui::SameLine (); ImGui::Text("%02d", i+1); // track number
				std::string type_str = " " + track -> getTypeStr () + " "; // track type
				ImGui::SameLine (); ImGui::Text("%s", type_str.c_str());
				ImGui::SameLine ();
				// move track buttons
				if ( i == 0 ) ImGui::Button(" ");
				else if ( ImGui::Button("<") ) project -> swapTracks (i, i-1); // to left
				ImGui::SameLine ();
				if ( i == project->nTracks()-1 ) ImGui::Button(" ");
				else if ( ImGui::Button(">") ) project -> swapTracks (i, i+1); // to right
				
				ImGui::Text ( "%s", track -> getName().c_str() ); // track name
				if ( ImGui::BeginPopupContextItem ("rename track") ) // renaming popup
				{
					char buf[20];
					sprintf (buf, "%s", track -> getName().c_str());
					if ( ImGui::InputText ("track name", buf, IM_ARRAYSIZE(buf), ImGuiInputTextFlags_AutoSelectAll|ImGuiInputTextFlags_EnterReturnsTrue) ) 
					{
						track -> setName (buf);
						ImGui::CloseCurrentPopup ();
					}
					ImGui::EndPopup ();
				}

				ImGui::Spacing ();
				ImGui::Separator ();
				// displaying Clips
				for ( unsigned int j=0; j < track -> nClips(); j++ )
					ConsoleClip (track->getClip(j), j, track->getHue());
			}
			
			// track selection behavior
			if ( ImGui::IsMouseHoveringWindow () )
			{
				if ( action_drag_clip ) asking_track = track;
				else if ( ImGui::IsMouseClicked(0) ) asking_track = track;
			}

			ImGui::PopStyleVar ();
			ImGui::EndChild ();
			ImGui::PopStyleColor ();
			ImGui::PopStyleVar ();
		}
	}

	// SETTING NEW ACTIVE TRACK
	if ( action_drag_clip and not asking_track ) State::setTrack(NULL);
	if ( asking_track and asking_track != State::getTrack() ) State::setTrack(asking_track);
	asking_track = NULL;

	// DROPPING A CLIP
	if ( action_drag_clip )
	{
		if ( ImGui::IsMouseReleased(0) ) // if mouse released
		{
			if ( State::getTrack() and ui.dragged_clip->dataType() == State::getTrack()->dataType() )
				State::getTrack() -> addClip (ui.dragged_clip);
			ui.dragged_clip = NULL;
			action_drag_clip = false;
		}
		else DragClipOverlay (&action_drag_clip); // display clip overlay
	}

	// CLOSING EVERYTHING
	ImGui::PopStyleVar ();
	ImGui::EndChild ();
	ImGui::PopStyleVar ();
	
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
						ui.type = Screen::PROJECT;
						break;
					case SDL_SCANCODE_F2:
						ui.type = Screen::CONSOLE;
						break;
					case SDL_SCANCODE_F3:
						ui.type = Screen::SEQUENCER;
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
	switch ( ui.type )
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

