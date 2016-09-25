#ifndef INC_AUDIOFILE_H
#define INC_AUDIOFILE_H

#include <string>
#include "Engine.hpp"


class RessourceFile
{
public:
	RessourceFile (DataType dt, std::string path);
	virtual ~RessourceFile ();
	inline DataType dataType () { return data_type_; }
	inline std::string getPath () { return path_; }
	inline std::string getName () { return name_; }

protected:
	DataType data_type_;
	std::string path_, name_;
};


class AudioFile : public RessourceFile
{
public:
	AudioFile (std::string path);
	~AudioFile ();

};
	

#endif
