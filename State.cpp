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

// Constructor
State::State () : onoff_(true)
{
	projectlist_ = new std::vector<std::string>;
}

// Destructor
State::~State ()
{
}

int State::scanCallback(const char *fpath, const struct stat *sb, int typeflag)
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
    ftw(user_dir().c_str(), scanCallback, 16);
    return 0;
}

