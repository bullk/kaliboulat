#include "MidiTrack.hpp"


//-------------
// Constructor 
//-------------

MidiTrack::MidiTrack (std::string name) : Track (MIDI, "MIDI", name)
{
	//clipset_ = new std::vector<std::shared_ptr<MidiClip>>;
	hue_ =  0.75f + (float)((rand() % 31) -15) / 100 ;
}

MidiTrack::MidiTrack (std::string s, float h, std::vector<std::shared_ptr<MidiClip>> cs) :
	Track (MIDI, "Midi", s, h), clipset_(cs)
{
}

//------------
// Destructor 
//------------

MidiTrack::~MidiTrack ()
{
	//spdlog::get("main")->info("Deleting {} track {}", type_str_, name_);
	while (clipset_.size())
		clipset_.pop_back();
    //delete clipset_;
    //clipset_ = NULL;
}


//------------
// Add a clip
//------------

void MidiTrack::addClip (std::shared_ptr<Clip> clip)
{
	addClip ( std::static_pointer_cast<MidiClip>(clip) );
}

void MidiTrack::addClip(std::shared_ptr<MidiClip> clip)
{
	clipset_.push_back (clip);
}

void MidiTrack::deleteClip (unsigned int i)
{
	std::shared_ptr<MidiClip> clip = clipset_.at(i);
	clipset_.erase (clipset_.begin() + i);
	//delete clip;
}

void MidiTrack::tick (RtMidiOut * midiout)
{
	for (unsigned int i = 0; i < clipset_.size (); i++)
		if (clipset_.at(i)->getState () == Clip::PLAYING)
			clipset_.at(i)->tick (midiout);
}

void MidiTrack::stopAll ()
{
	for (unsigned int i = 0; i < clipset_.size(); i++)
		clipset_.at(i) -> stop();
}
		
