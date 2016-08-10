#include "MidiGroup.h"

//-------------
// Constructor 
//-------------

MidiGroup::MidiGroup ()
{
	ClipSet = new std::vector<MidiClip *>;
}


//------------
// Destructor 
//------------

MidiGroup::~MidiGroup ()
{
    //delete each audio file object (and corresponding buffer, etc.)
    if (ClipSet != NULL){
        for (unsigned int i = 0; i < ClipSet->size (); i++)
        {
            delete ClipSet->at(i);
        }
    }
}


//---------------------------------------------------------------------------
// Access file set externally (note this is not thread safe) ??? advise for AudioGroup
//---------------------------------------------------------------------------
std::vector<MidiClip *> * MidiGroup::getClipSet()
{
    return this->ClipSet;
}

//------------
// Add a clip
//------------

void MidiGroup::addClip(MidiClip * daClip)
{
		ClipSet->push_back (daClip);
}

//unsigned long MidiGroup::getLength()
//{
	//return 1;
//}

void MidiGroup::tick (RtMidiOut * midiout)
{
	for (unsigned int i = 0; i < ClipSet->size (); i++)
		if (ClipSet->at(i)->getState () == CS_PLAYING)
			ClipSet->at(i)->tick (midiout);
}
