#ifndef INC_CLIP_H
#define INC_CLIP_H

#include <string>
#include <vector>
#include <stk/FileLoop.h>
#include <stk/PitShift.h>

/*		Classes Clip, AudioClip
 * 
 *
 */


class Clip
{
public:	
	enum ClipType { AUDIO, MIDI, OSC, COM };
	enum ClipState { STOPPED, PLAYING, RECORDING };
	enum LaunchStyle { FREE, BEAT, BAR };
	enum LoopStyle { ONESHOT, MANY, FOREVER };
	
	Clip ();
	virtual ~Clip ();
	inline ClipState getState () { return state_; }
	inline void setState (ClipState s) { state_ = s; }
	inline virtual std::string getPath ()  { return path_; }
	inline virtual std::string getName () { return name_; }
	inline void virtual setName (std::string s) { name_ = s; }
	virtual unsigned long getLength() = 0;
	//virtual int getAngle (void) = 0;
   
protected:    
	ClipType m_type;
	ClipState state_;
	std::string path_;
	std::string name_;
	LaunchStyle launchstyle_;
	LoopStyle loopstyle_;
	//int angle_;
	
};


class AudioClip : public Clip, public stk::FileLoop
{
	
public:
	AudioClip (std::string path);
	~AudioClip ();
	long unsigned int getLength ();
	stk::StkFloat getTime ();
	float * getVolume ();
	float * getGUIData ();
	float * getGUIRateP ();
	int * getGUIPitchP ();
	void updateRate ();
	void updatePitch ();
	stk::StkFloat tick (unsigned int channel = 0);
	//void setVolume (StkFloat);
	//int getAngle (void);
	
protected:    
	float volume_;
	float gui_rate_;
	int gui_pitch_;
	float gui_data_[];
	stk::PitShift * pitshift_;
};


#endif
