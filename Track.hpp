#ifndef INC_TRACK_H
#define INC_TRACK_H

#include <string>
#include "Clip.hpp"

class Track
{
	
public:
	enum TrackType { AUDIO, MIDI, OSC, COM };
	
	// Constructor 
	Track (std::string s);
	// Destructor 
	virtual ~Track ();

	inline TrackType getType () { return type_; }
	inline std::string getName () { return name_; }
	inline void setName (std::string s) { name_ = s; }
	inline void setState (bool b) { state_ = b; }
	inline void muteSwitch () { state_ = not(state_); }
	inline bool isPlaying () { return state_; }
    virtual void addClip (std::string path) = 0;
    virtual void deleteClip (unsigned int i) = 0;
    virtual Clip * getClip (unsigned int i) = 0;
    virtual unsigned int nClips () = 0;
    virtual void stopAll () = 0;
	inline float * getHueP () { return &hue_; }
	inline float getHue () { return hue_; }
	inline void setHue (float f) { hue_ = f; }
    
protected:
	TrackType type_;
	std::string name_;
	bool state_;
	float hue_ = 0.0f;

};

#endif
