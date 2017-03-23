#include <RtMidi.h>
#include "spdlog/spdlog.h"

#include "Clock.hpp"
#include "midi.hpp"
#include "MidiClip.hpp"
#include "MidiFile.hpp"
#include "Project.hpp"
#include "State.hpp"
//#include <unistd.h> // sleep

//class MidiFile;

//-------------
// Constructor 
//-------------

MidiClip::MidiClip (std::string name = "No name") : Clip(), divscale_(1), length_(0)
{
	data_type_ = MIDI;
	name_ = name;
	launchstyle_ = LAUNCH_BAR;
	stopstyle_ = STOP_BAR;
	loopstyle_ = FOREVER;
	rewind();
}

MidiClip::MidiClip( std::string filename, int tn ) : Clip(), divscale_(1)
{
	data_type_ = MIDI;
	filename_ = name_from_path( filename );
	tracknum_ = tn;
	launchstyle_ = LAUNCH_BAR;
	stopstyle_ = STOP_BAR;
	loopstyle_ = FOREVER;
	rewind();
	getEventsFromSource( true );
}

MidiClip::MidiClip( std::string filename, int tn, std::string name, int launch, int stop, int loop, int amc, int amk ) :
	Clip( name, launch, stop, loop, amc, amk ), divscale_(1)
{
	data_type_ = MIDI;
	filename_ = name_from_path( filename );
	tracknum_ = tn;
	rewind();
	getEventsFromSource( false );
}

//------------
// Destructor 
//------------

MidiClip::~MidiClip ()
{
}

//------------

int ppcm(int X, int Y)
{
  int A=X;
  int B=Y;
  while (A!=B)
  {
    while (A>B) B=B+Y;
    while (A<B) A=A+X;
  }
  return A;
}


void MidiClip::rewind ()
{
	clock_time_ = 0;
	time_ = 0;
	index_ = 0;
}


void MidiClip::getEventsFromSource( bool rename )
{
	auto mainlog= spdlog::get( "main" );	
	mainlog->debug( "MidiClip::getEventsFromSource" );
	std::string uri = State::getProject()->getMidiDir() + "/" + filename_;
	MidiFile source( uri );
	
	std::vector<unsigned char> * event = new std::vector<unsigned char> ();
	unsigned long time_delta_time, abs_time;

	if ( rename )
	{
		int p = filename_.rfind( "." );
		name_ = filename_.substr( 0, p );
		char buffer[50];
		sprintf( buffer, "%s-track-%02d", name_.c_str(), tracknum_ );
		name_ = buffer;
	}
	
	setDivision( source.getDivision() );
	if ( State::getProject()->getClock()->getTicksPerBeat() % division_ != 0 )
		State::getProject()->getClock()->setTicksPerBeat(
			ppcm( State::getProject()->getClock()->getTicksPerBeat(), division_ )
		);
	divscale_ = State::getProject()->getClock()->getTicksPerBeat() / division_;
	source.rewindTrack( tracknum_ );

	mainlog->debug( "parsing events" );
	abs_time=0;
	mainlog->debug( "getting first event" );
	time_delta_time = source.getNextEvent( event, tracknum_ );
	while ( event->size () > 0 )
	{
		abs_time += time_delta_time;
		switch ( event->at(0) )
		{
		case 0xFF:
			mainlog->debug( "time {} : SMF meta : {} {}", abs_time, char_vector_to_hex( *event ) );
			switch ( event->at(1) )
			{
			case 0x03:
			{
				if (rename)
				{
					name_ = "";
					unsigned int strend = event->at(2) + 3;
					for ( unsigned int i=3; i < strend; i++ )
						name_ += (char) event->at(i);
					mainlog->debug( "time {} : setting clip name to {}", abs_time, name_ );
				}
				break;
			}
			}
			break;
		}
		mainlog->debug( "time {} : event {}", abs_time, event->at(0) );
		appendEvent( abs_time, event );
		mainlog->debug( "." );
		mainlog->debug( "getting next event" );
		time_delta_time = source.getNextEvent( event, tracknum_ );
	}
	mainlog->debug( "/parsing events" );
	
	setLength ( abs_time );
	//if ( abs_time > length_ ) length_ = abs_time;
					
	delete event;
	mainlog->debug( "/MidiClip::getEventsFromSource" );
	
}

void MidiClip::tick( RtMidiOut * midiout )
{
	if ( clock_time_ % divscale_ == 0 )
	{
		time_ = clock_time_ / divscale_;
		while ( events_[index_].getTime() == time_ )
		{
			//std::cout << time_ << " : " << index_ << " : " << events_->at (index_)->hexData () << std::endl;
			midiout->sendMessage( events_[index_].getData() );
			if ( index_ < events_.size() ) index_++;
			if ( index_ == events_.size() ) {
				if ( loopstyle_ == ONESHOT ) stop();
				else rewind();
			}
		}
	}
	clock_time_++;
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
