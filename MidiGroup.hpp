#ifndef INC_MIDIGROUP_H
#define INC_MIDIGROUP_H

#include "MidiClip.hpp"

class MidiGroup
{

public:
	// Constructor 
	MidiGroup ();
	// Destructor 
	~MidiGroup ();

	void addClip (MidiClip * daClip);
    void deleteClip (unsigned int i);
	void tick (RtMidiOut *);
    void stopAll ();
	inline std::vector<MidiClip *> * getClipSet () { return this->ClipSet; }
	
private:
	std::vector<MidiClip *> * ClipSet;
	unsigned int selected;

};

#endif
