#include "MidiGroup.h"

//-------------
// Constructor 
//-------------

MidiGroup::MidiGroup()
{
	ClipSet = new vector<MidiClip *>;
}


//------------
// Destructor 
//------------

MidiGroup::~MidiGroup()
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
// Access file set externally (note this is not thread safe) ??? advise for AudioGroup
//---------------------------------------------------------------------------
vector<MidiClip *> * MidiGroup::getClipSet()
{
    return this->ClipSet;
}

//------------
// Add a clip
//------------

void MidiGroup::addAclip(std::string path)
{
	ClipSet->push_back(new MidiClip(path));
}

unsigned long MidiGroup::getLength()
{
	return 1;
}
