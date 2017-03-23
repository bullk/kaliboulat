#include "spdlog/spdlog.h"
#include "midi.hpp"
#include "State.hpp"
#include "Clip.hpp"

//----------------------------------------------------------------------
// MIDI FUNCTIONS
//----------------------------------------------------------------------

void midi_init()
{
	auto mainlog= spdlog::get("main");	
	mainlog->info("----- MIDI init -----");
	RtMidiOut * midiout = NULL;
	RtMidiIn * midiin = NULL;
	try { 
	#ifdef __UNIX_JACK__
		mainlog->info("creating RtMidiOut with JACK");
		midiout = new RtMidiOut (RtMidi::UNIX_JACK, MIDIOUT_MODULE_NAME);
		midiin = new RtMidiIn (RtMidi::UNIX_JACK, MIDIIN_MODULE_NAME);
	#else
		mainlog->info("creating RtMidiOut without JACK !!!");
		midiout = new RtMidiOut (MIDI_MODULE_NAME);
		midiin = new RtMidiIn (MIDI_MODULE_NAME);
	#endif
	}
	catch ( RtMidiError &error ) 
	{
		error.printMessage ();
		exit ( EXIT_FAILURE );
	}

	State::getInstance()->setMidiIn( midiin );
	State::getInstance()->setMidiOut( midiout );
		
	midiin->setCallback (midiCallback);
	midiin->ignoreTypes( false, true, true );
	
	mainlog->info( "opening virtual MIDI out port" );
	try { 
		midiout->openVirtualPort ( "thru" );
	}
	catch ( RtMidiError &error ) 
	{
		error.printMessage();
		exit( EXIT_FAILURE );
	}

	mainlog->info( "opening virtual MIDI in port" );
	try { 
		midiin->openVirtualPort ( "control" );
	}
	catch ( RtMidiError &error ) 
	{
		error.printMessage();
		exit( EXIT_FAILURE );
	}

}
	
void midiPanic (RtMidiOut * midiout)
{
	RawMidi cc121 { 0xB0, 0x79, 0x00 }; // Channel 1, all CC OFF
	RawMidi cc123 { 0xB0, 0x7B, 0x00 }; // Channel 1, all notes OFF
	RawMidi message;
	
	for (unsigned short i=0; i<16; i++)
	{
		message = cc123;
		message[0] += i;
		midiout->sendMessage(&message);
		message = cc121;
		message[0] += i;
		midiout->sendMessage(&message);
	}
}

RawMidi key_to_trigger( RawMidi m )
{
	return RawMidi( m.begin(), m.begin()+2 );
}

void midiCallback( double timeStamp, RawMidi * message, void *userData )
{
	auto mainlog = spdlog::get( "main" );	
	unsigned int high_digit = message->at(0) >> 4;
	unsigned int low_digit = message->at(0) & 0xF;
	if ( MidiWaiter::getInstance()->getPreemption() )
	{
		if (( high_digit >> 1 ) == 4)
		{
			mainlog->info( "MidiWaiter : assign {}", char_vector_to_hex( *message ) );
			MidiWaiter::getInstance()->setAssignment( *message );
		}
	}
	else
	{
		mainlog->info( "MidiWaiter : execute {}", char_vector_to_hex( *message ) );
		MidiWaiter::getInstance()->execute( *message );
		switch ( high_digit )
		{
			case 0x8: // note off
				//std::cout << "channel " << low_digit +1
					//<< " NOTE OFF " << (unsigned int)message->at(1)
					//<< std::endl;
				mainlog->info( "MidiWaiter : execute {}", char_vector_to_hex( key_to_trigger(*message) ) );
				MidiWaiter::getInstance()->execute( key_to_trigger(*message) );	
				break;
			case 0x9: // note on
				//std::cout << "channel " << low_digit +1
					//<< " NOTE ON " << (unsigned int)message->at(1)
					//<< " velocity " << (unsigned int)message->at(2)
					//<< std::endl;
				mainlog->info( "MidiWaiter : execute {}", char_vector_to_hex( key_to_trigger(*message) ) );
				MidiWaiter::getInstance()->execute( key_to_trigger(*message) );	
				break;
			case 0xA: // polyphonic after touch
				//std::cout << "channel " << low_digit +1
					//<< " polyphonic after touch note " << (unsigned int)message->at(1)
					//<< " pressure " << (unsigned int)message->at(2)
					//<< std::endl;
				break;
			case 0xB: // control change
				//std::cout << "channel " << low_digit +1
					//<< " controller " << (unsigned int)message->at(1)
					//<< " value " << (unsigned int)message->at(2)
					//<< std::endl;
				break;
			case 0xC: // program change
				//std::cout << "channel " << low_digit +1
					//<< " program change " << (unsigned int)message->at(1)
					//<< std::endl;
				break;
			case 0xD: // channel after touch
				//std::cout << "channel " << low_digit +1
					//<< " after touch " << (unsigned int)message->at(1)
					//<< std::endl;
				break;
			case 0xE: // pitch bend
				//std::cout << "channel " << low_digit +1
					//<< " pitch bend " << (unsigned int)message->at(2)
					//<< std::endl;
				break;
			case 0xF: // system message
				switch ( low_digit )
				{
					case 0x0:
						//std::cout << "SYSEX" << std::endl;
						break;
					case 0x1:
						//std::cout << "MTC" << std::endl;
						break;
					case 0x2:
						//std::cout << "Song Position" << std::endl;
						break;
					case 0x3:
						//std::cout << "Song Select" << std::endl;
						break;
					case 0x6:
						//std::cout << "Tune request" << std::endl;
						break;
					case 0x8:
						//std::cout << "Timing clock" << std::endl;
						break;
					case 0x9:
						//std::cout << "Mesure end" << std::endl;
						break;
					case 0xA:
						//std::cout << "Start" << std::endl;
						break;
					case 0xB:
						//std::cout << "Continue" << std::endl;
						break;
					case 0xC:
						//std::cout << "Stop" << std::endl;
						break;
					case 0xE:
						//std::cout << "Active sensing" << std::endl;
						break;
					case 0xF:
						//std::cout << "System reset" << std::endl;
						break;
					default:
						//std::cout << "WARNING : unknown MIDI data" << std::endl;
						break;
				}
				break;
			default:
				//std::cout << "WARNING : unknown MIDI data" << std::endl;
				break;
		}
		//State::getInstance()->midiLog (message);
	}
}

