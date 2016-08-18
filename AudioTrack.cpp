#include "AudioTrack.hpp"

//-------------
// Constructor 
//-------------

AudioTrack::AudioTrack ()
{
	ClipSet = new std::vector<AudioClip *>;
}


//------------
// Destructor 
//------------

AudioTrack::~AudioTrack ()
{
    //delete each audio file object (and corresponding buffer, etc.)
	if ( ClipSet != NULL )
		for (unsigned int i=0; i < ClipSet -> size(); i++)
			delete ClipSet -> at (i);
}


//------------
// Add a clip
//------------

void AudioTrack::addClip (std::string path)
{
	ClipSet -> push_back (new AudioClip(path));
}

void AudioTrack::deleteClip (unsigned int i)
{
	AudioClip * clip = ClipSet -> at(i);
	ClipSet -> erase (ClipSet -> begin() + i);
	delete clip;
}

void AudioTrack::stopAll ()
{
	for ( unsigned int i=0; i < ClipSet -> size(); i++ )
		ClipSet -> at(i) -> setState (CS_STOPPED);
}
		
		
