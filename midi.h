#ifndef INC_MIDI_H
#define INC_MIDI_H

#include <string>
#include <vector>
#include "Scheduled.h"


// MIDI message types filtered by status byte, sorted by categories (events, channel mode, system common, realtime)
// source : https://www.midi.org/specifications/item/table-1-summary-of-midi-message

enum MidiMessageType { MMT_NOTE_OFF, MMT_NOTE_ON, MMT_AFTERTOUCH, MMT_CONTROL_CHANGE, MMT_PROGRAM_CHANGE, MMT_CHANNEL_AFTERTOUCH, MMT_PITCH_BEND,
						MMT_ALL_SOUND_OFF, MMT_RESET_ALL_CTRLS, MMT_LOCAL_CONTROL, MMT_ALL_NOTES_OFF,
						MMT_SYSEX, MMT_TIMECODE, MMT_SONG_POSITION_POINTER, MMT_SONG_SELECT, MMT_TUNE_REQUEST, MMT_SYSEX_END,
						MMT_CLOCK, MMT_START, MMT_CONTINUE, MMT_STOP, MMT_ACTIVE_SENSING, MMT_RESET };


class MidiMessage
{
public:
	MidiMessage (std::vector<unsigned char> * data);
	~MidiMessage ();
	inline std::vector<unsigned char> * getData () { return data_; }
	std::string hexData ();
	
protected:
	int type;
	std::vector<unsigned char> * data_;
	
};



class ScheduledMidiMessage : public Scheduled, public MidiMessage
{
public:
	ScheduledMidiMessage (long unsigned int time, std::vector<unsigned char> * data);
	~ScheduledMidiMessage ();

protected:
	int bar_, beat_, tick_;

};


#endif

