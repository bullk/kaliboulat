#ifndef INC_MIDIFILE_H
#define INC_MIDIFILE_H

#include <stk/MidiFileIn.h>
#include "MidiGroup.hpp"


class MidiFile : public stk::MidiFileIn
{
public:
	MidiFile (std::string path);
	~MidiFile ();
	void rewind ();
	void parse (MidiGroup * midigroup_p);
	//long unsigned int getLength ();
	//long unsigned int getTime ();
	//void tick (RtMidiOut *);
	
protected:
	std::string path_, name_;
	long unsigned int length_, time_;
	//std::vector<long unsigned int> track_indexes_;
};

#endif
