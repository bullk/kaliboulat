//#include <stdlib.h>
/* Pour fork() */
#include <unistd.h>
#include <errno.h>
#include <stk/FileWvIn.h>
#include "spdlog/spdlog.h"

#include "State.hpp"
#include "SLBus.hpp"


//-------------
// Constructor
//-------------

SLBus::SLBus( std::string s ) : Track( AUDIO, "SooperLooper", s ), volume_(1.0f)
{
	hue_ =  1.0f + (float)((rand() % 21) -10) / 100;
	hue_ -= ( hue_ >= 1.0f ); 
	startSL();
}

SLBus::SLBus( std::string s, float h, float v, std::vector<std::shared_ptr<SLClip>> cs ) :
	Track( AUDIO, "SooperLooper", s, h ), volume_(v), clipset_(cs)
{
	startSL();
	for ( std::vector<std::shared_ptr<SLClip>>::iterator it=clipset_.begin(); it<clipset_.end(); it++ )
		(*it)->setParent( this );
}

//------------
// Destructor 
//------------

SLBus::~SLBus()
{
	if ( lo_send( sl_target_, "/quit", "" ) == -1 ) {
		spdlog::get( "main" )->error(
			"OSC error {}: {}", lo_address_errno(sl_target_), lo_address_errstr(sl_target_)
		);
	}

	while (clipset_.size())
		clipset_.pop_back();
}


//------------
// Add a clip
//------------

void SLBus::startSL()
{
	auto mainlog = spdlog::get( "main" );	
	sl_port_ = State::getInstance()->newOSCport();
	sl_target_ = lo_address_new( NULL, std::to_string( sl_port_ ).c_str() );
	std::string s_port_arg = "--osc-port=" + std::to_string( sl_port_ );
	char * port_arg = const_cast<char*>( s_port_arg.c_str() );
	std::string s_jack_client_arg = "--jack-name=sl-" + name_;
	char * jack_client_arg = const_cast<char*>( s_jack_client_arg.c_str() );
	char *arg[] = { "sooperlooper", "--loopcount=0", "--discrete-io=no",
		port_arg, jack_client_arg, NULL };
	do {
		sl_pid_ = fork();
	} while ( ( sl_pid_ == -1 ) && ( errno == EAGAIN ) );

	switch ( sl_pid_ )
	{
	case -1:
		mainlog->error( "launching superlooper : Fork failed" );
		break;
	case 0:
		if ( execv( "/usr/bin/sooperlooper", arg ) == -1 )
		{
			mainlog->info( "launching superlooper : execv failed" );
			exit(EXIT_FAILURE);
		}
		break;
	default:
		mainlog->info( "launching superlooper pid {} on port {}", sl_pid_, sl_port_ );
		break;
	}
}

void SLBus::send2SL()
{
	//if ( lo_send( sl_target_, "/quit", "" ) == -1 ) {
		//spdlog::get( "main" )->error(
			//"OSC error {}: {}", lo_address_errno(sl_target_), lo_address_errstr(sl_target_)
		//);
	//}
}

void SLBus::addClip( std::string path, int tn )
{
	auto mainlog = spdlog::get( "main" );	
	mainlog->debug( "SLBus::addClip" );
	std::shared_ptr<SLClip> clip( new SLClip( path ) );
	addClip( clip );
	mainlog->debug( "/SLBus::addClip" );
}

void SLBus::addClip( std::shared_ptr<Clip> clip )
{
	addClip( std::static_pointer_cast<SLClip>(clip) );
}

void SLBus::addClip( std::shared_ptr<SLClip> clip )
{
	stk::FileWvIn f( clip->getURI() );
	
	if ( lo_send( sl_target_, "/loop_add", "if", f.channelsOut(), 10.0f ) == -1 ) {
		spdlog::get( "main" )->error(
			"OSC error {}: {}", lo_address_errno(sl_target_), lo_address_errstr(sl_target_)
		);
	}
	else {
		clip->setParent( this );
		clip->setSLid( clipset_.size() );
		clip->SLload();
		clipset_.push_back( clip );
		Waiter::getInstance() -> selectClip( clip );
	}
}

void SLBus::deleteClip( unsigned int i )
{
	std::shared_ptr<SLClip> clip = clipset_.at(i);
	clipset_.erase (clipset_.begin() + i);
}

void SLBus::stopAll()
{
	for ( unsigned int i=0; i < clipset_.size(); i++ )
		clipset_.at(i) -> stop();
}
		
