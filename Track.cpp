#include "Track.hpp"

//-------------
// Constructor 
//-------------

Track::Track (DataType dt, std::string ts, std::string s) :
	data_type_(dt), type_str_(ts), name_(s), state_(true)
{
	auto mainlog= spdlog::get("main");
	mainlog -> info("creating track");
	mainlog -> info("	data_type_ : {}", dt);
	mainlog -> info("	type_str_ : {}", ts);
	mainlog -> info("	name_ : {}", s);
}

Track::Track (DataType dt, std::string ts, std::string s, float h) :
	data_type_(dt), type_str_(ts), name_(s), state_(true), hue_(h)
{
	auto mainlog= spdlog::get("main");
	mainlog -> info("creating track");
	mainlog -> info("	data_type_ : {}", dt);
	mainlog -> info("	type_str_ : {}", ts);
	mainlog -> info("	name_ : {}", s);
	mainlog -> info("	hue_ : {}", h);
}


//------------
// Destructor 
//------------

Track::~Track ()
{
}


