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
	path_ = path;
	int p = path_.rfind("/") + 1;
	name_ = path_.substr(p, path_.length()-p);
	state_ = CS_STOPPED;
	angle_ = 0;
	volume_ = 0.5f;
	pitshift_ = new PitShift();
	pitshift_->setEffectMix(1.0);
	gui_rate_ = 1.0f;
	gui_pitch_ = 0;
	//float * gui_data_[data_.size()];
	//for ( unsigned int i=0; i<=data_.size(); i++ )	{ *gui_data_[i] = (float) data_[i]; }
	updateRate(); updatePitch();
}


//------------
// Destructor 
//------------

AudioClip::~AudioClip()
{
	delete pitshift_;
}

//------------

std::string AudioClip::getName ()
{
	return name_;
}
	
std::string AudioClip::getPath ()
{
	return path_;
}

float * AudioClip::getGUIData ()
{
	return gui_data_;
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

float * AudioClip::getGUIRateP (void)
{
	
	return &gui_rate_;
}

int * AudioClip::getGUIPitchP (void)
{
	
	return &gui_pitch_;
}

void AudioClip::updateRate (void)
{
	setRate( (StkFloat) gui_rate_ );
}

void AudioClip::updatePitch (void)
{
	pitshift_->setShift( (StkFloat) pow(2, ((float)gui_pitch_/12)) );
}

StkFloat AudioClip::tick (unsigned int channel)
{
	return pitshift_->tick(FileLoop::tick(channel));
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
