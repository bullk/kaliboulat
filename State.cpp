#include "State.hpp"

#include <stdio.h>
#include <stdlib.h>

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
	//audiodirs_ = new std::vector<std::string>;
	projectlist_ = new std::vector<std::string>;
	audiofiles_ = new std::vector<std::string>;
	midifiles_ = new std::vector<std::string>;
	loadconf ();
}

// Destructor
State::~State ()
{
	track_ = NULL;
	clip_ = NULL;
	//delete project_;
}

int State::loadconf()
{
	auto mainlog= spdlog::get("main");
	mainlog->info("cleaning audiodirs_");
	audiodirs_.clear();
	mainlog->info("opening conf file");
	FILE * cfile = NULL;
	char fname[64] = "";
	sprintf(fname, "%s/.Kaliboulat", getenv("HOME"));
	cfile = fopen(fname, "r");
	mainlog->info("reading conf");
	char buf[256] = "";
	while (fgets(buf, 256, cfile) != NULL)
	{
		char *p = strchr(buf, '\n');
		if (p) *p = 0;
		mainlog->info("adding {} to audiodirs_", buf);
		audiodirs_.push_back(std::string(buf));
	}
	fclose (cfile);
	return 0;
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
	auto mainlog = spdlog::get("main");
	char * localpath = strdup (fpath);
	//mainlog->info("{}", localpath);
	if (typeflag == FTW_F) {
		if (fnmatch("*.wav", localpath, FNM_CASEFOLD) == 0) {
			audiofiles_ -> push_back(localpath);
			//std::string str = std::string (basename(localpath));
			//audiofiles_ -> push_back(str.substr(0, str.length()-4));
			//mainlog->info("found {}", str.c_str());
			mainlog->info("found {}", localpath);
		}
	}
	return 0;
}

int State::scanAudioFiles ()
{
	auto mainlog = spdlog::get("main");
	audiofiles_->clear();
	for ( unsigned int i=0; i< audiodirs_.size(); i++ )
	{
		mainlog->info("scanning {}", audiodirs_[i].c_str());
		ftw(audiodirs_[i].c_str(), scanAudioFilesCallback, 16);
		//ftw("/home/commun/claude.mcmb.fr/APPRET", scanAudioFilesCallback, 16);
	}
	return 0;
}

int State::scanMidiFilesCallback (const char *fpath, const struct stat *sb, int typeflag)
{
	char * localpath = strdup (fpath);
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
	midifiles_->clear();
	ftw(getenv("HOME"), scanMidiFilesCallback, 16);
	return 0;
}

