#ifndef INC_CLIP_H
#define INC_CLIP_H

#include <string>

/*		Classes Clip, AudioClip
 * 
 *
 */


class Clip
{
public:	
	enum ClipType { AUDIO, MIDI, OSC, COM };
	enum ClipState { HALTED, ARMED, PLAYING, RECORDING };
	enum LaunchStyle { BAR, BEAT, FREE, RESYNC };
	enum LoopStyle { ONESHOT, MANY, FOREVER };
	
	Clip ();
	virtual ~Clip ();
	inline ClipState getState () { return state_; }
	inline bool isPlaying () { return (state_ == PLAYING); }
	inline void play () { state_ = PLAYING; }
	inline void stop () { state_ = HALTED; }
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
	LaunchStyle launchstyle_, stopstyle_;
	LoopStyle loopstyle_;
	//int angle_;
	
};

#endif
