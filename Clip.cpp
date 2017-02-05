#include "Clip.hpp"


//-------------
// Constructor 
//-------------

Clip::Clip () : state_(HALTED), name_(""), path_("")
{
}

Clip::Clip (std::string name) : state_(HALTED), name_(name), path_("")
{
}

Clip::Clip (std::string name, int launch, int stop, int loop) :
	state_(HALTED), name_(name), path_(""), launchstyle_(launch), stopstyle_(stop), loopstyle_(loop)
{
}

//------------
// Destructor 
//------------

Clip::~Clip ()
{
}


void Clip::arm ()
{
	switch ( launchstyle_ )
	{
		case LAUNCH_BAR:
			state_ = ARMED;
			Waiter::getInstance() -> addBar (new Command<Clip*, void(Clip::*)()>(this, &Clip::play));
			break;
		case LAUNCH_BEAT:
			state_ = ARMED;
			Waiter::getInstance() -> addBeat (new Command<Clip*, void(Clip::*)()>(this, &Clip::play));
			break;
		case LAUNCH_FREE:
			play ();
			break;
		case LAUNCH_RESYNC:
			break;
	}
}
