#include <fstream>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/archives/xml.hpp>

#include "Engine.hpp"
#include "State.hpp"

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
	//while ( !main_.empty() ) main_.pop();
	//while ( !tick_.empty() ) tick_.pop();
	//while ( !beat_.empty() ) beat_.pop();
	//while ( !bar_.empty() ) bar_.pop();
	//while ( !midilog_.empty() ) midilog_.pop_back();
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

void Waiter::newProject (std::string name)
{
	//sauvegarder le projet courant
	saveProject ();
	std::shared_ptr<Project> project (new Project (name) );
	State::setProject (project);
}

void Waiter::loadProject (std::string name)
{
	newProject (name);
    std::ifstream is(State::getProject()->getFile());
    cereal::XMLInputArchive archive(is);
    State::getProject()->serialize(archive); 
    //archive(State::getProject()); 
}

void Waiter::saveProject ()
{
    std::ofstream os(State::getProject()->getFile());
    cereal::XMLOutputArchive archive(os);
    State::getProject()->serialize(archive); 
    //archive(State::getProject()); 
}

