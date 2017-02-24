#include "State.hpp"

#include <stdio.h>
#include <stdlib.h>

#include <ftw.h>
#include <fnmatch.h>
#include <libgen.h>
#include <fstream>

// Initialisation du singleton à NULL
State * State::singleton_ = NULL;
std::shared_ptr<Project> State::project_ = NULL;
std::shared_ptr<Track> State::track_ = NULL;
std::shared_ptr<Clip> State::clip_ = NULL;
std::vector<std::string> * State::projectlist_ = NULL;
std::vector<std::string> * State::audiofiles_ = NULL;
std::vector<std::string> * State::midifiles_ = NULL;

// Constructor
State::State () : onoff_(true)
{
	//audiodirs_ = new std::vector<std::string>;
	projectlist_ = new std::vector<std::string>;
	audiofiles_ = new std::vector<std::string>;
	midifiles_ = new std::vector<std::string>;
	loadConfiguration();
}

// Destructor
State::~State ()
{
	track_ = NULL;
	clip_ = NULL;
	//delete project_;
}

std::string State::getConfigurationFileName()
{
	char fname[128] = "";
	sprintf( fname, "%s/.Kaliboulat.xml", getenv("HOME") );
	return std::string( fname );
}

void State::loadConfiguration()
{
    std::ifstream is( getConfigurationFileName() );
    if ( is.is_open() )
    {
		cereal::XMLInputArchive archive( is );
		serialize( archive );
	}
	else
	{
		audiodirs_.push_back( std::string( getenv( "HOME" ) ) );
		mididirs_.push_back( std::string( getenv( "HOME" ) ) );
	}
	saveConfiguration();
}

void State::saveConfiguration()
{
	auto mainlog= spdlog::get( "main" );
	mainlog->info( "State::saveConfiguration" );
	mainlog->info( "* audiodirs_ contains {} element(s)", audiodirs_.size() );
	mainlog->info( "* {}", audiodirs_.at(0) );
	mainlog->info( "* mididirs_ contains {} element(s)", mididirs_.size() );
	mainlog->info( "* {}", mididirs_.at(0) );
	mainlog->info( "* get stream" );
    std::ofstream os( getConfigurationFileName() );
	mainlog->info( "* create archive" );
    cereal::XMLOutputArchive archive( os );
	mainlog->info( "* serialize" );
    serialize( archive ); 
	mainlog->info( "/State::saveConfiguration" );
}

RtMidiOut * State::getMidiOut()
{
	return midiout_;
}

void State::setMidiOut( RtMidiOut * midiout )
{
	midiout_ = midiout;
}


int State::scanProjectsCallback( const char *fpath, const struct stat *sb, int typeflag )
{
	char * localpath = strdup( fpath );
	if ( typeflag == FTW_F ) {
		if ( fnmatch( "*.kal", localpath, FNM_CASEFOLD ) == 0 ) {
			std::string str = std::string( basename( localpath ) );
			projectlist_ -> push_back( str.substr( 0, str.length()-4 ) );
		}
	}
	return 0;
}

int State::scanProjects ()
{
	projectlist_->clear();
	ftw( user_dir().c_str(), scanProjectsCallback, 16 );
	return 0;
}

int State::scanAudioFilesCallback( const char *fpath, const struct stat *sb, int typeflag )
{
	auto mainlog = spdlog::get( "main" );
	char * localpath = strdup( fpath );
	if ( typeflag == FTW_F ) {
		if ( fnmatch( "*.wav", localpath, FNM_CASEFOLD ) == 0 ) {
			audiofiles_ -> push_back( localpath );
			mainlog->info( "found {}", localpath );
		}
	}
	return 0;
}

int State::scanAudioFiles ()
{
	auto mainlog = spdlog::get( "main" );
	audiofiles_->clear();
	for ( unsigned int i=0; i< audiodirs_.size(); i++ )
	{
		mainlog->info( "scanning {}", audiodirs_[i].c_str() );
		ftw( audiodirs_[i].c_str(), scanAudioFilesCallback, 16 );
	}
	return 0;
}

int State::scanMidiFilesCallback (const char *fpath, const struct stat *sb, int typeflag)
{
	auto mainlog = spdlog::get("main");
	char * localpath = strdup (fpath);
	if (typeflag == FTW_F) {
		if (fnmatch("*.mid", localpath, FNM_CASEFOLD) == 0) {
			midifiles_ -> push_back(localpath);
			mainlog->info("found {}", localpath);
		}
	}
	return 0;
}

int State::scanMidiFiles ()
{
	auto mainlog = spdlog::get("main");
	midifiles_->clear();
	for ( unsigned int i=0; i< mididirs_.size(); i++ )
	{
		mainlog->info("scanning {}", mididirs_[i].c_str());
		ftw(mididirs_[i].c_str(), scanMidiFilesCallback, 16);
	}
	return 0;
}

