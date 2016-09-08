#ifndef INC_MODULES_H
#define INC_MODULES_H

#include <string>
#include <vector>
#include "AudioTrack.hpp"

class AudioModule
{
	
public:
	// Constructor 
	AudioModule ();
	// Destructor 
	~AudioModule ();

    AudioTrack * addTrack (std::string s);
    void deleteTrack (unsigned int i);
    void deleteTrack (AudioTrack * t);
    //inline AudioTrack * getTrack (unsigned int i) { return trackset_ -> at(i); }
    inline std::vector<AudioTrack *> * getTrackSet () { return trackset_; }
    void addClip (std::string path);
    void deleteClip (unsigned int i);
    inline std::vector<AudioClip *> * getClipSet () { return clipset_; }
    void stopAll ();
    
protected:
	std::vector<AudioTrack *> * trackset_;
	std::vector<AudioClip *> * clipset_;

};

#endif
