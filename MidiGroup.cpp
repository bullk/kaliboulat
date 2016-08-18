#include "MidiGroup.hpp"

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


//------------
// Add a clip
//------------

void MidiGroup::addClip(MidiClip * daClip)
{
		ClipSet->push_back (daClip);
}

void MidiGroup::deleteClip (unsigned int i)
{
	MidiClip * clip = ClipSet -> at(i);
	ClipSet -> erase (ClipSet -> begin() + i);
	delete clip;
}

void MidiGroup::tick (RtMidiOut * midiout)
{
	for (unsigned int i = 0; i < ClipSet->size (); i++)
		if (ClipSet->at(i)->getState () == CS_PLAYING)
			ClipSet->at(i)->tick (midiout);
}

void MidiGroup::stopAll ()
{
	for (unsigned int i = 0; i < ClipSet->size(); i++)
		ClipSet->at(i)->setState(CS_STOPPED);
}
		
