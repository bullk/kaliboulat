#ifndef INC_STATE_H
#define INC_STATE_H

#include <RtMidi.h>
#include "cereal/types/vector.hpp"
#include "cereal/types/string.hpp"
#include <cereal/types/memory.hpp>
#include <cereal/archives/xml.hpp>

class Clip;
class Track;
class Project;

class State
{
private:
	// Constructor
	State ();
	// Destructor
	~State ();
	RtMidiOut * midiout_;
	bool onoff_;
	static State * singleton_;
	std::shared_ptr<Project> project_;
	std::shared_ptr<Track> track_;
	std::shared_ptr<Clip> clip_;
	std::vector<std::string> audiodirs_;
	std::vector<std::string> mididirs_;
	static std::vector<std::string> projectlist_;
	static std::vector<std::string> audiofiles_;
	static std::vector<std::string> midifiles_;
	static int scanProjectsCallback(const char *fpath, const struct stat *sb, int typeflag);
	static int scanAudioFilesCallback(const char *fpath, const struct stat *sb, int typeflag);
	static int scanMidiFilesCallback(const char *fpath, const struct stat *sb, int typeflag);
	std::string getConfigurationFileName();
	void loadConfiguration();

public:
	RtMidiOut * getMidiOut();
	void setMidiOut( RtMidiOut * midiout );
	inline bool isOn () { return onoff_; }
	inline void halt () { onoff_ = false; }
	void saveConfiguration();
	inline std::shared_ptr<Project> getProject () { return project_; }
	void setProject( std::shared_ptr<Project> );
	void shared ();
	inline std::shared_ptr<Track> getTrack () { return track_; }
	inline void setTrack (std::shared_ptr<Track> track) { track_ = track; }
	inline std::shared_ptr<Clip> getClip () { return clip_; }
	inline void setClip (std::shared_ptr<Clip> clip) { clip_ = clip; }
	inline void unsetClip() { clip_ = NULL; }
	int scanProjects ();
	int scanAudioFiles ();
	int scanMidiFiles ();
	static inline std::vector<std::string> * getProjectList () { return &projectlist_; }
	static inline std::vector<std::string> * getAudioFiles () { return &audiofiles_; }
	static inline std::vector<std::string> * getMidiFiles () { return &midifiles_; }
	
	template<class Archive>
	void serialize(Archive & archive)
	{
		archive (
			CEREAL_NVP(audiodirs_),
			CEREAL_NVP(mididirs_)
		);
	}
	
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
