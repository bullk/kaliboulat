#ifndef INC_MIDITRACK_H
#define INC_MIDITRACK_H

//#include <string>
#include "MidiClip.hpp"
#include "Track.hpp"

class MidiTrack : public Track
{

public:
	// Constructor
	//MidiTrack ();
	MidiTrack (std::string s="MidiTrack");
	// Destructor 
	~MidiTrack ();

	void addClip (Clip * clip);
	void addClip (MidiClip * clip);
	void deleteClip (unsigned int i);
	inline MidiClip * getClip (unsigned int i) const { return clipset_ -> at(i); }
	inline unsigned int nClips () const { return clipset_ -> size(); }
	void tick (RtMidiOut *);
	void stopAll ();
	//inline std::vector<MidiClip *> * getClipSet () { return clipset_; }
	inline RtMidi * getOutput () const { return output_; }
	inline void setOutput (RtMidi * o) { output_ = o; }
	
    template<class Archive>
	void serialize(Archive & archive)
	{
		archive (data_type_, type_str_, name_, hue_);
	}
	
private:
	std::vector<MidiClip *> * clipset_;
	RtMidi * output_;

};

#endif
