#include "Project.hpp"
#include <algorithm>


Project::Project (std::string str)
{
	name_ = str;
	dir_ = USER_DIR;
	dir_ += "/" + str;
	file_ = dir_ + "/" + name_ + FILE_EXT;
	clock_ = new Clock ();
	audio_ = new AudioModule (); // Audio clips manager
	audiofiles_ = new std::vector<std::string>;
	midifiles_ = new std::vector<std::string>;
	updateRessources ();
}

Project::~Project () {
	//if ( tracks_ != NULL )
		//for (unsigned int i=0; i < tracks_ -> size(); i++)
			//delete tracks_ -> at (i);
	delete audio_;
	delete clock_;
}

void Project::updateRessources ()
{
	struct dirent* daFile = NULL;
	
	//std::string path = dir_ + "/Audio";
	DIR * audiodir = opendir (getAudioDir().c_str());
	if (audiodir == NULL)
	{
		printf("Erreur à l'ouverture du dossier '%s'\n", getAudioDir().c_str());
		exit(1);
	}
	
	while ((daFile = readdir(audiodir)) != NULL)
		if ( daFile -> d_type == DT_REG )
		{
			std::string str = daFile -> d_name;
			std::size_t found = str.find_last_of(".");
			if ( (str.substr(found+1) == "wav") or (str.substr(found+1) == "WAV") )
			{
				audiofiles_ -> push_back(str);
				std::cout << "Audio ressource : " << str << std::endl;
			}
		}
	
	closedir (audiodir);
	
	//path = dir_ + "/MIDI";
	DIR * mididir = opendir (getMIDIDir().c_str());
	if (mididir == NULL)
	{
		printf("Erreur à l'ouverture du dossier '%s'\n", getMIDIDir().c_str());
		exit(1);
	}
	
	daFile = NULL;
	while ((daFile = readdir(mididir)) != NULL)
		if ( daFile -> d_type == DT_REG )
		{
			std::string str = daFile -> d_name;
			std::size_t found = str.find_last_of(".");
			if ( (str.substr(found+1) == "mid") or (str.substr(found+1) == "MID") )
			{
				midifiles_ -> push_back(str);
				std::cout << "MIDI ressource : " << str << std::endl;
			}
		}
	
	closedir (mididir);
	
}

void Project::addTrack (std::string s)
{
	tracks_.push_back ( audio_ -> addTrack (s) );
}

void Project::deleteTrack (unsigned int i)
{
	Track * track = tracks_[i];
	tracks_.erase (tracks_.begin() + i);
	switch ( track -> getType () )
	{
	case Track::AUDIO:
		audio_ -> deleteTrack ((AudioTrack *)track);
		break;
	//case Track::MIDI:
		//midi -> deleteTrack ((MidiTrack *)track);
		//break;
	default:
		break;
	}
}

void Project::swapTracks ( unsigned int i, unsigned int j )
{
	//std::iter_swap (tracks_->begin()+i, tracks_->begin()+j);
	std::swap ( tracks_[i], tracks_[j] );
}
