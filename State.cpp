#include "State.hpp"

// Initialisation du singleton à NULL
State * State::singleton_ = NULL;
Track *	State::track_ = NULL;
Clip * State::clip_ = NULL;

// Constructor
State::State () : onoff_(true)
{
}

// Destructor
State::~State ()
{
}
		
void State::midiLog (MidiRaw * message)
{
	MidiRaw * m = new MidiRaw(*message);
	midilog_.push_back (m);
}
