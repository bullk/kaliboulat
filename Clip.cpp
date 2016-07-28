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
	return m_state;
}

void Clip::setState(ClipState s)
{
	m_state = s;
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
	m_name = path;
	m_state = CS_STOPPED;
	m_angle = 0;
	m_volume = 0.5f;
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
	return m_name;
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
	return &m_volume;
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
	//m_volume = v;
//}
//

//int AudioClip::getAngle(void)
//{
	//m_angle = (360 * time_) / getSize();
	//return m_angle;
//}
