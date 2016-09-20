#include "AudioTrack.hpp"
#include <stdlib.h>


//-------------
// Constructor 
//-------------

AudioTrack::AudioTrack (std::string s) : Track (s)
{
	type_ = AUDIO;
	type_str_ = "Audio";
	clipset_ = new std::vector<AudioClip *>;
	volume_ = 1.0f;
	hue_ =  0.25f + (float)((rand() % 31) -15) / 100 ;
}


//------------
// Destructor 
//------------

AudioTrack::~AudioTrack ()
{
	std::cout << "~AudioTrack()" << std::endl;
	if ( clipset_ != NULL )
		for (unsigned int i=0; i < clipset_ -> size(); i++)
			delete clipset_ -> at (i);
}


//------------
// Add a clip
//------------

void AudioTrack::addClip (AudioClip * clip)
{
	clipset_ -> push_back (clip);
}

void AudioTrack::deleteClip (unsigned int i)
{
	AudioClip * clip = clipset_ -> at(i);
	clipset_ -> erase (clipset_ -> begin() + i);
	delete clip;
}

void AudioTrack::stopAll ()
{
	for ( unsigned int i=0; i < clipset_ -> size(); i++ )
		clipset_ -> at(i) -> stop();
}
		
stk::StkFloat AudioTrack::tick()
{
	register stk::StkFloat sample;

	sample = 0;
	for ( unsigned int j = 0; j < clipset_ -> size (); j++ )
	{
		AudioClip * clip = clipset_ -> at (j);
		if ( clip -> isPlaying () )
			sample += clip -> tick () * *(clip -> getVolume ());
	}
	return sample;
}

		
