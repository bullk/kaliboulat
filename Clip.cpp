#include "Clip.hpp"


//-------------
// Constructor 
//-------------

Clip::Clip () : state_(HALTED)
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
		case BAR:
			state_ = ARMED;
			std::cout << "clip armed" << std::endl;
			Waiter::getInstance () -> addBar (new Command<Clip*, void(Clip::*)()>(this, &Clip::play));
			break;
		case BEAT:
			state_ = ARMED;
			break;
		case FREE:
			play ();
			break;
		case RESYNC:
			break;
	}
}
