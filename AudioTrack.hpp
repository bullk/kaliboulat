#ifndef INC_AUDIOTRACK_H
#define INC_AUDIOTRACK_H

//#include <iostream> // cout, endl
#include <string>
#include <vector>
#include <RtAudio.h>
#include "AudioClip.hpp"
#include "Track.hpp"

#include <cereal/archives/xml.hpp>
#include <cereal/types/polymorphic.hpp>

class AudioTrack : public Track
{
	
public:
	// Constructor 
	AudioTrack ();
	AudioTrack (std::string s);
	// Destructor 
	~AudioTrack ();

	void addClip (Clip * clip);
	void addClip (AudioClip * clip);
	void deleteClip (unsigned int i);
	inline AudioClip * getClip (unsigned int i) const { return clipset_ -> at(i); }
	inline unsigned int nClips () const { return clipset_ -> size(); }
	void stopAll ();
	inline RtAudio * getOutput () const { return output_; }
	inline void setOutput (RtAudio * o) { output_ = o; }
	inline float * getVolume () { return &volume_; }
	stk::StkFloat tick () const;
	
    template<class Archive>
	void serialize(Archive & archive)
	{
		archive (
			CEREAL_NVP(data_type_),
			CEREAL_NVP(type_str_),
			CEREAL_NVP(name_),
			CEREAL_NVP(hue_),
			CEREAL_NVP(volume_)
		);
	}
	
	
protected:
	float volume_;
	std::vector<AudioClip *> * clipset_;
	RtAudio * output_;

};

CEREAL_REGISTER_TYPE(AudioTrack)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Track, AudioTrack)

#endif
