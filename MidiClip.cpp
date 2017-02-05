#include "MidiClip.hpp"
#include <RtMidi.h>
#include "midi.hpp"


//-------------
// Constructor 
//-------------

MidiClip::MidiClip (std::string name = "No name") : Clip()
{
	data_type_ = MIDI;
	//path_ = path;
	//int p = path_.rfind("/") + 1;
	//name_ = path_.substr(p, path_.length()-p);
	name_ = name;
	length_ = 0;
	time_ = 0;
	index_ = 0;
	data_ = new std::vector<ScheduledMidiMessage *>;
	launchstyle_ = LAUNCH_BAR;
	stopstyle_ = STOP_BAR;
	loopstyle_ = FOREVER;
}

MidiClip::MidiClip (std::string name, int launch, int stop, int loop) : Clip(name, launch, stop, loop)
{
	data_type_ = MIDI;
	//path_ = path;
	//int p = path_.rfind("/") + 1;
	//name_ = path_.substr(p, path_.length()-p);
	name_ = name;
	length_ = 0;
	time_ = 0;
	index_ = 0;
	data_ = new std::vector<ScheduledMidiMessage *>;
}

//------------
// Destructor 
//------------

MidiClip::~MidiClip ()
{
    if (data_ != NULL){
        for (unsigned int i = 0; i < data_ -> size(); i++)
            delete data_ -> at(i);
    }
}

//------------


void MidiClip::rewind ()
{
	//std::cout << "rewind" << std::endl;
	time_ = 0;
	index_ = 0;
}

void MidiClip::tick (RtMidiOut * midiout)
{
	while ( data_ -> at (index_) -> getTime () == time_ )
	{
		//std::cout << time_ << " : " << index_ << " : " << data_ -> at (index_) -> hexData () << std::endl;
		midiout -> sendMessage (data_ -> at (index_) -> getData ());
		if ( index_ < data_ -> size () ) index_++;
		if ( index_ == data_ -> size () ) rewind ();
	}
	time_++;
}

void MidiClip::appendEvent (unsigned long time, std::vector<unsigned char> * event)
{
	std::vector<unsigned char> ev;
	ev = *event;
	data_ -> push_back (new ScheduledMidiMessage (time, &ev));
}
