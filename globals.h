#ifndef GLOBALS_H
#define GLOBALS_H

#include <string>
#include <RtAudio.h>
#include <RtError.h>

#include "AudioGroup.h"

//Audio 

string sampleDir = "user/Audio";
string sampleLs[2];

RtAudio dac;
AudioGroup audioMaster;

#endif
