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

enum ClipType { CT_AUDIO, CT_MIDI, CT_OSC, CT_COM };
enum ClipState { CS_STOPPED, CS_PLAYING, CS_RECORDING };

class Clip
{
	
public:
	Clip ();
	virtual ~Clip ();
	ClipState getState ();
	void setState (ClipState);
	virtual std::string getPath ();
	virtual std::string getName ();
	virtual unsigned long getLength() = 0;
	//virtual int getAngle (void) = 0;
   
protected:    
	ClipType m_type;
	ClipState state_;
	std::string path_;
	std::string name_;
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
