#ifndef INC_RESSOURCEFILE_H
#define INC_RESSOURCEFILE_H

#include "globals.hpp"

class RessourceFile
{
public:
	RessourceFile (DataType dt);
	RessourceFile (DataType dt, std::string path);
	virtual ~RessourceFile ();
	inline DataType dataType () { return data_type_; }
	inline std::string getPath () { return path_; }
	void setPath (std::string path);
	inline std::string getName () { return name_; }
	inline void setName (std::string name) { name_ = name; }
	
protected:
	DataType data_type_;
	std::string path_, name_;
};


#endif
