#include "MidiClip.h"


//-------------
// Constructor 
//-------------

MidiClip::MidiClip ()
{
	name_ = "No name";
	length_ = 0;
	time_ = 0;
	data_ = new std::vector<ScheduledMidiMessage *>;
}

MidiClip::MidiClip (std::string name)
{
	//path_ = path;
	//int p = path_.rfind("/") + 1;
	//name_ = path_.substr(p, path_.length()-p);
	name_ = name;
	length_ = 0;
	time_ = 0;
	data_ = new std::vector<ScheduledMidiMessage *>;
}


//------------
// Destructor 
//------------

MidiClip::~MidiClip ()
{
    if (data_ != NULL){
        for (unsigned int i = 0; i < data_->size(); i++)
            delete data_->at(i);
    }
}

//------------

long unsigned int MidiClip::getLength () { return length_; }

void MidiClip::setLength (long unsigned int length) { length_ = length; }

long unsigned int MidiClip::getTime () { return time_; }

void MidiClip::rewind () { time_ = 0; }

void MidiClip::tick (RtMidiOut * midiout)
{
	if (time_ == length_) rewind ();
	else time_++;
}

void MidiClip::appendEvent (long unsigned int time, std::vector<unsigned char> * event)
{
	std::vector<unsigned char> ev;
	ev = *event;
	data_->push_back (new ScheduledMidiMessage (time, &ev));
}
