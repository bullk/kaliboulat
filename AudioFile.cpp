#include "AudioFile.hpp"
 

AudioFile::AudioFile (std::string path) :
	RessourceFile(AUDIO, path), FileWvIn(path, false, true, 1024, 1024)
{
}

AudioFile::~AudioFile ()
{
}

