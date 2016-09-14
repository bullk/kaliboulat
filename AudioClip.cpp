#include "AudioClip.hpp"


//-------------
// Constructor 
//-------------

AudioClip::AudioClip(std::string path) : Clip(), FileWvIn(path)
{
	m_type = AUDIO;
	openFile(path);
	path_ = path;
	int p = path_.rfind("/") + 1;
	std::string file = path_.substr(p, path_.length()-p);
	p = file.rfind(".");
	name_ = file.substr(0, p);
	launchstyle_ = BAR;
	stopstyle_ = FREE;
	loopstyle_ = ONESHOT;
	//angle_ = 0;
	setRate(1);
	volume_ = 0.5f;
	pitshift_ = new stk::PitShift();
	pitshift_ -> setEffectMix(1.0);
	gui_rate_ = 1.0f;
	gui_pitch_ = 0;
	//gui_data_[data_.size()];
	//for ( unsigned int i=0; i<=data_.size(); i++ )	{ gui_data_[i] = (float) data_[i]; }
	//for ( unsigned int i=0; i<=sizeof(gui_data_); i++ )	{ gui_data_[i] = (float) data_[i]; }
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

float * AudioClip::getGUIData () { return &gui_data_[0]; }

unsigned long AudioClip::getLength () { return getSize(); }

stk::StkFloat AudioClip::getTime () { return time_; }

float * AudioClip::getVolume () { return &volume_; }

float * AudioClip::getGUIRateP () { return &gui_rate_; }

int * AudioClip::getGUIPitchP () { return &gui_pitch_; }

void AudioClip::updateRate ()
{
	setRate( (stk::StkFloat) gui_rate_ );
}

void AudioClip::updatePitch ()
{
	pitshift_->setShift( (stk::StkFloat) pow(2, ((float)gui_pitch_/12)) );
}

stk::StkFloat AudioClip::tick (unsigned int channel)
{
	if ( isFinished () )
	{
		stop ();
		reset ();
		return 0;
	}
	else return pitshift_->tick(FileWvIn::tick(channel));
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



