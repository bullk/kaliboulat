#ifndef INC_MIDITRACK_H
#define INC_MIDITRACK_H

//#include <string>
#include "MidiClip.hpp"
#include "Track.hpp"

#include <cereal/archives/xml.hpp>
#include <cereal/types/polymorphic.hpp>

class MidiTrack : public Track
{

public:
	// Constructor
	MidiTrack ();
	MidiTrack (std::string);
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
		archive (
			CEREAL_NVP(data_type_),
			CEREAL_NVP(type_str_),
			CEREAL_NVP(name_),
			CEREAL_NVP(hue_)
		);
	}
	
private:
	std::vector<MidiClip *> * clipset_;
	RtMidi * output_;

};

CEREAL_REGISTER_TYPE(MidiTrack)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Track, MidiTrack)

#endif
