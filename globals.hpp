#ifndef GLOBALS_H
#define GLOBALS_H


#include <string>
#include <dirent.h>

enum DataType { NONE, AUDIO, MIDI, OSC, COM, SL };

std::string user_dir();

std::string name_from_path( std::string );

DIR * testandcreatedir( std::string );

#endif
