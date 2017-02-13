#include "MidiClip.hpp"
#include <RtMidi.h>
#include "midi.hpp"
//#include <unistd.h> // sleep


//-------------
// Constructor 
//-------------

MidiClip::MidiClip (std::string name = "No name") : Clip()
{
	data_type_ = MIDI;
	name_ = name;
	launchstyle_ = LAUNCH_BAR;
	stopstyle_ = STOP_BAR;
	loopstyle_ = FOREVER;
	length_ = 0;
	time_ = 0;
	index_ = 0;
}

MidiClip::MidiClip (std::string filename, int tn) : Clip()
{
	data_type_ = MIDI;
	filename_ = filename;
	int p = filename_.rfind(".");
	name_ = filename_.substr(0, p);
	launchstyle_ = LAUNCH_BAR;
	stopstyle_ = STOP_BAR;
	loopstyle_ = FOREVER;
	length_ = 0;
	time_ = 0;
	index_ = 0;
}

MidiClip::MidiClip (std::string filename, int tn, std::string name, int launch, int stop, int loop) :
	Clip(name, launch, stop, loop)
{
	data_type_ = MIDI;
	filename_ = filename;
	tracknum_ = tn;
	
	length_ = 0;
	time_ = 0;
	index_ = 0;
}

//------------
// Destructor 
//------------

MidiClip::~MidiClip ()
{
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
	while ( events_[index_].getTime () == time_ )
	{
		//std::cout << time_ << " : " << index_ << " : " << events_ -> at (index_) -> hexData () << std::endl;
		midiout -> sendMessage (events_[index_].getData ());
		if ( index_ < events_.size () ) index_++;
		if ( index_ == events_.size () ) rewind ();
	}
	time_++;
}

void MidiClip::appendEvent (unsigned long time, std::vector<unsigned char> * event)
{
	auto mainlog= spdlog::get("main");	
	std::vector<unsigned char> ev;
	ev = *event;
	mainlog->debug("appendEvent {}", ev[0]);
	ScheduledMidiMessage smm (time, ev);
	events_.push_back (smm);
	//events_.push_back (ScheduledMidiMessage (time, ev));
	mainlog->debug("/appendEvent");
}
