#ifndef INC_MIDICLIP_H
#define INC_MIDICLIP_H

#include <RtMidi.h>
#include "midi.hpp"
#include "Clip.hpp"


class MidiClip : public Clip
{
public:
	MidiClip (std::string name);
	~MidiClip ();
	inline unsigned long getLength () { return length_; }
	inline void setLength (unsigned long l) { length_ = l; }
	inline int getDivision () const { return division_; }
	inline void setDivision (int d) { division_ = d; }
	inline unsigned long getTime () { return time_; }
	//void setTime (long unsigned int time);
	inline unsigned long getIndex () { return index_; }
	inline unsigned long getSize () { return data_ -> size(); }
	inline ScheduledMidiMessage * getEvent (unsigned long i) { return data_ -> at(i); }
	void rewind ();
	void tick (RtMidiOut *);
	void appendEvent (unsigned long time, std::vector<unsigned char> * event);
	
protected:
	int division_;
	unsigned long length_, time_, index_;
	std::vector<ScheduledMidiMessage *> * data_;
};

#endif
