#ifndef INC_PROJECT_H
#define INC_PROJECT_H

#include <iostream>
#include <string>
#include <vector>

#ifndef WIN32
    #include <sys/types.h>
#endif 

#include "Track.hpp"
#include <cereal/archives/xml.hpp>

//void SplitFilename (const std::string& str)
//{
  //std::size_t found = str.find_last_of("/\\");
  //std::cout << " path: " << str.substr(0,found) << '\n';
  //std::cout << " file: " << str.substr(found+1) << '\n';
//}

class Clock;
class AudioFile;
class MidiFile;

class Project
{

public:
	Project( std::string str );
	~Project();
	void updateRessources();
	inline std::string getName() { return name_; }
	inline std::string getFile() { return file_; }
	inline Clock * getClock() { return clock_; }
	void tick();
	inline std::string getAudioDir() { return dir_ + "/Audio"; }
	inline std::string getMidiDir() { return dir_ + "/MIDI"; }
	void addAudioTrack( std::string );
	void addMidiTrack( std::string );
	void addSLBus( std::string );
	void deleteTrack( unsigned int i );
	void swapTracks( unsigned int i, unsigned int j );
	inline unsigned int nTracks() { return tracks_.size(); }
	inline std::shared_ptr<Track> getTrack( unsigned int i ) { return tracks_[i]; }
	void addAudioFile( std::string s );
	inline std::vector<AudioFile *> * getAudioFiles() { return &audiofiles_; }
	void addMidiFile( std::string s );
	inline std::vector<MidiFile *> * getMidiFiles() { return &midifiles_; }
	inline bool ctrlPressed() { return ctrl_; }
	inline void ctrlDown() { ctrl_ = true; }
	inline void ctrlUp() { ctrl_ = false; }
	
	template<class Archive>
	void serialize( Archive & archive )
	{
		archive (
			CEREAL_NVP(name_),
			CEREAL_NVP(tracks_)
		);
	}
	
protected:
	std::string name_;
	std::string dir_;
	std::string file_;
	Clock * clock_;
	std::vector<std::shared_ptr<Track>> tracks_;
	std::vector<AudioFile *> audiofiles_;
	std::vector<MidiFile *> midifiles_;
	bool saved_;
	bool ctrl_; // à passer dans State
	
};



#endif
