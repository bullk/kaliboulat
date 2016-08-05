#ifndef INC_CLIP_H
#define INC_CLIP_H

#include <string>
#include <vector>
#include <stk/FileLoop.h>
#include <stk/PitShift.h>
#include <stk/MidiFileIn.h>
#include "midi.h"

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
	virtual unsigned long getLength() = 0;
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
	long unsigned int getLength ();
	StkFloat getTime ();
	float * getVolume ();
	float * getGUIData ();
	float * getGUIRateP ();
	int * getGUIPitchP ();
	void updateRate ();
	void updatePitch ();
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


class MidiClip : public Clip, public MidiFileIn
{
public:
	MidiClip (string path);
	~MidiClip ();
	void parse ();
	long unsigned int getLength ();
	
protected:
	vector<ScheduledMidiMessage *> * data_;
};

#endif
