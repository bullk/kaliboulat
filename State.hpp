#ifndef INC_STATE_H
#define INC_STATE_H

#include "Track.hpp"

class State
{
	private:
		// Constructor
		State ();
		// Destructor
		~State ();
		bool onoff_;
		Track * track_;
		Clip * clip_;
		static State * singleton_;

	public:
		inline bool isOn () { return onoff_; }
		inline void halt () { onoff_ = false; }
		inline Track * getTrack () { return track_; }
		inline void setTrack (Track * track) { track_ = track; }
		inline Clip * getClip () { return clip_; }
		inline void setClip (Clip * clip) { clip_ = clip; }
		
		// Singleton
		static State *getInstance ()
		{
			if ( NULL == singleton_ )
				singleton_ =  new State;

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
