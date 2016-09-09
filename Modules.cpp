#include "Modules.hpp"


//-------------
// Constructor 
//-------------

AudioModule::AudioModule ()
{
	trackset_ = new std::vector<AudioTrack *>;
	clipset_ = new std::vector<AudioClip *>;
}


//------------
// Destructor 
//------------

AudioModule::~AudioModule ()
{
    //delete each audio file object (and corresponding buffer, etc.)
	if ( clipset_ != NULL )
		for (unsigned int i=0; i < clipset_ -> size(); i++)
			delete clipset_ -> at (i);
    //delete each track
	if ( trackset_ != NULL )
		for (unsigned int i=0; i < trackset_ -> size(); i++)
			delete trackset_ -> at (i);
}

//==================

AudioTrack * AudioModule::addTrack (std::string s)
{
	AudioTrack * track = new AudioTrack (s);
	trackset_ -> push_back (track);
	std::cout << "Audio Module has " << trackset_ -> size () << " tracks." << std::endl;
	return track;
}

void AudioModule::deleteTrack (unsigned int i)
{
	AudioTrack * track = trackset_ -> at(i);
	trackset_ -> erase (trackset_ -> begin() + i);
	std::cout << "Audio Module has " << trackset_ -> size () << " tracks." << std::endl;
	delete track;
}

void AudioModule::deleteTrack (AudioTrack * t)
{
	unsigned int i=0;
	while ( trackset_ -> at(i) != t ) i++;
	deleteTrack (i);
}

void AudioModule::addClip (std::string path)
{
	clipset_ -> push_back (new AudioClip(path));
}

void AudioModule::deleteClip (unsigned int i)
{
	AudioClip * clip = clipset_ -> at(i);
	clipset_ -> erase (clipset_ -> begin() + i);
	delete clip;
}

