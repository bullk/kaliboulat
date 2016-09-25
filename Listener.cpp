#include "Listener.hpp"

// Initialisation du singleton à NULL
Listener *Listener::singleton_ = NULL;
stk::FileWvIn * Listener::audiofile_ = new stk::FileWvIn ();
bool Listener::onoff_ = false;

// Constructor
Listener::Listener ()
{
	volume_ = 1.0f;
}

// Destructor
Listener::~Listener ()
{
	delete audiofile_;
}

stk::StkFloat Listener::tick(unsigned int channel)
{
	stk::StkFloat res = 0.0f;
	if ( onoff_ && audiofile_->isOpen() ) res = audiofile_ -> tick(channel) * volume_;
	return res;
}

