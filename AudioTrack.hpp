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
	AudioTrack (std::string s);
	// Destructor 
	~AudioTrack ();

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
	static void load_and_construct( Archive & ar, cereal::construct<AudioTrack> & construct )
	{
		std::string name;
		ar( name );
		construct( name );
	}
	
     template<class Archive>
	void serialize(Archive & archive)
	{
		archive (
			//CEREAL_NVP(data_type_),
			//CEREAL_NVP(type_str_),
			CEREAL_NVP(name_),
			CEREAL_NVP(hue_),
			CEREAL_NVP(volume_),
			CEREAL_NVP(clipset_)
		);
	}

protected:
	float volume_;
	std::vector<std::shared_ptr<AudioClip>> clipset_;
	RtAudio * output_;

};

CEREAL_REGISTER_TYPE(AudioTrack)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Track, AudioTrack)

#endif
