#ifndef INC_AUDIOTRACK_H
#define INC_AUDIOTRACK_H

#include <string>
#include <vector>
#include <RtAudio.h>
#include "Clip.hpp"
#include "Track.hpp"


class AudioTrack : public Track
{
	
public:
	// Constructor 
	AudioTrack (std::string s);
	// Destructor 
	~AudioTrack ();

    void addClip (std::string path);
    void deleteClip (unsigned int i);
    inline Clip * getClip (unsigned int i) { return clipset_ -> at(i); }
    inline unsigned int nClips () { return clipset_ -> size(); }
    void stopAll ();
    inline RtAudio * getOutput () { return output_; }
    inline void setOutput (RtAudio * o) { output_ = o; }
	inline float * getVolume () { return &volume_; }
    stk::StkFloat tick ();
    
    
protected:
	float volume_;
	std::vector<AudioClip *> * clipset_;
	RtAudio * output_;

};

#endif
