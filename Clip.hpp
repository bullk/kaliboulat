#ifndef INC_CLIP_H
#define INC_CLIP_H

#include <string>
#include "globals.h"
#include "Engine.hpp"

class Clip
{
public:	
	enum ClipState { HALTED, ARMED, PLAYING, RECORDING };
	enum LaunchStyle { LAUNCH_BAR, LAUNCH_BEAT, LAUNCH_FREE, LAUNCH_RESYNC };
	enum StopStyle { STOP_BAR, STOP_BEAT, STOP_FREE };
	enum LoopStyle { ONESHOT, MANY, FOREVER };
	const char * launchText[4] = { "BAR", "BEAT", "FREE", "RESYNC" };
	const char * stopText[3] = { "BAR", "BEAT", "FREE" };
	const char * loopText[3] = { "ONESHOT", "MANY", "FOREVER" };
	
public:	
	Clip();
	Clip( std::string );
	Clip( std::string, int, int, int, int );
	virtual ~Clip ();
	inline DataType dataType () { return data_type_; }
	inline ClipState getState () { return state_; }
	inline bool isPlaying () { return (state_ == PLAYING); }
	inline void select () { selected_ = true; }
	inline void unselect () { selected_ = false; }
	inline bool isSelected () { return selected_; }
	inline void play () { state_ = PLAYING; }
	inline void stop () { state_ = HALTED; }
	void armLaunch();
	void armStop();
	void arm ();
	void MIDIKey(char k);
	inline virtual std::string getFileName ()  { return filename_; }
	inline virtual std::string getName () { return name_; }
	inline virtual void setName (std::string s) { name_ = s; }
	inline virtual int getLaunchStyle () { return launchstyle_; }
	inline virtual const char* getLaunchStyleText () { return launchText[launchstyle_]; }
	inline virtual void setLaunchStyle (int ls) { launchstyle_ = ls; }
	inline virtual void nextLaunchStyle () { launchstyle_ = (launchstyle_ + 1) % 4; }
	inline virtual int getStopStyle () { return stopstyle_; }
	inline virtual const char* getStopStyleText () { return stopText[stopstyle_]; }
	inline virtual void setStopStyle (int ls) { launchstyle_ = ls; }
	inline virtual void nextStopStyle () { stopstyle_ = (stopstyle_ + 1) % 3; }
	inline virtual int getLoopStyle () { return loopstyle_; }
	inline virtual const char* getLoopStyleText () { return loopText[loopstyle_]; }
	inline virtual void setLoopStyle (int ls) { launchstyle_ = ls; }
	inline virtual void nextLoopStyle () { loopstyle_ = (loopstyle_ + 1) % 3; }
	inline int * getArmMIDIChannelP () { return &armMIDIchannel_; }
	inline int * getArmMIDIKeyP () { return &armMIDIkey_; }
	virtual unsigned long getLength() = 0;
	virtual float getProgress() = 0;
	//virtual int getAngle (void) = 0;
   
protected:
	DataType data_type_;
	ClipState state_;
	std::string name_, filename_;
	int launchstyle_, stopstyle_, loopstyle_;
	int armMIDIchannel_, armMIDIkey_;
	bool selected_;
	float progress_;

	//int angle_;
	
};

#endif