// MidiMessage Class

//-------------
// Constructor 
//-------------

MidiMessage::MidiMessage (RawMidi data) :
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

std::string char_vector_to_hex (RawMidi v)
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
	return char_vector_to_hex( data_ );
}


// ScheduledMidiMessage Class

//-------------
// Constructor 
//-------------

ScheduledMidiMessage::ScheduledMidiMessage (long unsigned int time, RawMidi data) :
	Scheduled (time), MidiMessage (data)
{
	auto mainlog= spdlog::get( "main" );	
	mainlog->debug( "ScheduledMidiMessage::ScheduledMidiMessage {} {}", time, hexData() );
	bar_ = 0;
	beat_ = 0;
	tick_ = 0;
	mainlog->debug( "/ScheduledMidiMessage::ScheduledMidiMessage" );
}


//------------
// Destructor 
//------------

ScheduledMidiMessage::~ScheduledMidiMessage ()
{
	auto mainlog= spdlog::get( "main" );	
	mainlog->debug( "ScheduledMidiMessage::~ScheduledMidiMessage {}", hexData() );
}

//------------

RawMidi note_on_trigger( int channel, int key )
{
	RawMidi trigger;
	trigger.push_back( (9<<4) + channel-1 );
	trigger.push_back( key );
	return trigger;
}

RawMidi note_off_trigger( int channel, int key )
{
	RawMidi trigger;
	trigger.push_back( (8<<4) + channel-1 );
	trigger.push_back( key );
	return trigger;
}

//------------

// Initialisation du singleton à NULL
MidiWaiter *MidiWaiter::singleton_ = NULL;

// Constructor
MidiWaiter::MidiWaiter() : preemption_(false)
{
	spdlog::get("main")->info("creating MidiWaiter");
	assignment_ = RawMidi();
}

// Destructor
MidiWaiter::~MidiWaiter ()
{
	spdlog::get("main")->info("killing MidiWaiter");
}

void MidiWaiter::addCommand( RawMidi m, BaseCommand * c )
{
	std::map<RawMidi, std::vector<BaseCommand *>>::iterator it;
	it = filters_.find(m);
	if ( it == filters_.end() )
		filters_.emplace( m, std::vector<BaseCommand *>() );
	filters_.at(m).push_back(c);
}

void MidiWaiter::deleteCommand( RawMidi m, Clip * clip )
{
	auto mainlog = spdlog::get( "main" );	
	mainlog->debug( "MidiWaiter::deleteCommand" );
	std::map<RawMidi, std::vector<BaseCommand *>>::iterator map_it;
	map_it = filters_.find(m);
	if ( map_it != filters_.end() )
	{
		mainlog->debug( "* found trigger {}", char_vector_to_hex(m) );
		std::vector<BaseCommand *> comv = filters_.at(m);
		mainlog->debug( "* command vector size {}", comv.size() );
		std::vector<BaseCommand *>::iterator f_it;
		for ( f_it = comv.begin(); f_it != comv.end(); f_it++ )
			if ( (*f_it)->isFrom( clip ) )
			{
				mainlog->debug( "* command object is clip {}", clip->getName() );
				comv.erase( f_it );
				break;
			}
	}
	mainlog->debug( "/MidiWaiter::deleteCommand" );
}

bool MidiWaiter::inCommands ( RawMidi m )
{
	bool res = false;
	auto mainlog = spdlog::get( "main" );	
	mainlog->debug( "MidiWaiter::inCommands" );
	std::map<RawMidi, std::vector<BaseCommand *>>::iterator map_it;
	map_it = filters_.find(m);
	if ( map_it != filters_.end() )	res = true;
	mainlog->debug( "/MidiWaiter::inCommands" );
	return res;
}

void MidiWaiter::execute ( RawMidi m )
{
	auto mainlog = spdlog::get( "main" );	
	mainlog->debug( "MidiWaiter::execute" );
	std::map<RawMidi, std::vector<BaseCommand *>>::iterator map_it;
	map_it = filters_.find(m);
	if ( map_it != filters_.end() )
	{
		std::vector<BaseCommand *> comv = filters_.at(m);
		std::vector<BaseCommand *>::iterator f_it;
		for ( f_it = comv.begin(); f_it != comv.end(); f_it++ )
			( *f_it )->execute();
	}
	mainlog->debug( "/MidiWaiter::execute" );
}


void MidiWaiter::setAssignment( RawMidi m )
{
	assignment_ = m;
	release();
}

RawMidi MidiWaiter::getAssignment()
{
	RawMidi m = assignment_;
	assignment_ = RawMidi();
	return m;
}

