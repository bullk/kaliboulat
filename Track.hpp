#ifndef INC_TRACK_H
#define INC_TRACK_H

#include <string>
#include "Clip.hpp"


class Track
{
	
public:
	// Constructor 
	Track (std::string s);
	// Destructor 
	virtual ~Track ();

	inline DataType dataType () { return data_type_; }
	inline std::string getTypeStr () { return type_str_; }
	inline std::string getName () { return name_; }
	inline void setName (std::string s) { name_ = s; }
	inline void setState (bool b) { state_ = b; }
	inline void muteSwitch () { state_ = not(state_); }
	inline bool isPlaying () { return state_; }
    virtual void addClip (Clip * clip) = 0;
    virtual void deleteClip (unsigned int i) = 0;
    virtual Clip * getClip (unsigned int i) = 0;
    virtual unsigned int nClips () = 0;
    virtual void stopAll () = 0;
	inline float * getHueP () { return &hue_; }
	inline float getHue () { return hue_; }
	inline void setHue (float f) { hue_ = f; }
    
protected:
	DataType data_type_;
	std::string type_str_;
	std::string name_;
	bool state_;
	float hue_ = 0.0f;

};

/*
class BaseTrack
{
public:
	 //Constructor 
	BaseTrack ();
	 //Destructor 
	virtual ~BaseTrack ();

	inline std::string getName () { return name_; }
	inline void setName (std::string s) { name_ = s; }
	inline void setState (bool b) { state_ = b; }
	inline void muteSwitch () { state_ = not(state_); }
	inline bool isPlaying () { return state_; }
	//inline float * getHueP () { return &hue_; }
	inline float getHue () { return hue_; }
	inline void setHue (float f) { hue_ = f; }
    
protected:
	std::string type_str_;
	std::string name_;
	bool state_;
	float hue_ = 0.0f;
	
};

template <typename C>
class ClipBox
{
public:
	 //Constructor 
	ClipBox ();
	 //Destructor 
	virtual ~ClipBox ();

	inline DataType dataType () { return data_type_; }
	inline std::string getTypeStr () { return type_str_; }
    virtual void addClip (C * clip) = 0;
    virtual void deleteClip (unsigned int i) = 0;
    virtual C * getClip (unsigned int i) = 0;
    virtual unsigned int nClips () = 0;
    virtual void stopAll () = 0;
    
protected:
	DataType data_type_;
	
};
*/
#endif
