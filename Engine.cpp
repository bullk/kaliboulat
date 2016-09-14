#include "Engine.hpp"

// Initialisation du singleton à NULL
Waiter *Waiter::_singleton = NULL;

// Constructor
Waiter::Waiter ()	: _value (0)
{
}

// Destructor
Waiter::~Waiter ()
{
}
		
void Waiter::bar ()
{
	while ( !bar_.empty() )
	{
		bar_.front() -> execute ();
		bar_.pop();
	}
}

void Waiter::beat ()
{
	while ( !beat_.empty() )
	{
		beat_.front() -> execute ();
		beat_.pop();
	}
}

void Waiter::tick ()
{
	while ( !tick_.empty() )
	{
		tick_.front() -> execute ();
		tick_.pop();
	}
}

void Waiter::main ()
{
	while ( !main_.empty() )
	{
		main_.front() -> execute ();
		main_.pop();
	}
}

