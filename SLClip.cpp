#include <lo/lo.h>
#include "spdlog/spdlog.h"

#include "SLClip.hpp"
#include "SLBus.hpp"
#include "State.hpp"
#include "Project.hpp"

//-------------
// Constructor 
//-------------

SLClip::SLClip( std::string fname ) : Clip(), volume_(0.5f)
{
	data_type_ = AUDIO;
	uri_ = fname;
	filename_ = name_from_path( uri_ );
	int p = filename_.rfind( "." );
	name_ = filename_.substr( 0, p );
	launchstyle_ = LAUNCH_BAR;
	stopstyle_ = STOP_FREE;
	loopstyle_ = ONESHOT;
}

SLClip::SLClip
	(std::string fname, std::string name, int launch, int stop, int loop, int amc, int amk, float vol, float rate, int pitch) :
	Clip(name, launch, stop, loop, amc, amk), volume_(vol)
{
	data_type_ = AUDIO;
	uri_ = fname;
	filename_ = name_from_path( uri_ );
}


//------------
// Destructor 
//------------

SLClip::~SLClip()
{
}

//------------

void SLClip::send2SL(const char* pathend, lo_message msg)
{
	auto mainlog = spdlog::get( "main" );
	mainlog->info( "SLClip::send2SL");
	lo_message_pp( msg );
	lo_address target = ((SLBus*)parent_)->getSLTarget();
	mainlog->info( "* address : {}", target);
	std::string path = "/sl/" + std::to_string( sl_id_ ) + pathend;
	mainlog->info( "* path : {}", path);
	if ( lo_send_message( target, path.c_str(), msg ) == -1 ) {
		mainlog->error(
			"OSC error {}: {}", lo_address_errno(target), lo_address_errstr(target)
		);
	}
	mainlog->info( "/SLClip::send2SL");
}

void SLClip::SLload()
{
	SLload( uri_ );
}

void SLClip::SLload( std::string path )
{
	auto mainlog = spdlog::get( "main" );
	mainlog->info( "SLClip::SLload");
	lo_message msg = lo_message_new();
	lo_message_add( msg, "sss", path.c_str(), "osc.udp://localhost:7300", "/error");
	//lo_message_add( msg, "sss", path.c_str(), "localhost:7300", "/test");
	//mainlog->info( "* message : {}", msg);
	send2SL( "/load_loop", msg );
	mainlog->info( "/SLClip::SLload");
}

void SLClip::SLget( std::string control )
{
	auto mainlog = spdlog::get( "main" );
	mainlog->info( "SLClip::SLget");
	lo_message msg = lo_message_new();
	mainlog->info( "* new OSC message");
	std::string rpath = "/sl/" + ((SLBus*)parent_)->getName() + "/" + std::to_string(sl_id_);
	mainlog->info( "* rpath : {}", rpath);
	lo_message_add( msg, "sss", control.c_str(), "osc.udp://localhost:7300", rpath.c_str() );
	send2SL( "/get", msg );
	mainlog->info( "/SLClip::SLget");
}

unsigned long SLClip::getLength () { return 0; }

stk::StkFloat SLClip::getTime () { return 0.0f; }

float SLClip::getProgress() {return 0.0f; }

float * SLClip::getVolume () { return &volume_; }


//void SLClip::setVolume(StkFloat v)
//{
	//volume_ = v;
//}
//

//int SLClip::getAngle(void)
//{
	//angle_ = (360 * time_) / getSize();
	//return angle_;
//}



