#ifndef INC_MIDICLIP_H
#define INC_MIDICLIP_H

#include <RtMidi.h>
#include "midi.h"
#include "Clip.h"


class MidiClip : public Clip
{
public:
	MidiClip ();
	MidiClip (std::string name);
	~MidiClip ();
	long unsigned int getLength ();
	void setLength (long unsigned int length);
	long unsigned int getTime ();
	void setTime (long unsigned int time);
	void rewind ();
	void tick (RtMidiOut *);
	void appendEvent (long unsigned int time, std::vector<unsigned char> * event);
	
protected:
	long unsigned int length_, time_, index_;
	std::vector<ScheduledMidiMessage *> * data_;
};

#endif
