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
		static std::shared_ptr<Clip> clip_;
		static std::vector<std::string> * projectlist_;
		static std::vector<std::string> * audiofiles_;
		static std::vector<std::string> * midifiles_;
		static int scanProjectsCallback(const char *fpath, const struct stat *sb, int typeflag);
		static int scanAudioFilesCallback(const char *fpath, const struct stat *sb, int typeflag);
		static int scanMidiFilesCallback(const char *fpath, const struct stat *sb, int typeflag);

	public:
		inline bool isOn () { return onoff_; }
		inline void halt () { onoff_ = false; }
		static inline std::shared_ptr<Project> getProject () { return project_; }
		static inline void setProject (std::shared_ptr<Project> project) { project_ = project; }
		static inline std::shared_ptr<Track> getTrack () { return track_; }
		static inline void setTrack (std::shared_ptr<Track> track) { track_ = track; }
		static inline std::shared_ptr<Clip> getClip () { return clip_; }
		static inline void setClip (std::shared_ptr<Clip> clip) { clip_ = clip; }
		static int scanProjects ();
		static int scanAudioFiles ();
		static int scanMidiFiles ();
		static inline std::vector<std::string> * getProjectList () { return projectlist_; }
		static inline std::vector<std::string> * getAudioFiles () { return audiofiles_; }
		static inline std::vector<std::string> * getMidiFiles () { return midifiles_; }
		
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
