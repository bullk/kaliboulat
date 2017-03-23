#include "globals.hpp"

std::string user_dir()
{
	return std::string( getenv( "HOME" ) ) + "/Documents/Kaliboulat";
}

std::string name_from_path (std::string path)
{
	int p = path.rfind("/") + 1;
	return path.substr(p, path.length()-p);
}

