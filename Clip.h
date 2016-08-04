#ifndef INC_CLIP_H
#define INC_CLIP_H

#include <string>
#include <stk/FileLoop.h>
#include <stk/PitShift.h>

#include "midifile/include/MidiFile.h"
#include "midifile/include/Options.h"
#include <iostream>
#include <iomanip>

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
	Clip ();
	~Clip ();
	ClipState getState ();
	void setState (ClipState);
	virtual string getPath ();
	virtual string getName ();
	virtual unsigned long getLength(void) = 0;
	//virtual int getAngle (void) = 0;
   
protected:    
	ClipType m_type;
	ClipState state_;
	string path_;
	string name_;
	int angle_;
	
};


class AudioClip : public Clip, public FileLoop
{
	
public:
	AudioClip (string path);
	~AudioClip ();
	long unsigned int getLength (void);
	StkFloat getTime (void);
	float * getVolume (void);
	float * getGUIData ();
	float * getGUIRateP (void);
	int * getGUIPitchP (void);
	void updateRate (void);
	void updatePitch (void);
	StkFloat tick (unsigned int channel = 0);
	//void setVolume (StkFloat);
	//int getAngle (void);
	
protected:    
	float volume_;
	float gui_rate_;
	int gui_pitch_;
	float gui_data_[];
	PitShift * pitshift_;
};


class MidiClip : public Clip, public MidiFile
{
public:
	MidiClip (string path);
	~MidiClip ();
	long unsigned int getLength ();
	
protected:
	float data;
};

#endif
