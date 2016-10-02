#ifndef INC_MIDIFILE_H
#define INC_MIDIFILE_H

#include <vector>
#include <stk/MidiFileIn.h>
#include "MidiTrack.hpp"


class MidiFile : public stk::MidiFileIn, public RessourceFile
{
public:
	MidiFile (std::string path);
	~MidiFile ();
	void rewind ();
	void parse ();
	MidiClip * getTrack (unsigned int i);
	inline unsigned int nClips () { return miditracks_ -> size(); }
	//long unsigned int getLength ();
	//long unsigned int getTime ();
	//void tick (RtMidiOut *);
	
protected:
	long unsigned int length_, time_;
	std::vector<MidiClip *> * miditracks_;
	//std::vector<long unsigned int> track_indexes_;
};

#endif
