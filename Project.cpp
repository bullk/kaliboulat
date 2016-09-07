#include "Project.hpp"

Project::Project (std::string str)
{
	name_ = str;
	dir_ = USER_DIR;
	dir_ += "/" + str;
	file_ = dir_ + "/" + name_ + FILE_EXT;
	audiofiles_ = new std::vector<std::string>;
	midifiles_ = new std::vector<std::string>;
	updateRessources ();
}

Project::~Project () {}

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

void Project::addTrack (Track * t)
{
	tracks_.push_back (t);
}

void Project::deleteTrack (unsigned int i)
{
	tracks_.erase (tracks_.begin() + i);
}
