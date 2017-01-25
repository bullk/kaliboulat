#ifndef INC_MIDITRACK_H
#define INC_MIDITRACK_H

#include "MidiClip.hpp"
#include "Track.hpp"

#include <cereal/types/vector.hpp>
//#include <cereal/archives/binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/types/polymorphic.hpp>


class MidiTrack : public Track
{

public:
	// Constructor
	MidiTrack (std::string name);
	// Destructor 
	~MidiTrack ();

	void addClip (std::shared_ptr<Clip> clip);
	void addClip (std::shared_ptr<MidiClip> clip);
	void deleteClip (unsigned int i);
	inline std::shared_ptr<Clip> getClip (unsigned int i) const { return clipset_.at(i); }
	inline unsigned int nClips () const { return clipset_.size(); }
	void tick (RtMidiOut *);
	void stopAll ();
	//inline std::vector<std::shared_ptr<MidiClip>> * getClipSet () { return clipset_; }
	inline RtMidiOut * getOutput () const { return output_; }
	inline void setOutput (RtMidiOut * o) { output_ = o; }
	
	template <class Archive>
	static void load_and_construct( Archive & ar, cereal::construct<MidiTrack> & construct )
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
			//CEREAL_NVP(type_str_),
			CEREAL_NVP(name_),
			CEREAL_NVP(hue_),
			CEREAL_NVP(clipset_)
		);
	}
	
private:
	std::vector<std::shared_ptr<MidiClip>> clipset_;
	RtMidiOut * output_;

};

CEREAL_REGISTER_TYPE(MidiTrack)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Track, MidiTrack)

#endif
