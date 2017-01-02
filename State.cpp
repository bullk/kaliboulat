#include "State.hpp"

#include <ftw.h>
#include <fnmatch.h>
#include <libgen.h>

// Initialisation du singleton à NULL
State * State::singleton_ = NULL;
std::shared_ptr<Project> State::project_ = NULL;
std::shared_ptr<Track> State::track_ = NULL;
std::shared_ptr<Clip> State::clip_ = NULL;
std::vector<std::string> * State::projectlist_ = NULL;
std::vector<std::string> * State::audiofiles_ = NULL;
std::vector<std::string> * State::midifiles_ = NULL;

// Constructor
State::State () : onoff_(true)
{
	projectlist_ = new std::vector<std::string>;
}

// Destructor
State::~State ()
{
}

int State::scanProjectsCallback (const char *fpath, const struct stat *sb, int typeflag)
{
	char * localpath = strdup (fpath);
    if (typeflag == FTW_F) {
		if (fnmatch("*.kal", localpath, FNM_CASEFOLD) == 0) {
			std::string str = std::string (basename(localpath));
			projectlist_ -> push_back(str.substr(0, str.length()-4));
		}
    }
    return 0;
}

int State::scanProjects ()
{
	projectlist_->clear();
    ftw(user_dir().c_str(), scanProjectsCallback, 16);
    return 0;
}

int State::scanAudioFilesCallback (const char *fpath, const struct stat *sb, int typeflag)
{
	char * localpath = strdup (user_dir().c_str());
    if (typeflag == FTW_F) {
		if (fnmatch("*.wav", localpath, FNM_CASEFOLD) == 0) {
			std::string str = std::string (basename(localpath));
			audiofiles_ -> push_back(str.substr(0, str.length()-4));
		}
    }
    return 0;
}

int State::scanAudioFiles ()
{
	projectlist_->clear();
    ftw(user_dir().c_str(), scanAudioFilesCallback, 16);
    return 0;
}

int State::scanMidiFilesCallback (const char *fpath, const struct stat *sb, int typeflag)
{
	char * localpath = strdup (user_dir().c_str());
    if (typeflag == FTW_F) {
		if (fnmatch("*.mid", localpath, FNM_CASEFOLD) == 0) {
			std::string str = std::string (basename(localpath));
			midifiles_ -> push_back(str.substr(0, str.length()-4));
		}
    }
    return 0;
}

int State::scanMidiFiles ()
{
	projectlist_->clear();
    ftw(user_dir().c_str(), scanMidiFilesCallback, 16);
    return 0;
}

