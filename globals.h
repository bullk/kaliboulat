#ifndef GLOBALS_H
#define GLOBALS_H

#include <string>
#include <chrono>
#include <RtAudio.h>
#include <RtError.h>

#include "AudioGroup.h"
#include "MidiGroup.h"


bool mcState = false;
std::chrono::time_point<std::chrono::system_clock> mcStartTime;
std::chrono::time_point<std::chrono::system_clock> mcNow;
long long unsigned int mcDelta;

// MIDI

string midiClipDir = "user/MIDI";
MidiGroup midiMaster;

string midiClipLs[1] = { "test-Drums-1.mid" };

// Audio 

const int GLOBAL_SAMPLE_RATE = 44100;
string sampleDir = "user/Audio";

RtAudio dac;
AudioGroup audioMaster;

string sampleLs[2] = { "bar.wav", "hellosine.wav" };


#endif
