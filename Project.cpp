#include <algorithm>
#include <dirent.h>

#include <spdlog/spdlog.h>

#include "Project.hpp"
#include "Clock.hpp"
#include "State.hpp"
#include "Modules.hpp"
#include "AudioFile.hpp"
#include "MidiFile.hpp"


Project::Project (std::string str)
{
	name_ = str;
	dir_ = USER_DIR;
	dir_ += "/" + str;
	file_ = dir_ + "/" + name_ + FILE_EXT;
	clock_ = new Clock ();
	audio_ = new AudioModule ();
	midi_ = new MidiModule ();
	audiofiles_ = new std::vector<AudioFile *>;
	midifiles_ = new std::vector<MidiFile *>;
	updateRessources ();
	saved_ = false;
	ctrl_ = false; // à passer dans State
}

Project::~Project () {
	while (tracks_.size())
		deleteTrack(0);
	for (unsigned int i=0; i < audiofiles_ -> size(); i++)
		delete audiofiles_ -> at (i);
	delete audiofiles_;
	delete midifiles_;
	delete audio_;
	delete midi_;
	delete clock_;
}

//TODO : factoriser
void Project::updateRessources ()
{
	auto mainlog= spdlog::get("main");	
	struct dirent* daFile = NULL;
	int resmkdir;
	
	DIR * projectdir = opendir (dir_.c_str());
	if (projectdir == NULL)
	{
		mainlog->error("Création du dossier {}", dir_.c_str());
		resmkdir = mkdir (dir_.c_str(), S_IRWXU | S_IRWXG);
		if ( resmkdir )
		{
			mainlog->error("Impossible de créer le dossier {}", dir_.c_str());
			exit(1);
		}
	}
	
	//std::string path = dir_ + "/Audio";
	DIR * audiodir = opendir (getAudioDir().c_str());
	if (audiodir == NULL)
	{
		mainlog->error("Création du dossier {}", getAudioDir().c_str());
		resmkdir = mkdir (getAudioDir().c_str(), S_IRWXU | S_IRWXG);
		if ( resmkdir )
		{
			mainlog->error("Impossible de créer le dossier {}", getAudioDir().c_str());
			exit(1);
		}
		//mainlog->error("Impossible d'ouvrir le dossier {}", getAudioDir().c_str());
	}
	
	while ((daFile = readdir(audiodir)) != NULL)
		if ( daFile -> d_type == DT_REG )
		{
			std::string str = daFile -> d_name;
			std::size_t found = str.find_last_of(".");
			if ( (str.substr(found+1) == "wav") or (str.substr(found+1) == "WAV") )
			{
				audiofiles_ -> push_back(new AudioFile(getAudioDir() + "/" +str));
				mainlog->info("Audio ressource : {}", str);
			}
		}
	
	closedir (audiodir);
	
	//path = dir_ + "/MIDI";
	DIR * mididir = opendir (getMIDIDir().c_str());
	if (mididir == NULL)
	{
		mainlog->error("Création du dossier {}", getMIDIDir().c_str());
		resmkdir = mkdir (getMIDIDir().c_str(), S_IRWXU | S_IRWXG);
		if ( resmkdir )
		{
			mainlog->error("Impossible de créer le dossier {}", getMIDIDir().c_str());
			exit(1);
		}
		//mainlog->error("Impossible d'ouvrir le dossier {}", getMIDIDir().c_str());
	}
	
	daFile = NULL;
	while ((daFile = readdir(mididir)) != NULL)
		if ( daFile -> d_type == DT_REG )
		{
			std::string str = daFile -> d_name;
			std::size_t found = str.find_last_of(".");
			if ( (str.substr(found+1) == "mid") or (str.substr(found+1) == "MID") )
			{
				midifiles_ -> push_back(new MidiFile(getMIDIDir() + "/" +str));
				mainlog->info("MIDI ressource : {}", str);
			}
		}
	
	closedir (mididir);
	
}

void Project::addAudioTrack ( std::string s )
{
	std::shared_ptr<Track> track = audio_ -> addTrack (s);
	//std::shared_ptr<Track> track = new AudioTrack(s);
	tracks_.push_back (track);
	State::getInstance() -> setTrack(track);
}

void Project::addMidiTrack ( std::string s )
{
	std::shared_ptr<Track> track = midi_ -> addTrack (s);
	//std::shared_ptr<Track> track = new MidiTrack(s);
	tracks_.push_back (track);
	State::getInstance() -> setTrack(track);
}

void Project::deleteTrack (unsigned int i)
{
	std::shared_ptr<Track> track = tracks_[i];
	tracks_.erase (tracks_.begin() + i);
	switch ( track -> dataType () )
	{
	case AUDIO:
		audio_ -> deleteTrack (std::static_pointer_cast<AudioTrack>(track));
		//audio_ -> deleteTrack (track);
		break;
	case MIDI:
		midi_ -> deleteTrack (std::static_pointer_cast<MidiTrack>(track));
		break;
	default:
		break;
	}
}

void Project::swapTracks ( unsigned int i, unsigned int j )
{
	//std::iter_swap (tracks_->begin()+i, tracks_->begin()+j);
	std::swap ( tracks_[i], tracks_[j] );
}

