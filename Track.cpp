#include "Track.hpp"

//-------------
// Constructor 
//-------------

Track::Track (std::string s)
{
	setName (s);
	state_ = true;
}


//------------
// Destructor 
//------------

Track::~Track ()
{
}


