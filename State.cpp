#include <stdio.h>
#include <stdlib.h>

#include <ftw.h>
#include <fnmatch.h>
#include <libgen.h>
#include <fstream>
#include "spdlog/spdlog.h"

#include "globals.hpp"
#include "State.hpp"


// Initialisation du singleton à NULL
State * State::singleton_ = NULL;
std::vector<std::string> State::projectlist_ = {};
std::vector<std::string> State::audiofiles_ = {};
std::vector<std::string> State::midifiles_ = {};
std::shared_ptr<Project> State::project_ = NULL;
std::shared_ptr<Track> State::track_ = NULL;
std::shared_ptr<Clip> State::clip_ = NULL;

// Constructor
State::State () : onoff_(true)
{
	spdlog::get("main")->info("creating State");
	loadConfiguration();
	scanAudioFiles();
	scanMidiFiles();
}

// Destructor
State::~State ()
{
	spdlog::get("main")->info("killing State");
	track_ = NULL;
	clip_ = NULL;
	project_ = NULL;
}

std::string State::getConfigurationFileName()
{
	char fname[128] = "";
	sprintf( fname, "%s/.Kaliboulat.xml", getenv("HOME") );
	return std::string( fname );
}

void State::loadConfiguration()
{
	auto mainlog= spdlog::get( "main" );
	mainlog->debug( "State::saveConfiguration" );
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
	mainlog->debug( "/State::saveConfiguration" );
}

void State::saveConfiguration()
{
	auto mainlog= spdlog::get( "main" );
	mainlog->debug( "State::saveConfiguration" );
	mainlog->debug( "* audiodirs_ contains {} element(s)", audiodirs_.size() );
	mainlog->debug( "* {}", audiodirs_.at(0) );
	mainlog->debug( "* mididirs_ contains {} element(s)", mididirs_.size() );
	mainlog->debug( "* {}", mididirs_.at(0) );
	mainlog->debug( "* get stream" );
    std::ofstream os( getConfigurationFileName() );
	mainlog->debug( "* create archive" );
    cereal::XMLOutputArchive archive( os );
	mainlog->debug( "* serialize" );
    serialize( archive ); 
	mainlog->debug( "/State::saveConfiguration" );
}

void State::setProject( std::shared_ptr<Project> project )
{
	//delete project_;
	project_ = project;
}

void State::shared ()
{
	auto mainlog= spdlog::get("main");
	mainlog->info("Project shared_ptr count : {}", project_.use_count());
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
			projectlist_.push_back( str.substr( 0, str.length()-4 ) );
		}
	}
	return 0;
}

int State::scanProjects ()
{
	projectlist_.clear();
	ftw( user_dir().c_str(), scanProjectsCallback, 16 );
	std::sort( projectlist_.begin(), projectlist_.end() );
	return 0;
}

int State::scanAudioFilesCallback( const char *fpath, const struct stat *sb, int typeflag )
{
	char * localpath = strdup( fpath );
	if ( typeflag == FTW_F ) {
		if ( fnmatch( "*.wav", localpath, FNM_CASEFOLD ) == 0 ) {
			audiofiles_.push_back( localpath );
			spdlog::get( "main" )->debug( "found {}", localpath );
		}
	}
	return 0;
}

int State::scanAudioFiles ()
{
	audiofiles_.clear();
	for ( unsigned int i=0; i< audiodirs_.size(); i++ )
	{
		spdlog::get( "main" )->info( "scanning {}", audiodirs_[i].c_str() );
		ftw( audiodirs_[i].c_str(), scanAudioFilesCallback, 16 );
	}
	std::sort( audiofiles_.begin(), audiofiles_.end() );
	return 0;
}

int State::scanMidiFilesCallback (const char *fpath, const struct stat *sb, int typeflag)
{
	char * localpath = strdup( fpath );
	if ( typeflag == FTW_F ) {
		if ( fnmatch( "*.mid", localpath, FNM_CASEFOLD ) == 0 ) {
			midifiles_.push_back(localpath);
			spdlog::get( "main" )->debug( "found {}", localpath );
		}
	}
	return 0;
}

int State::scanMidiFiles ()
{
	midifiles_.clear();
	for ( unsigned int i=0; i< mididirs_.size(); i++ )
	{
		spdlog::get( "main" )->info("scanning {}", mididirs_[i].c_str());
		ftw(mididirs_[i].c_str(), scanMidiFilesCallback, 16);
	}
	std::sort( midifiles_.begin(), midifiles_.end() );
	return 0;
}

