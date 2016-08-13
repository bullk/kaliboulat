#include "midi.h"

// MidiMessage Class

//-------------
// Constructor 
//-------------

MidiMessage::MidiMessage (std::vector<unsigned char> * data)
{
	data_ = new std::vector<unsigned char>;
	*data_ = *data;
}


//------------
// Destructor 
//------------

MidiMessage::~MidiMessage ()
{
	delete data_;
}

//------------

std::string MidiMessage::hexData ()
{
	std::string s = "";
	char buffer[2];
	for ( unsigned int i = 0; i < data_ -> size (); i++ )
	{
		sprintf (buffer, "%x", data_ -> at (i));
		s += buffer;
	}
	return s;
}


// ScheduledMidiMessage Class

//-------------
// Constructor 
//-------------

ScheduledMidiMessage::ScheduledMidiMessage (long unsigned int time, std::vector<unsigned char> * data) : Scheduled (time), MidiMessage (data)
{
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

