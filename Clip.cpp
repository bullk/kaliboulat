#include <string>
#include "Clip.h"

//-------------
// Constructor 
//-------------

Clip::Clip()
{
}


//------------
// Destructor 
//------------

Clip::~Clip()
{
}

//------------

ClipState Clip::getState()
{
	return state_;
}

void Clip::setState(ClipState s)
{
	state_ = s;
}

//-------------
// Constructor 
//-------------

AudioClip::AudioClip(std::string path) : FileLoop(path)
{
	m_type = CT_AUDIO;
	//static unsigned int clipAutoNumber = 0;
	openFile(path);
	setRate(1);
	gui_rate_ = 1.0f;
	path_ = path;
	int p = path_.rfind("/") + 1;
	name_ = path_.substr(p, path_.length()-p);
	state_ = CS_STOPPED;
	angle_ = 0;
	volume_ = 0.5f;
}


//------------
// Destructor 
//------------

AudioClip::~AudioClip()
{
}

//------------

std::string AudioClip::getName(void)
{
	return name_;
}
	
std::string AudioClip::getPath(void)
{
	return path_;
}

unsigned long AudioClip::getLength(void)
{
	return getSize();
}

StkFloat AudioClip::getTime(void)
{
	return time_;
}

float * AudioClip::getVolume(void)
{
	return &volume_;
}

float * AudioClip::getGUIRateP(void)
{
	
	return &gui_rate_;
}

void AudioClip::updateRate(void)
{
	setRate( (StkFloat) gui_rate_ );
}

//void AudioClip::setVolume(StkFloat v)
//{
	//volume_ = v;
//}
//

//int AudioClip::getAngle(void)
//{
	//angle_ = (360 * time_) / getSize();
	//return angle_;
//}
