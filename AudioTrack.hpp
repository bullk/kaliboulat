#ifndef INC_AUDIOTRACK_H
#define INC_AUDIOTRACK_H

//#include <iostream> // cout, endl
#include <vector>
#include <RtAudio.h>
#include "AudioClip.hpp"
#include "Track.hpp"

#include <cereal/types/vector.hpp>
//#include <cereal/archives/binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/types/polymorphic.hpp>

#include <typeinfo>  //for 'typeid' to work  

class AudioTrack : public Track
{
	
public:
	// Constructor 
	AudioTrack (std::string);
	AudioTrack (std::string, float, float, std::vector<std::shared_ptr<AudioClip>>);
	// Destructor 
	~AudioTrack ();

	void addClip (std::string, int);
	void addClip (std::shared_ptr<Clip> clip);
	void addClip (std::shared_ptr<AudioClip> clip);
	void deleteClip (unsigned int i);
	inline std::shared_ptr<Clip> getClip (unsigned int i) const { return clipset_[i]; }
	inline unsigned int nClips () const { return clipset_.size(); }
	void stopAll ();
	inline RtAudio * getOutput () const { return output_; }
	inline void setOutput (RtAudio * o) { output_ = o; }
	inline float * getVolume () { return &volume_; }
	stk::StkFloat tick () const;
	
	template <class Archive>
	void serialize(Archive & archive)
	{
		archive (
			CEREAL_NVP(name_),
			CEREAL_NVP(hue_),
			CEREAL_NVP(volume_),
			CEREAL_NVP(clipset_)
		);
	}
	
	template <class Archive>
	static void load_and_construct( Archive & archive, cereal::construct<AudioTrack> & construct )
	{
		std::string name;
		float hue, volume;
		std::vector<std::shared_ptr<AudioClip>> clipset;
		archive( name, hue, volume, clipset );
		construct( name, hue, volume, clipset );
	}

protected:
	float volume_;
	unsigned int sl_port_;
	pid_t sl_pid_;
	std::vector<std::shared_ptr<AudioClip>> clipset_;
	RtAudio * output_;
	void startSL();


};

CEREAL_REGISTER_TYPE (AudioTrack)
CEREAL_REGISTER_POLYMORPHIC_RELATION (Track, AudioTrack)

#endif
