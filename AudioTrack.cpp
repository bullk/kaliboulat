//#include <stdlib.h>
/* Pour fork() */
#include <unistd.h>
#include <errno.h>
#include "spdlog/spdlog.h"

#include "State.hpp"
#include "AudioTrack.hpp"


//-------------
// Constructor
//-------------

AudioTrack::AudioTrack (std::string s) : Track (AUDIO, "Audio", s), volume_(1.0f)
{
	hue_ =  0.25f + (float)((rand() % 31) -15) / 100;
	startSL();
}

AudioTrack::AudioTrack (std::string s, float h, float v, std::vector<std::shared_ptr<AudioClip>> cs) :
	Track (AUDIO, "Audio", s, h), volume_(v), clipset_(cs)
{
	startSL();
	for ( std::vector<std::shared_ptr<AudioClip>>::iterator it=clipset_.begin(); it<clipset_.end(); it++ )
		(*it)->setParent( this );
}

//------------
// Destructor 
//------------

AudioTrack::~AudioTrack ()
{
	while (clipset_.size())
		clipset_.pop_back();
}


//------------
// Add a clip
//------------

void AudioTrack::startSL()
{
	auto mainlog = spdlog::get( "main" );	
	sl_port_ = State::getInstance()->newOSCport();
	char port_arg[32] = "";
	sprintf( port_arg, "--osc-port=%u", sl_port_ );
	char *arg[] = { "sooperlooper", "--loopcount=0", port_arg, NULL };
	do {
		sl_pid_ = fork();
	} while ( (sl_pid_ == -1) && (errno == EAGAIN) );

	switch (sl_pid_)
	{
	case -1:
		mainlog->info( "launching superlooper : Fork failed" );
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

void AudioTrack::addClip (std::string path, int tn)
{
	auto mainlog = spdlog::get("main");	
	mainlog->debug("AudioTrack::addClip");
	std::shared_ptr<AudioClip> clip (new AudioClip(path));
	addClip ( clip );
	mainlog->debug("/AudioTrack::addClip");
}

void AudioTrack::addClip (std::shared_ptr<Clip> clip)
{
	addClip ( std::static_pointer_cast<AudioClip>(clip)  );
}

void AudioTrack::addClip (std::shared_ptr<AudioClip> clip)
{
	clipset_.push_back( clip );
	clip -> setParent( this );
	Waiter::getInstance() -> selectClip( clip );
}

void AudioTrack::deleteClip (unsigned int i)
{
	std::shared_ptr<AudioClip> clip = clipset_.at(i);
	clipset_.erase (clipset_.begin() + i);
}

void AudioTrack::stopAll ()
{
	for ( unsigned int i=0; i < clipset_.size(); i++ )
		clipset_.at(i) -> stop();
}
		
stk::StkFloat AudioTrack::tick() const
{
	register stk::StkFloat sample;

	sample = 0;
	for ( unsigned int j = 0; j < clipset_.size (); j++ )
	{
		sample += clipset_.at(j) -> tick();
	}
	return sample;
}


