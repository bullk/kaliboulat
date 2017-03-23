#ifndef INC_CLIP_H
#define INC_CLIP_H

#include <string>
#include "globals.hpp"
#include "midi.hpp"

class Track;

class Clip
{
public:	
	enum ClipState { HALTED, ARMED, PLAYING, RECORDING };
	enum LaunchStyle { LAUNCH_BAR, LAUNCH_BEAT, LAUNCH_FREE, LAUNCH_RESYNC };
	enum StopStyle { STOP_BAR, STOP_BEAT, STOP_FREE };
	enum LoopStyle { ONESHOT, FOREVER };
	const char * launchText[4] = { "BAR", "BEAT", "FREE", "RESYNC" };
	const char * stopText[3] = { "BAR", "BEAT", "FREE" };
	const char * loopText[2] = { "ONESHOT", "FOREVER" };
	
public:	
	Clip();
	Clip( std::string );
	Clip( std::string, int, int, int, int, int );
	virtual ~Clip ();
	inline DataType dataType () { return data_type_; }
	inline ClipState getState () { return state_; }
	inline Track * getParent() { return parent_; }
	inline void setParent(Track * t) { parent_ = t; }
	inline bool isPlaying () { return (state_ == PLAYING); }
	inline void select () { selected_ = true; }
	inline void unselect () { selected_ = false; }
	inline bool isSelected () { return selected_; }
	inline void play () { state_ = PLAYING; }
	virtual void rewind() = 0;
	void stop();
	void armLaunch();
	void armStop();
	void arm ();
	void setMIDIKey();
	void delMIDIKey();
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
	inline virtual void nextLoopStyle () { loopstyle_ = (loopstyle_ + 1) % 2; }
	inline int getArmMIDIChannel () { return armMIDIchannel_; }
	inline int getArmMIDIKey () { return armMIDIkey_; }
	void setArmMIDI( RawMidi );
	virtual unsigned long getLength() = 0;
	virtual float getProgress() = 0;
	//virtual int getAngle (void) = 0;
   
protected:
	DataType data_type_;
	ClipState state_;
	Track * parent_;
	std::string name_, filename_;
	int launchstyle_, stopstyle_, loopstyle_;
	int armMIDIchannel_, armMIDIkey_;
	bool selected_;
	float progress_;

	//int angle_;
	
};

#endif
