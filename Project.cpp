#include <stdlib.h> //getenv
#include <algorithm>
#include <dirent.h>

#include "Project.hpp"
#include "Clock.hpp"
#include "State.hpp"
#include "Modules.hpp"
#include "AudioFile.hpp"
#include "MidiFile.hpp"

DIR * testandcreatedir (std::string s)
{
	auto mainlog = spdlog::get("main");	
	int resmkdir;
	DIR * dir = opendir (s.c_str());
	if (dir == NULL)
	{
		mainlog->info("Création du dossier {}", s.c_str());
		resmkdir = mkdir (s.c_str(), S_IRWXU | S_IRWXG);
		if ( resmkdir )
		{
			mainlog->error("Impossible de créer le dossier {}", s.c_str());
			exit(1);
		}
		else dir = opendir (s.c_str());
	}
	return dir;
}

Project::Project (std::string str)
{
	name_ = str;
	dir_ = user_dir ();
	testandcreatedir (dir_);
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

Project::~Project ()
{
	while (tracks_.size())
		tracks_.pop_back();
	for (unsigned int i=0; i < audiofiles_ -> size(); i++)
		delete audiofiles_ -> at (i);
	delete audiofiles_;
	delete midifiles_;
	delete audio_;
	delete midi_;
	delete clock_;
}

void Project::tick ()
{
	for ( unsigned int j = 0; j < nTracks(); j++ )
	{
		if ( getTrack(j) -> dataType() == MIDI )
		{
			std::shared_ptr<MidiTrack> daTrack = std::static_pointer_cast<MidiTrack>( getTrack(j) );
			if ( daTrack -> isPlaying() )
				daTrack -> tick ();
		}
	}
}

void Project::addAudioFile (std::string s)
{
	audiofiles_ -> push_back(new AudioFile (s.c_str()));
}

void Project::addMidiFile (std::string s)
{
	midifiles_ -> push_back(new MidiFile (s.c_str()));
}

//TODO : factoriser
void Project::updateRessources ()
{
	auto mainlog= spdlog::get("main");	
	struct dirent* daFile = NULL;
	testandcreatedir (dir_);
	
	DIR * audiodir = testandcreatedir (getAudioDir());
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
	
	DIR * mididir = testandcreatedir (getMidiDir());
	daFile = NULL;
	while ((daFile = readdir(mididir)) != NULL)
		if ( daFile -> d_type == DT_REG )
		{
			std::string str = daFile -> d_name;
			std::size_t found = str.find_last_of(".");
			if ( (str.substr(found+1) == "mid") or (str.substr(found+1) == "MID") )
			{
				midifiles_ -> push_back(new MidiFile(getMidiDir() + "/" +str));
				mainlog->info("MIDI ressource : {}", str);
			}
		}
	closedir (mididir);

}

void Project::addAudioTrack ( std::string s )
{
	std::shared_ptr<Track> track (new AudioTrack(s));
	tracks_.push_back (track);
	State::getInstance() -> setTrack(track);
}

void Project::addMidiTrack ( std::string s )
{
	std::shared_ptr<Track> track (new MidiTrack(s));
	tracks_.push_back (track);
	State::getInstance() -> setTrack(track);
}

void Project::deleteTrack (unsigned int i)
{
	tracks_.erase (tracks_.begin() + i);
}

void Project::swapTracks ( unsigned int i, unsigned int j )
{
	//std::iter_swap (tracks_->begin()+i, tracks_->begin()+j);
	std::swap ( tracks_[i], tracks_[j] );
}

