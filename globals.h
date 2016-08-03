#ifndef GLOBALS_H
#define GLOBALS_H

#include <string>
#include <RtAudio.h>
#include <RtError.h>

#include "AudioGroup.h"
#include "MidiGroup.h"


// MIDI

string midiClipDir = "user/MIDI";
string midiClipLs[1] = { "test-Drums-1.mid" };

// Audio 

string sampleDir = "user/Audio";
string sampleLs[2];

const int GLOBAL_SAMPLE_RATE = 44100;

RtAudio dac;
AudioGroup audioMaster;
MidiGroup midiMaster;
bool mcState = false;

#endif
