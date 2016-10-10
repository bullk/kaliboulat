#ifndef INC_STATE_H
#define INC_STATE_H

#include "Project.hpp"

class State
{
	private:
		// Constructor
		State ();
		// Destructor
		~State ();
		bool onoff_;
		static State * singleton_;
		static std::shared_ptr<Project> project_;
		static std::shared_ptr<Track> track_;
		static Clip * clip_;

	public:
		inline bool isOn () { return onoff_; }
		inline void halt () { onoff_ = false; }
		static inline std::shared_ptr<Project> getProject () { return project_; }
		static inline void setProject (std::shared_ptr<Project> project) { project_ = project; }
		static inline std::shared_ptr<Track> getTrack () { return track_; }
		static inline void setTrack (std::shared_ptr<Track> track) { track_ = track; }
		static inline Clip * getClip () { return clip_; }
		static inline void setClip (Clip * clip) { clip_ = clip; }
		
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
