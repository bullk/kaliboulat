#ifndef INC_AUDIOCLIP_H
#define INC_AUDIOCLIP_H

#include <stk/FileWvIn.h>
//#include <stk/LentPitShift.h>
#include <stk/PitShift.h>
#include "Clip.hpp"

#include <cereal/types/memory.hpp>
//#include <cereal/archives/binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/types/polymorphic.hpp>


class AudioClip : public Clip, public stk::FileWvIn
{
	
public:
	AudioClip (std::string);
	AudioClip (std::string, std::string, int, int, int, float, float, int);
	~AudioClip ();
	unsigned long getLength ();
	stk::StkFloat getTime ();
	float getProgress();
	float * getVolume ();
	float * getGUIData ();
	float * getGUIRateP ();
	int * getGUIPitchP ();
	void updateRate ();
	void updatePitch ();
	stk::StkFloat tick (unsigned int channel=0);
	//void setVolume (StkFloat);
	//int getAngle (void);

	template <class Archive>
	void serialize (Archive & archive)
	{
		archive (
			CEREAL_NVP(filename_),
			CEREAL_NVP(name_),
			CEREAL_NVP(launchstyle_),
			CEREAL_NVP(stopstyle_),
			CEREAL_NVP(loopstyle_),
			CEREAL_NVP(volume_),
			CEREAL_NVP(gui_rate_),
			CEREAL_NVP(gui_pitch_)
		);
	}
	
	template <class Archive>
	static void load_and_construct (Archive & archive, cereal::construct<AudioClip> & construct)
	{
		std::string fname, name;
		int launch, stop, loop;
		float volume, gui_rate;
		int gui_pitch;
		archive (fname, name, launch, stop, loop, volume, gui_rate, gui_pitch);
		construct (fname, name, launch, stop, loop, volume, gui_rate, gui_pitch);
	}
	
protected:    
	float volume_;
	float gui_rate_;
	int gui_pitch_;
	float gui_data_[];
	//stk::LentPitShift * pitshift_;
	stk::PitShift * pitshift_;
	void initPitShift();
	
};

CEREAL_REGISTER_TYPE (AudioClip)
CEREAL_REGISTER_POLYMORPHIC_RELATION (Clip, AudioClip)

#endif
