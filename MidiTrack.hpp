#ifndef INC_MIDITRACK_H
#define INC_MIDITRACK_H

//#include <string>
#include "MidiClip.hpp"
#include "Track.hpp"

class MidiTrack : public Track
{

public:
	// Constructor 
	MidiTrack (std::string s);
	// Destructor 
	~MidiTrack ();

    void addClip (Clip * clip);
	void addClip (MidiClip * clip);
    void deleteClip (unsigned int i);
    inline MidiClip * getClip (unsigned int i) { return clipset_ -> at(i); }
    inline unsigned int nClips () { return clipset_ -> size(); }
	void tick (RtMidiOut *);
    void stopAll ();
	//inline std::vector<MidiClip *> * getClipSet () { return clipset_; }
    inline RtMidi * getOutput () { return output_; }
    inline void setOutput (RtMidi * o) { output_ = o; }
	
private:
	std::vector<MidiClip *> * clipset_;
	RtMidi * output_;

};

#endif
