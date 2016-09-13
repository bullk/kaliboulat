#ifndef INC_AUDIOCLIP_H
#define INC_AUDIOCLIP_H

#include <string>
#include <stk/FileWvIn.h>
#include <stk/PitShift.h>
#include "Clip.hpp"

/*		Classe AudioClip
 * 
 *
 */


class AudioClip : public Clip, public stk::FileWvIn
{
	
public:
	AudioClip (std::string path);
	~AudioClip ();
	long unsigned int getLength ();
	stk::StkFloat getTime ();
	float * getVolume ();
	float * getGUIData ();
	float * getGUIRateP ();
	int * getGUIPitchP ();
	void updateRate ();
	void updatePitch ();
	stk::StkFloat tick (unsigned int channel = 0);
	//void setVolume (StkFloat);
	//int getAngle (void);
	
protected:    
	float volume_;
	float gui_rate_;
	int gui_pitch_;
	float gui_data_[];
	stk::PitShift * pitshift_;
};


#endif
