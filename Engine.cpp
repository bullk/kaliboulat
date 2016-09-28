#include "Engine.hpp"


RessourceFile::RessourceFile (DataType dt, std::string path) : data_type_(dt), path_(path)
{
	int p = path_.rfind("/") + 1;
	name_ = path_.substr(p, path_.length()-p);
}

RessourceFile::~RessourceFile ()
{
}

// Initialisation du singleton à NULL
Waiter *Waiter::singleton_ = NULL;

// Constructor
Waiter::Waiter ()
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
