#include "Track.hpp"

//-------------
// Constructor 
//-------------

Track::Track (std::string s)
{
	state_ = true;
	setName (s);
}


//------------
// Destructor 
//------------

Track::~Track ()
{
}


