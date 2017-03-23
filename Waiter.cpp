#include <fstream>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/archives/xml.hpp>
#include "spdlog/spdlog.h"

#include "Waiter.hpp"
#include "Project.hpp"
#include "State.hpp"
#include "Clock.hpp"
#include "midi.hpp"

// Initialisation du singleton à NULL
Waiter *Waiter::singleton_ = NULL;

// Constructor
Waiter::Waiter ()
{
	spdlog::get("main")->info("creating Waiter");
}

// Destructor
Waiter::~Waiter ()
{
	spdlog::get("main")->info("killing Waiter");
	//while ( !main_.empty() ) main_.pop();
	//while ( !tick_.empty() ) tick_.pop();
	//while ( !beat_.empty() ) beat_.pop();
	//while ( !bar_.empty() ) bar_.pop();
	//while ( !midilog_.empty() ) midilog_.pop_back();
}

//-------------------------------------------------------

// TODO : deplacer la fonction midiPanic dans MidiModule
void Waiter::panic()
{
	midiPanic( State::getInstance()->getMidiOut() );
}

void Waiter::start ()
{
	State::getInstance()->getProject() -> getClock() -> start();
}
		
void Waiter::pause ()
{
	State::getInstance()->getProject() -> getClock() -> pause();
}
		
void Waiter::stop ()
{
	State::getInstance()->getProject() -> getClock() -> stop();
	panic();
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
	State::getInstance()->getProject() -> tick();
	while ( !tick_.empty() )
	{
		tick_.front() -> execute();
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

void Waiter::newProject( std::string name )
{
	//sauvegarder le projet courant
	//closeProject ();
	std::shared_ptr<Project> project (new Project (name) );
	State::getInstance()->setProject( project );
}

void Waiter::loadProject( std::string name )
{
	//closeProject ();
	newProject( name );
    std::ifstream is( State::getInstance()->getProject()->getFile() );
    cereal::XMLInputArchive archive( is );
    State::getInstance()->getProject()->serialize( archive ); 
    //archive(State::getProject()); 
}

void Waiter::saveProject()
{
    std::ofstream os( State::getInstance()->getProject()->getFile() );
    cereal::XMLOutputArchive archive( os );
    State::getInstance()->getProject()->serialize( archive );
    //archive(State::getProject()); 
}

//void Waiter::closeProject ()
//{
	//saveProject();
    //State::setProject( NULL );
//}

void copyFile( const char * name_in, const char * name_out )
{
    FILE * infile  = fopen( name_in, "rb" );
    FILE * outfile = fopen( name_out, "wb" );
     
    char  buffer[1024];
    size_t count_in;

    /* copy from input to output */
    while ( ( count_in = fread( buffer, 1, sizeof( buffer ), infile ) ) )
        fwrite( buffer, 1, count_in, outfile );

    fclose( infile );    
    fclose( outfile );
}

void Waiter::importAudioFile( std::string name_in )
{
	std::string filename = name_from_path( name_in );
	std::string name_out = State::getInstance()->getProject() -> getAudioDir() + "/" + filename;
	copyFile( name_in.c_str(), name_out.c_str() );
	State::getInstance()->getProject() -> addAudioFile( name_out );
}

void Waiter::importMidiFile( std::string name_in )
{
	std::string filename = name_from_path( name_in );
	std::string name_out = State::getInstance()->getProject() -> getMidiDir() + "/" + filename;
	copyFile( name_in.c_str(), name_out.c_str() );
	State::getInstance()->getProject() -> addMidiFile( name_out );
}

void Waiter::selectClip (std::shared_ptr<Clip> clip)
{
	auto mainlog= spdlog::get("main");	
	mainlog->debug("Waiter::selectClip");
	mainlog->debug("* unselect");
	if ( State::getInstance() -> getClip() )
		State::getInstance() -> getClip() -> unselect();
	mainlog->debug("* select");
	clip -> select();
	mainlog->debug("* setClip");
	State::getInstance() -> setClip( clip );
	mainlog->debug("/Waiter::selectClip");
}

void Waiter::deleteClip( std::shared_ptr<Clip> clip )
{
	auto mainlog= spdlog::get("main");	
	mainlog->debug("Waiter::deleteClip");
	mainlog->debug("* unsetClip");
	State::getInstance() -> unsetClip();
	//if ( State::getInstance() -> getClip() )
		//State::getInstance() -> getClip() -> unselect();
	//mainlog->debug("* select");
	//clip -> select();
	mainlog->debug("/Waiter::deleteClip");
}
