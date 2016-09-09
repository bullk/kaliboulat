#ifndef INC_PROJECT_H
#define INC_PROJECT_H

#include <iostream>
#include <string>
#include <vector>
#include <dirent.h>

#ifndef WIN32
    #include <sys/types.h>
#endif 

#include "globals.h"
#include "Clock.hpp"
#include "Modules.hpp"


//void SplitFilename (const std::string& str)
//{
  //std::size_t found = str.find_last_of("/\\");
  //std::cout << " path: " << str.substr(0,found) << '\n';
  //std::cout << " file: " << str.substr(found+1) << '\n';
//}

class Project
{

public:
	Project (std::string str);
	~Project ();
	void updateRessources ();
	//void importAll ();
	inline std::string getName () { return name_; }
	inline Clock * getClock () { return clock_; }
	inline AudioModule * getAudio () { return audio_; }
	inline std::string getAudioDir () { return dir_ + "/Audio"; }
	inline std::string getMIDIDir () { return dir_ + "/MIDI"; }
	void addTrack (std::string s);
	void deleteTrack (unsigned int i);
	void swapTracks (unsigned int i, unsigned int j);
	inline unsigned int nTracks () { return tracks_.size (); }
	inline Track * getTrack (unsigned int i) { return tracks_[i]; }
	inline std::vector<std::string> * getAudioFiles () { return audiofiles_; }
	inline std::vector<std::string> * getMIDIFiles () { return midifiles_; }
	
protected:
	std::string name_;
	std::string dir_;
	std::string file_;
	Clock * clock_;
	AudioModule * audio_;
	std::vector<Track *> tracks_;
	std::vector<std::string> * audiofiles_;
	std::vector<std::string> * midifiles_;
};

#endif
