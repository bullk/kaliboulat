#ifndef INC_CLIP_H
#define INC_CLIP_H

#include <string>
#include "globals.h"
#include "Engine.hpp"

class Clip
{
public:	
	enum ClipState { HALTED, ARMED, PLAYING, RECORDING };
	enum LaunchStyle { BAR, BEAT, FREE, RESYNC };
	enum LoopStyle { ONESHOT, MANY, FOREVER };
	
public:	
	Clip ();
	virtual ~Clip ();
	inline DataType dataType () { return data_type_; }
	inline ClipState getState () { return state_; }
	inline bool isPlaying () { return (state_ == PLAYING); }
	void arm ();
	inline void play () { std::cout << "clip started" << std::endl; state_ = PLAYING; }
	inline void stop () { state_ = HALTED; }
	inline virtual std::string getPath ()  { return path_; }
	inline virtual std::string getName () { return name_; }
	inline void virtual setName (std::string s) { name_ = s; }
	inline void virtual setLaunchStyle (LaunchStyle ls) { launchstyle_ = ls; }
	inline void virtual setStopStyle (LaunchStyle ls) { stopstyle_ = ls; }
	inline void virtual setLoopStyle (LoopStyle ls) { loopstyle_ = ls; }
	virtual unsigned long getLength() = 0;
	//virtual int getAngle (void) = 0;
   
protected:
	DataType data_type_;
	ClipState state_;
	std::string name_;
	std::string path_;
	LaunchStyle launchstyle_, stopstyle_;
	LoopStyle loopstyle_;
	//int angle_;
	
};

#endif
