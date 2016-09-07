#include "MidiTrack.hpp"

//-------------
// Constructor 
//-------------

MidiTrack::MidiTrack (std::string s) : Track (s)
{
	ClipSet = new std::vector<MidiClip *>;
}


//------------
// Destructor 
//------------

MidiTrack::~MidiTrack ()
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

void MidiTrack::addClip(MidiClip * daClip)
{
		ClipSet->push_back (daClip);
}

void MidiTrack::deleteClip (unsigned int i)
{
	MidiClip * clip = ClipSet -> at(i);
	ClipSet -> erase (ClipSet -> begin() + i);
	delete clip;
}

void MidiTrack::tick (RtMidiOut * midiout)
{
	for (unsigned int i = 0; i < ClipSet->size (); i++)
		if (ClipSet->at(i)->getState () == Clip::PLAYING)
			ClipSet->at(i)->tick (midiout);
}

void MidiTrack::stopAll ()
{
	for (unsigned int i = 0; i < ClipSet->size(); i++)
		ClipSet -> at(i) -> stop();
}
		
