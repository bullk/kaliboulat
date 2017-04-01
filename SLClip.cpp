#include "SLClip.hpp"
#include "State.hpp"
#include "Project.hpp"

//-------------
// Constructor 
//-------------

SLClip::SLClip( std::string fname ) : Clip(), volume_(0.5f)
{
	data_type_ = AUDIO;
	filename_ = name_from_path( fname );
	int p = filename_.rfind( "." );
	name_ = filename_.substr( 0, p );
	launchstyle_ = LAUNCH_BAR;
	stopstyle_ = STOP_FREE;
	loopstyle_ = ONESHOT;
	std::string uri = State::getProject()->getAudioDir() + "/" + filename_;
}

SLClip::SLClip
	(std::string fname, std::string name, int launch, int stop, int loop, int amc, int amk, float vol, float rate, int pitch) :
	Clip(name, launch, stop, loop, amc, amk), volume_(vol)
{
	data_type_ = AUDIO;
	filename_ = name_from_path( fname );
	std::string uri = State::getProject()->getAudioDir() + "/" + filename_;
}


//------------
// Destructor 
//------------

SLClip::~SLClip()
{
}

//------------

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



