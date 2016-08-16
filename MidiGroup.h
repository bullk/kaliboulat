#ifndef INC_MIDIGROUP_H
#define INC_MIDIGROUP_H

#include "MidiClip.h"

class MidiGroup
{

public:
	// Constructor 
	MidiGroup ();
	// Destructor 
	~MidiGroup ();

	std::vector<MidiClip *> * getClipSet ();
	void addClip (MidiClip * daClip);
	//unsigned long getLength ();
	void tick (RtMidiOut *);
    void stopAll ();
	
private:
	std::vector<MidiClip *> * ClipSet;
	unsigned int selected;

};

#endif
