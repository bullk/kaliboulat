#ifndef INC_SLClip_H
#define INC_SLClip_H

#include <stk/FileWvIn.h>
//#include <stk/LentPitShift.h>
#include <stk/PitShift.h>
#include "Clip.hpp"

#include <cereal/types/memory.hpp>
//#include <cereal/archives/binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/types/polymorphic.hpp>


class SLClip : public Clip
{
	
public:
	SLClip (std::string);
	SLClip (std::string, std::string, int, int, int, int, int, float, float, int);
	~SLClip ();
	inline void setSLid( int i ) { sl_id_ = i; }
	void SLload();
	inline std::string getURI() { return uri_; }
	inline void rewind() {}
	unsigned long getLength ();
	stk::StkFloat getTime ();
	float getProgress();
	float * getVolume ();
	void SLget( std::string );
	//void setVolume (StkFloat);

	template <class Archive>
	void serialize (Archive & archive)
	{
		archive (
			CEREAL_NVP(filename_),
			CEREAL_NVP(name_),
			CEREAL_NVP(launchstyle_),
			CEREAL_NVP(stopstyle_),
			CEREAL_NVP(loopstyle_),
			CEREAL_NVP(armMIDIchannel_),
			CEREAL_NVP(armMIDIkey_),
			CEREAL_NVP(volume_)
		);
	}
	
	template <class Archive>
	static void load_and_construct (Archive & archive, cereal::construct<SLClip> & construct)
	{
		std::string fname, name;
		int launch, stop, loop;
		int amc, amk;
		float volume, gui_rate;
		int gui_pitch;
		archive (fname, name, launch, stop, loop, amc, amk, volume, gui_rate, gui_pitch);
		construct (fname, name, launch, stop, loop, amc, amk, volume, gui_rate, gui_pitch);
	}
	
protected:    
	std::string uri_;
	int sl_id_;
	float volume_;
	void send2SL( const char*, lo_message );
	void SLload( std::string );
};

CEREAL_REGISTER_TYPE (SLClip)
CEREAL_REGISTER_POLYMORPHIC_RELATION (Clip, SLClip)

#endif
