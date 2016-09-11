#include "Track.hpp"

//-------------
// Constructor 
//-------------

Track::Track (std::string s)
{
	setName (s);
	state_ = true;
	hue_ = 0.0f;
}


//------------
// Destructor 
//------------

Track::~Track ()
{
}


