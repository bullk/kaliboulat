#include "midi.hpp"
#include "spdlog/spdlog.h"

//----------------------------------------------------------------------
// MIDI FUNCTIONS
//----------------------------------------------------------------------

void midiInit ()
{
}

void midiPanic (RtMidiOut * midiout)
{
	std::vector<unsigned char> cc121 { 0xB0, 0x79, 0x00 }; // Channel 1, all CC OFF
	std::vector<unsigned char> cc123 { 0xB0, 0x7B, 0x00 }; // Channel 1, all notes OFF
	std::vector<unsigned char> message;
	
	for (unsigned short i=0; i<16; i++)
	{
		message = cc123;
		message[0] += i;
		midiout -> sendMessage(&message);
		message = cc121;
		message[0] += i;
		midiout -> sendMessage(&message);
	}
}

void midiCallback (double timeStamp, std::vector< unsigned char > *message, void *userData)
{
	unsigned int high_digit = message->at(0) >> 4;
	unsigned int low_digit = message->at(0) & 0xF;
	switch ( high_digit )
	{
		case 0x8: // note off
			std::cout << "channel " << low_digit +1
				<< " NOTE OFF " << (unsigned int)message->at(1)
				<< std::endl;
			break;
		case 0x9: // note on
			std::cout << "channel " << low_digit +1
				<< " NOTE ON " << (unsigned int)message->at(1)
				<< " velocity " << (unsigned int)message->at(2)
				<< std::endl;
			break;
		case 0xA: // polyphonic after touch
			std::cout << "channel " << low_digit +1
				<< " polyphonic after touch note " << (unsigned int)message->at(1)
				<< " pressure " << (unsigned int)message->at(2)
				<< std::endl;
			break;
		case 0xB: // control change
			std::cout << "channel " << low_digit +1
				<< " controller " << (unsigned int)message->at(1)
				<< " value " << (unsigned int)message->at(2)
				<< std::endl;
			break;
		case 0xC: // program change
			std::cout << "channel " << low_digit +1
				<< " program change " << (unsigned int)message->at(1)
				<< std::endl;
			break;
		case 0xD: // channel after touch
			std::cout << "channel " << low_digit +1
				<< " after touch " << (unsigned int)message->at(1)
				<< std::endl;
			break;
		case 0xE: // pitch bend
			std::cout << "channel " << low_digit +1
				<< " pitch bend " << (unsigned int)message->at(2)
				<< std::endl;
			break;
		case 0xF: // system message
			switch ( low_digit )
			{
				case 0x0:
					std::cout << "SYSEX" << std::endl;
					break;
				case 0x1:
					std::cout << "MTC" << std::endl;
					break;
				case 0x2:
					std::cout << "Song Position" << std::endl;
					break;
				case 0x3:
					std::cout << "Song Select" << std::endl;
					break;
				case 0x6:
					std::cout << "Tune request" << std::endl;
					break;
				case 0x8:
					std::cout << "Timing clock" << std::endl;
					break;
				case 0x9:
					std::cout << "Mesure end" << std::endl;
					break;
				case 0xA:
					std::cout << "Start" << std::endl;
					break;
				case 0xB:
					std::cout << "Continue" << std::endl;
					break;
				case 0xC:
					std::cout << "Stop" << std::endl;
					break;
				case 0xE:
					std::cout << "Active sensing" << std::endl;
					break;
				case 0xF:
					std::cout << "System reset" << std::endl;
					break;
				default:
					std::cout << "WARNING : unknown MIDI data" << std::endl;
					break;
			}
			break;
		default:
			std::cout << "WARNING : unknown MIDI data" << std::endl;
			break;
	}
	//State::getInstance() -> midiLog (message);
}

// MidiMessage Class

//-------------
// Constructor 
//-------------

MidiMessage::MidiMessage (std::vector<unsigned char> data) :
	data_(data)
{
}


//------------
// Destructor 
//------------

MidiMessage::~MidiMessage ()
{
}

//------------

std::string char_vector_to_hex (std::vector<unsigned char> v)
{
	std::string s = "";
	char buf[4];
	for ( unsigned int i = 0; i < v.size (); i++ )
	{
		sprintf (buf, "%02X ", v.at (i));
		s += buf;
	}
	return s;
}


std::string MidiMessage::hexData ()
{
	return char_vector_to_hex (data_);
}


// ScheduledMidiMessage Class

//-------------
// Constructor 
//-------------

ScheduledMidiMessage::ScheduledMidiMessage (long unsigned int time, std::vector<unsigned char> data) :
	Scheduled (time), MidiMessage (data)
{
	auto mainlog= spdlog::get("main");	
	mainlog->debug("ScheduledMidiMessage::ScheduledMidiMessage {} {}", time, hexData());
	bar_ = 0;
	beat_ = 0;
	tick_ = 0;
	mainlog->debug("/ScheduledMidiMessage::ScheduledMidiMessage");
}


//------------
// Destructor 
//------------

ScheduledMidiMessage::~ScheduledMidiMessage ()
{
	auto mainlog= spdlog::get("main");	
	mainlog->debug("ScheduledMidiMessage::~ScheduledMidiMessage {}", hexData());
}

//------------

