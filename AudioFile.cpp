#include "AudioFile.hpp"

RessourceFile::RessourceFile (DataType dt, std::string path) : data_type_(dt), path_(path)
{
	int p = path_.rfind("/") + 1;
	name_ = path_.substr(p, path_.length()-p);
}

RessourceFile::~RessourceFile ()
{
}

AudioFile::AudioFile (std::string path) : RessourceFile(AUDIO, path)
{
}

AudioFile::~AudioFile ()
{
}

