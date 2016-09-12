#include "MidiTrack.hpp"

//-------------
// Constructor 
//-------------

MidiTrack::MidiTrack (std::string s) : Track (s)
{
	type_ = MIDI;
	clipset_ = new std::vector<MidiClip *>;
	hue_ =  0.75f + (float)((rand() % 31) -15) / 100 ;
}


//------------
// Destructor 
//------------

MidiTrack::~MidiTrack ()
{
    //delete each audio file object (and corresponding buffer, etc.)
    if (clipset_ != NULL){
        for (unsigned int i = 0; i < clipset_->size (); i++)
        {
            delete clipset_->at(i);
        }
    }
}


//------------
// Add a clip
//------------

void MidiTrack::addClip(MidiClip * daClip)
{
		clipset_->push_back (daClip);
}

void MidiTrack::addClip (std::string path)
{
}

void MidiTrack::deleteClip (unsigned int i)
{
	MidiClip * clip = clipset_ -> at(i);
	clipset_ -> erase (clipset_ -> begin() + i);
	delete clip;
}

void MidiTrack::tick (RtMidiOut * midiout)
{
	for (unsigned int i = 0; i < clipset_->size (); i++)
		if (clipset_->at(i)->getState () == Clip::PLAYING)
			clipset_->at(i)->tick (midiout);
}

void MidiTrack::stopAll ()
{
	for (unsigned int i = 0; i < clipset_->size(); i++)
		clipset_ -> at(i) -> stop();
}
		
