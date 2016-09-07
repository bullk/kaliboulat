#include "AudioTrack.hpp"

//-------------
// Constructor 
//-------------

AudioTrack::AudioTrack (std::string s) : Track (s)
{
	clipset_ = new std::vector<AudioClip *>;
	volume_ = 1.0f;
}


//------------
// Destructor 
//------------

AudioTrack::~AudioTrack ()
{
    //delete each audio file object (and corresponding buffer, etc.)
	if ( clipset_ != NULL )
		for (unsigned int i=0; i < clipset_ -> size(); i++)
			delete clipset_ -> at (i);
}


//------------
// Add a clip
//------------

void AudioTrack::addClip (std::string path)
{
	clipset_ -> push_back (new AudioClip(path));
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
		AudioClip * daClip = clipset_ -> at (j);
		if ( daClip -> isPlaying () )
			sample += daClip -> tick () * *(daClip -> getVolume ());
	}
	return sample;
}

		
