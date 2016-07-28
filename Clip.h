#ifndef INC_CLIP_H
#define INC_CLIP_H

#include <string>
//#include <stk/FileRead.h>
#include <stk/FileLoop.h>

/*		Classes Clip, AudioClip, MidiClip
 * 
 *
 */

using namespace std;
using namespace stk;

enum ClipType { CT_AUDIO, CT_MIDI, CT_OSC, CT_COM };
enum ClipState { CS_STOPPED, CS_PLAYING, CS_RECORDING };

class Clip
{
	
public:
	Clip();
	~Clip();
	ClipState getState();
	void setState(ClipState);
	virtual string getName(void) = 0;
	virtual unsigned long getLength(void) = 0;
	//virtual int getAngle(void) = 0;
   
protected:    
	ClipType m_type;
	ClipState m_state;
	string path_;
	string m_name;
	string m_location;
	int m_angle;
	
};


class AudioClip : public Clip, public FileLoop
{
	
public:
	AudioClip(string path);
	~AudioClip();
	string getName(void);
	long unsigned int getLength(void);
	StkFloat getTime(void);
	float * getVolume(void);
	float * getGUIRateP(void);
	void updateRate(void);
	//void setVolume(StkFloat);
	//int getAngle(void);
	
protected:    
	float m_volume;
	float gui_rate_;
};


class MidiClip : public Clip
{

};

#endif
