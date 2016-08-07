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

std::string Clip::getName ()
{
	return name_;
}
	
std::string Clip::getPath ()
{
	return path_;
}

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
	//angle_ = 0;
	volume_ = 0.5f;
	pitshift_ = new PitShift();
	pitshift_->setEffectMix(1.0);
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

float * AudioClip::getGUIData ()
{
	return &gui_data_[0];
}

unsigned long AudioClip::getLength ()
{
	return getSize();
}

StkFloat AudioClip::getTime ()
{
	return time_;
}

float * AudioClip::getVolume ()
{
	return &volume_;
}

float * AudioClip::getGUIRateP ()
{
	
	return &gui_rate_;
}

int * AudioClip::getGUIPitchP ()
{
	
	return &gui_pitch_;
}

void AudioClip::updateRate ()
{
	setRate( (StkFloat) gui_rate_ );
}

void AudioClip::updatePitch ()
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




//-------------
// Constructor 
//-------------

MidiClip::MidiClip (std::string path) : MidiFileIn (path)
{
	path_ = path;
	int p = path_.rfind("/") + 1;
	name_ = path_.substr(p, path_.length()-p);
	length_ = 0;
	time_ = 0;
	parse ();
	//data_ = new vector<ScheduledMidiMessage *>;
}


//------------
// Destructor 
//------------

MidiClip::~MidiClip ()
{
    //if (data_ != NULL){
        //for (unsigned int i = 0; i < data_->size(); i++)
        //{
            //delete data_->at(i);
        //}
    //}
}

//------------

long unsigned int MidiClip::getLength () { return length_; }

long unsigned int MidiClip::getTime () { return time_; }

void MidiClip::rewind ()
{
	time_ = 0;
	for (unsigned int i=0; i<getNumberOfTracks(); i++) rewindTrack(i);
}

void MidiClip::tick ()
{
	if (time_ == length_) rewind ();
	for (unsigned int i=0; i<getNumberOfTracks(); i++)
	{
		
		track_indexes_[i]++;
		
	}
	time_++;
}

void MidiClip::parse()
{
	track_indexes_.clear();
	vector< unsigned char > * event = new vector<unsigned char>();
	unsigned long delta_time, abs_time;
	for (unsigned int i=0; i<getNumberOfTracks(); i++)
	{
		track_indexes_.push_back(0);
		rewindTrack(i);
		abs_time=0;
		delta_time = getNextEvent(event, i);
		track_indexes_[i] = delta_time;
		while ( event->size() > 0 )
		{
			abs_time += delta_time;
			delta_time = getNextEvent(event, i);
		}
		if (abs_time > length_) length_ = abs_time;
		rewindTrack(i);
	}
	delete event;
}
