//#include <stdlib.h>
#include <spdlog/spdlog.h>
#include "AudioTrack.hpp"


//-------------
// Constructor
//-------------

AudioTrack::AudioTrack () : Track (AUDIO, "Audio", "AudioTrack"), volume_(1.0f)
{
	//clipset_ = new std::vector<std::shared_ptr<AudioClip>>;
	hue_ =  0.25f + (float)((rand() % 31) -15) / 100 ;
}

AudioTrack::AudioTrack (std::string s) : Track (AUDIO, "Audio", s), volume_(1.0f)
{
	//clipset_ = new std::vector<std::shared_ptr<AudioClip>>;
	hue_ =  0.25f + (float)((rand() % 31) -15) / 100 ;
}


//------------
// Destructor 
//------------

AudioTrack::~AudioTrack ()
{
	spdlog::get("main")->info("Deleting {} track {}", type_str_, name_);
	while (clipset_.size())
		deleteClip(0);
    //delete clipset_;
    //clipset_ = NULL;
}


//------------
// Add a clip
//------------

void AudioTrack::addClip (std::shared_ptr<Clip> clip)
{
	addClip ( std::static_pointer_cast<AudioClip>(clip)  );
}

void AudioTrack::addClip (std::shared_ptr<AudioClip> clip)
{
	//std::cout << "AudioTrack::addClip" << clipset_.size() << std::endl;
	clipset_.push_back (clip);
}

void AudioTrack::deleteClip (unsigned int i)
{
	std::shared_ptr<AudioClip> clip = clipset_.at(i);
	clipset_.erase (clipset_.begin() + i);
	//delete clip;
}

void AudioTrack::stopAll ()
{
	for ( unsigned int i=0; i < clipset_.size(); i++ )
		clipset_.at(i) -> stop();
}
		
stk::StkFloat AudioTrack::tick() const
{
	register stk::StkFloat sample;

	sample = 0;
	for ( unsigned int j = 0; j < clipset_.size (); j++ )
	{
		std::shared_ptr<AudioClip> clip = clipset_.at (j);
		if ( clip -> isPlaying () )
			sample += clip -> tick () * *(clip -> getVolume ());
	}
	return sample;
}


