#ifndef INC_LISTENER_H
#define INC_LISTENER_H

#include <stk/FileWvIn.h>
#include "AudioClip.hpp"
#include "MidiClip.hpp"

class Listener
{
	private:
		// Constructor
		Listener ();
		// Destructor
		~Listener ();
		float volume_;
		static bool onoff_;
		static stk::FileWvIn * audiofile_;
		static Listener * singleton_;

	public:
		inline float * getVolumeP () { return &volume_; }
		//inline stk::FileWvIn * getAudioFile () { return audiofile_; }
		//inline void setAudioFile (stk::FileWvIn * file) { audiofile_ = file; }
		stk::StkFloat tick (unsigned int channel = 0);

		static inline bool isOn () { return onoff_; }
		static inline void setOn () { onoff_ = true; }
		static inline void setOff () { onoff_ = false; }
		static inline bool * getOnOffP () { return &onoff_; }
		static void openFile (std::string filename) { audiofile_ -> openFile (filename, false, true); }
		static void closeFile () { if ( audiofile_->isOpen() ) audiofile_->closeFile (); }
		
		// Singleton
		static Listener *getInstance ()
		{
			if ( NULL == singleton_ )
				singleton_ =  new Listener;

			return singleton_;
		}

		static void kill ()
		{
			if ( NULL != singleton_ )
				{
					delete singleton_;
					singleton_ = NULL;
				}
		}
};

#endif
