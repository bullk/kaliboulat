#include "AudioGroup.h"

//-------------
// Constructor 
//-------------

AudioGroup::AudioGroup ()
{
	ClipSet = new std::vector<AudioClip *>;
}


//------------
// Destructor 
//------------

AudioGroup::~AudioGroup ()
{
    //delete each audio file object (and corresponding buffer, etc.)
	if ( ClipSet != NULL )
		for (unsigned int i=0; i < ClipSet -> size(); i++)
			delete ClipSet -> at (i);
}


//---------------------------------------------------------------------------
// Access file set externally (note this is not thread safe)
//---------------------------------------------------------------------------
std::vector<AudioClip *> * AudioGroup::getClipSet ()
{
    return ClipSet;
}

//------------
// Add a clip
//------------

void AudioGroup::addClip (std::string path)
{
	ClipSet -> push_back (new AudioClip(path));
}

void AudioGroup::deleteClip (unsigned int i)
{
	AudioClip * clip = ClipSet -> at(i);
	ClipSet -> erase (ClipSet -> begin() + i);
	delete clip;
}

void AudioGroup::stopAll ()
{
	for ( unsigned int i=0; i < ClipSet -> size(); i++ )
		ClipSet -> at(i) -> setState (CS_STOPPED);
}
		
		
