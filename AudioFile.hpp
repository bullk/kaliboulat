#ifndef INC_AUDIOFILE_H
#define INC_AUDIOFILE_H

#include <string>
#include "Engine.hpp"


class AudioFile : public RessourceFile
{
public:
	AudioFile (std::string path);
	~AudioFile ();

};
	
void importAudioFile (std::string name_in, std::string name_out);

#endif
