#ifndef INC_MIDITRACK_H
#define INC_MIDITRACK_H

#include "MidiClip.hpp"

class MidiTrack
{

public:
	// Constructor 
	MidiTrack ();
	// Destructor 
	~MidiTrack ();

	void addClip (MidiClip * daClip);
    void deleteClip (unsigned int i);
	void tick (RtMidiOut *);
    void stopAll ();
	inline std::vector<MidiClip *> * getClipSet () { return this->ClipSet; }
    inline RtMidi * getOutput () { return output_; }
    inline void setOutput (RtMidi * o) { output_ = o; }
	
private:
	std::vector<MidiClip *> * ClipSet;
	//unsigned int selected;
	RtMidi * output_;

};

#endif
