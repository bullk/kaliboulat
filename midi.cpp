#include "midi.h"

// MidiMessage Class

//-------------
// Constructor 
//-------------

MidiMessage::MidiMessage (vector<unsigned char> * data)
{
	data_ = new(vector<unsigned char>);
	data_ = data;
}


//------------
// Destructor 
//------------

MidiMessage::~MidiMessage ()
{
	delete data_;
}

//------------




// ScheduledMidiMessage Class

//-------------
// Constructor 
//-------------

ScheduledMidiMessage::ScheduledMidiMessage (vector<unsigned char> * data) : MidiMessage (data)
{
	delta_ticks_ = 0;
	abs_ticks_ = 0;
	bar_ = 0;
	beat_ = 0;
	tick_ = 0;
}


//------------
// Destructor 
//------------

ScheduledMidiMessage::~ScheduledMidiMessage ()
{
}

//------------

unsigned long ScheduledMidiMessage::getAbsTicks() { return abs_ticks_; }

void ScheduledMidiMessage::setAbsTicks(unsigned long ticks)
{
	abs_ticks_ = ticks;
}
