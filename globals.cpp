#include "spdlog/spdlog.h"
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

DIR * testandcreatedir (std::string s)
{
	auto mainlog = spdlog::get("main");	
	int resmkdir;
	DIR * dir = opendir (s.c_str());
	if (dir == NULL)
	{
		mainlog->info("Création du dossier {}", s.c_str());
		resmkdir = mkdir (s.c_str(), S_IRWXU | S_IRWXG);
		if ( resmkdir )
		{
			mainlog->error("Impossible de créer le dossier {}", s.c_str());
			exit(1);
		}
		else dir = opendir (s.c_str());
	}
	return dir;
}

