#ifndef INC_AUDIOTRACK_H
#define INC_AUDIOTRACK_H

#include <string>
#include <vector>
#include <RtAudio.h>
#include "Clip.hpp"


// TODO : convertir en piste
 
class AudioTrack
{
	
public:
	// Constructor 
	AudioTrack();
	// Destructor 
	~AudioTrack();

	inline void setState (bool s) { state_ = s; }
	inline bool isPlaying () { return state_; }
    void addClip (std::string path);
    void deleteClip (unsigned int i);
    void stopAll ();
    inline std::vector<AudioClip *> * getClipSet () { return ClipSet; }
    inline RtAudio * getOutput () { return output_; }
    inline void setOutput (RtAudio * o) { output_ = o; }
    
protected:
	bool state_;
	std::vector<AudioClip *> * ClipSet;
	RtAudio * output_;

};

#endif
