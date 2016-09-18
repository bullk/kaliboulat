#include "State.hpp"

// Initialisation du singleton à NULL
State *State::singleton_ = NULL;

// Constructor
State::State ()
{
	onoff_ = true;
	track_ = NULL;
	clip_ = NULL;
}

// Destructor
State::~State ()
{
}
		
