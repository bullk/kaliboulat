#ifndef INC_AUDIOFILE_H
#define INC_AUDIOFILE_H

#include <string>
#include <stk/FileWvIn.h>
#include "RessourceFile.hpp"


class AudioFile : public RessourceFile, public stk::FileWvIn
{
public:
	AudioFile (std::string path);
	~AudioFile ();

};
	

#endif
