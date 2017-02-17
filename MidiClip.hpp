#ifndef INC_MIDICLIP_H
#define INC_MIDICLIP_H

#include "Clip.hpp"
#include "midi.hpp"

#include <cereal/types/memory.hpp>
//#include <cereal/archives/binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/types/polymorphic.hpp>

class RtMidiOut;
//class ScheduledMidiMessage;

class MidiClip : public Clip
{

public:
	MidiClip (std::string);
	MidiClip (std::string, int);
	MidiClip (std::string, int, std::string, int, int, int);
	~MidiClip ();
	inline unsigned long getLength () { return length_; }
	inline void setLength (unsigned long l) { length_ = l; }
	inline unsigned int getDivScale () const { return divscale_; }
	inline void setDivScale (unsigned int d) { divscale_ = d; }
	inline unsigned int getDivision () const { return division_; }
	inline void setDivision (unsigned int d) { division_ = d; }
	inline unsigned long getTime () { return time_; }
	//void setTime (long unsigned int time);
	float getProgress() { return (float) time_ / length_; }
	inline unsigned long getIndex () { return index_; }
	inline unsigned long getSize () { return events_.size(); }
	inline ScheduledMidiMessage * getEvent (unsigned long i) { return &events_[i]; }
	void rewind ();
	void tick (RtMidiOut *);
	void appendEvent (unsigned long time, std::vector<unsigned char> * event);
	
    template<class Archive>
	void serialize(Archive & archive)
	{
		archive (
			CEREAL_NVP(filename_),
			CEREAL_NVP(tracknum_),
			CEREAL_NVP(name_),
			CEREAL_NVP(launchstyle_),
			CEREAL_NVP(stopstyle_),
			CEREAL_NVP(loopstyle_)
		);
	}

	template <class Archive>
	static void load_and_construct( Archive & archive, cereal::construct<MidiClip> & construct )
	{
		std::string filename, name;
		int tracknum, launchstyle, stopstyle, loopstyle;
		archive ( filename, tracknum, name, launchstyle, stopstyle, loopstyle );
		construct ( filename, tracknum, name, launchstyle, stopstyle, loopstyle );
	}
	
protected:
	unsigned int tracknum_, division_, divscale_;
	unsigned long length_, time_, index_, clock_time_;
	std::vector<ScheduledMidiMessage> events_;
	void getEventsFromSource (bool);

};

CEREAL_REGISTER_TYPE (MidiClip)
CEREAL_REGISTER_POLYMORPHIC_RELATION (Clip, MidiClip)

#endif
