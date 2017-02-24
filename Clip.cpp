#include "Clip.hpp"
#include "midi.hpp"

//-------------
// Constructor 
//-------------

Clip::Clip () : state_(HALTED), name_(""), filename_(""), armMIDIchannel_(1), armMIDIkey_(-1), selected_(false)
{
}

Clip::Clip (std::string name) : state_(HALTED), name_(name), filename_(""), armMIDIchannel_(1), armMIDIkey_(-1), selected_(false)
{
}

Clip::Clip (std::string name, int launch, int stop, int loop, char armkey) :
	state_(HALTED), name_(name), filename_(""),
	launchstyle_(launch), stopstyle_(stop), loopstyle_(loop), armMIDIchannel_(1), armMIDIkey_(armkey), selected_(false)
{
}

//------------
// Destructor 
//------------

Clip::~Clip ()
{
}

void Clip::armLaunch()
{
	switch ( launchstyle_ )
	{
		case LAUNCH_BAR:
			state_ = ARMED;
			Waiter::getInstance() -> addBar( new Command<Clip*, void(Clip::*)()>(this, &Clip::play) );
			break;
		case LAUNCH_BEAT:
			state_ = ARMED;
			Waiter::getInstance() -> addBeat( new Command<Clip*, void(Clip::*)()>(this, &Clip::play) );
			break;
		case LAUNCH_FREE:
			play ();
			break;
		case LAUNCH_RESYNC:
			break;
	}
	
}

void Clip::armStop()
{
	switch ( stopstyle_ )
	{
		case STOP_BAR:
			Waiter::getInstance() -> addBar( new Command<Clip*, void(Clip::*)()>(this, &Clip::stop) );
			break;
		case STOP_BEAT:
			Waiter::getInstance() -> addBeat( new Command<Clip*, void(Clip::*)()>(this, &Clip::stop) );
			break;
		case STOP_FREE:
			stop ();
			break;
	}
}

void Clip::arm()
{
	if ( isPlaying() ) armStop();
	else armLaunch();
}

void Clip::MIDIKey( char k )
{
	//interrupteur sans maintien
	if ( k >= 0 )
	{
		MidiWaiter::getInstance() -> addCommand( note_on_trigger(armMIDIchannel_, armMIDIkey_), new Command<Clip*, void(Clip::*)()>(this, &Clip::armLaunch) );
		MidiWaiter::getInstance() -> addCommand( note_off_trigger(armMIDIchannel_, armMIDIkey_), new Command<Clip*, void(Clip::*)()>(this, &Clip::armStop) );
	}
}
