#include "AudioGroup.h"

//-------------
// Constructor 
//-------------

AudioGroup::AudioGroup()
{
	ClipSet = new vector<AudioClip *>;
}


//------------
// Destructor 
//------------

AudioGroup::~AudioGroup()
{
    //delete each audio file object (and corresponding buffer, etc.)
    if (ClipSet != NULL){
        for (unsigned int i = 0; i < ClipSet->size(); i++)
        {
            delete ClipSet->at(i);
        }
    }
}


//---------------------------------------------------------------------------
// Access file set externally (note this is not thread safe)
//---------------------------------------------------------------------------
vector<AudioClip *> * AudioGroup::getClipSet()
{
    return this->ClipSet;
}

//------------
// Add a clip
//------------

void AudioGroup::addAclip(std::string path)
{
	ClipSet->push_back(new AudioClip(path));
}
