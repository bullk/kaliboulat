#ifndef INC_AUDIOFILE_H
#define INC_AUDIOFILE_H

#include <string>
#include "RessourceFile.hpp"


class AudioFile : public RessourceFile
{
public:
	AudioFile (std::string path);
	~AudioFile ();

};
	

#endif
