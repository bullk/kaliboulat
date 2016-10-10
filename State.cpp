#include "State.hpp"

// Initialisation du singleton à NULL
State * State::singleton_ = NULL;
std::shared_ptr<Project> State::project_ = NULL;
std::shared_ptr<Track> State::track_ = NULL;
Clip * State::clip_ = NULL;

// Constructor
State::State () : onoff_(true)
{
}

// Destructor
State::~State ()
{
}

