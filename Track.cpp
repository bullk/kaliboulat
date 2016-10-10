#include "Track.hpp"

//-------------
// Constructor 
//-------------

Track::Track (DataType dt, std::string ts, std::string s) :
	data_type_(dt), type_str_(ts), name_(s), state_(true)
{
}


//------------
// Destructor 
//------------

Track::~Track ()
{
}


