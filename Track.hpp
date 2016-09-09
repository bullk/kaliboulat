#ifndef INC_TRACK_H
#define INC_TRACK_H

#include <string>


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
    //virtual void addClip (std::string path) = 0;
    virtual void deleteClip (unsigned int i) = 0;
    virtual void stopAll () = 0;
    
protected:
	TrackType type_;
	std::string name_;
	bool state_;

};

#endif