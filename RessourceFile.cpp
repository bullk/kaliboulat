#include "RessourceFile.hpp"

RessourceFile::RessourceFile (DataType dt) : data_type_(dt)
{
}

RessourceFile::RessourceFile (DataType dt, std::string path) : data_type_(dt), path_(path)
{
	name_ = name_from_path (path_);
}

RessourceFile::~RessourceFile ()
{
}

void RessourceFile::setPath (std::string path)
{
	path_ = path;
	name_ = name_from_path (path_);
}

