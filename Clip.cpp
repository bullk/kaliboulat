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
	m_name = path;
	m_state = CS_STOPPED;
	m_angle = 0;
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

//int AudioClip::getAngle(void)
//{
	//m_angle = (360 * time_) / getSize();
	//return m_angle;
//}
