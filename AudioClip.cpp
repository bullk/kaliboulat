#include "AudioClip.hpp"
#include "State.hpp"

//-------------
// Constructor 
//-------------

AudioClip::AudioClip( std::string fname ) : Clip(), FileWvIn(),
	volume_(0.5f), gui_rate_(1.0f), gui_pitch_(0)
{
	data_type_ = AUDIO;
	filename_ = name_from_path( fname );
	int p = filename_.rfind(".");
	name_ = filename_.substr(0, p);
	launchstyle_ = LAUNCH_BAR;
	stopstyle_ = STOP_FREE;
	loopstyle_ = ONESHOT;
	std::string uri = State::getProject()->getAudioDir() + "/" + filename_;
	openFile( uri );
	initPitShift();
	//angle_ = 0;
	//gui_data_[data_.size()];
	//for ( unsigned int i=0; i<=data_.size(); i++ )	{ gui_data_[i] = (float) data_[i]; }
	//for ( unsigned int i=0; i<=sizeof(gui_data_); i++ )	{ gui_data_[i] = (float) data_[i]; }
}

AudioClip::AudioClip
	(std::string fname, std::string name, int launch, int stop, int loop, float vol, float rate, int pitch) :
	Clip(name, launch, stop, loop), FileWvIn(),
	volume_(vol), gui_rate_(rate), gui_pitch_(pitch)
{
	data_type_ = AUDIO;
	filename_ = name_from_path( fname );
	std::string uri = State::getProject()->getAudioDir() + "/" + filename_;
	openFile( uri );
	initPitShift();
	//angle_ = 0;
	//gui_data_[data_.size()];
	//for ( unsigned int i=0; i<=data_.size(); i++ )	{ gui_data_[i] = (float) data_[i]; }
	//for ( unsigned int i=0; i<=sizeof(gui_data_); i++ )	{ gui_data_[i] = (float) data_[i]; }
}


//------------
// Destructor 
//------------

AudioClip::~AudioClip()
{
	closeFile() ;
	delete pitshift_;
}

//------------

void AudioClip::initPitShift()
{
	//pitshift_ = new stk::LentPitShift();
	pitshift_ = new stk::PitShift();
	pitshift_ -> setEffectMix(1.0);
	updateRate(); updatePitch();
}

float * AudioClip::getGUIData () { return &gui_data_[0]; }

unsigned long AudioClip::getLength () { return getSize(); }

stk::StkFloat AudioClip::getTime () { return time_; }

float AudioClip::getProgress() { return time_ / getSize(); }

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
	stk::StkFloat res = 0;
	if ( isPlaying () )
	{
		if ( isFinished () )
		{
			reset();
			if ( loopstyle_ == FOREVER )
				res = pitshift_->tick( FileWvIn::tick(channel) ) * volume_;
			else
				stop ();
		}
		else
			res = pitshift_->tick( FileWvIn::tick(channel) ) * volume_;
	}
	return res;
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



