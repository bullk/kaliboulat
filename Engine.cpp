#include <fstream>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/archives/xml.hpp>

#include "Engine.hpp"
#include "State.hpp"
#include "Clock.hpp"
#include "midi.hpp"

std::string name_from_path (std::string path)
{
	int p = path.rfind("/") + 1;
	return path.substr(p, path.length()-p);
}

RessourceFile::RessourceFile (DataType dt) : data_type_(dt)
{
}

RessourceFile::RessourceFile (DataType dt, std::string path) : data_type_(dt), path_(path)
{
	name_ = name_from_path (path_);
}

RessourceFile::~RessourceFile ()
{
}

void RessourceFile::setPath (std::string path)
{
	path_ = path;
	name_ = name_from_path (path_);
}

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
	State::getProject() -> getClock() -> start();
}
		
void Waiter::pause ()
{
	State::getProject() -> getClock() -> pause();
}
		
void Waiter::stop ()
{
	State::getProject() -> getClock() -> stop();
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
	State::getProject() -> tick();
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
	State::setProject( project );
}

void Waiter::loadProject( std::string name )
{
	//closeProject ();
	newProject( name );
    std::ifstream is( State::getProject()->getFile() );
    cereal::XMLInputArchive archive( is );
    State::getProject()->serialize( archive ); 
    //archive(State::getProject()); 
}

void Waiter::saveProject()
{
    std::ofstream os( State::getProject()->getFile() );
    cereal::XMLOutputArchive archive( os );
    State::getProject()->serialize( archive );
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
	std::string name_out = State::getProject() -> getAudioDir() + "/" + filename;
	copyFile( name_in.c_str(), name_out.c_str() );
	State::getProject() -> addAudioFile( name_out );
}

void Waiter::importMidiFile( std::string name_in )
{
	std::string filename = name_from_path( name_in );
	std::string name_out = State::getProject() -> getMidiDir() + "/" + filename;
	copyFile( name_in.c_str(), name_out.c_str() );
	State::getProject() -> addMidiFile( name_out );
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
