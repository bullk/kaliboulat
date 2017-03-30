#ifndef GLOBALS_H
#define GLOBALS_H

#define APP_NAME "Kaliboulat"
#define AUDIO_MODULE_NAME "Kaliboulat | Audio"
#define MIDIIN_MODULE_NAME "Kaliboulat | MIDI in"
#define MIDIOUT_MODULE_NAME "Kaliboulat | MIDI out"
#define APP_NAME "Kaliboulat"
#define FILE_EXT ".kal"
#define GLOBAL_SAMPLE_RATE 44100
#define AUDIO_INPUTS 1
#define AUDIO_OUTPUTS 1


#include <string>
#include <dirent.h>

enum DataType { NONE, AUDIO, MIDI, OSC, COM };

std::string user_dir();

std::string name_from_path( std::string );

DIR * testandcreatedir( std::string );

#endif
