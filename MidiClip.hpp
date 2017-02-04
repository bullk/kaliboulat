#ifndef INC_MIDICLIP_H
#define INC_MIDICLIP_H

#include "Clip.hpp"

#include <cereal/types/memory.hpp>
//#include <cereal/archives/binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/types/polymorphic.hpp>

class RtMidiOut;
class ScheduledMidiMessage;

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
	
	template <class Archive>
	static void load_and_construct( Archive & ar, cereal::construct<MidiClip> & construct )
	{
		std::string name;
		ar( name );
		construct( name );
	}
	
    template<class Archive>
	void serialize(Archive & archive)
	{
		archive (
			//CEREAL_NVP(data_type_),
			CEREAL_NVP(path_),
			CEREAL_NVP(name_),
			CEREAL_NVP(launchstyle_),
			CEREAL_NVP(stopstyle_),
			CEREAL_NVP(loopstyle_),
			CEREAL_NVP(division_),
			CEREAL_NVP(length_)
		);
	}

protected:
	int division_;
	unsigned long length_, time_, index_;
	std::vector<ScheduledMidiMessage *> * data_;

};

CEREAL_REGISTER_TYPE(MidiClip)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Clip, MidiClip)

#endif
