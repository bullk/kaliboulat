#ifndef INC_MIDI_H
#define INC_MIDI_H

#include <string>
#include <vector>
#include <map>
//#include <memory>
#include <RtMidi.h>
#include "Scheduled.hpp"
#include "Engine.hpp"

typedef std::vector<unsigned char> MidiRaw;

void midiInit ();
void midiPanic (RtMidiOut * midiout);
void midiCallback (double timeStamp, MidiRaw *message, void *userData);
std::string char_vector_to_hex (MidiRaw v);

// MIDI message types filtered by status byte, sorted by categories (events, channel mode, system common, realtime)
// source : https://www.midi.org/specifications/item/table-1-summary-of-midi-message

enum MidiMessageType { MMT_NOTE_OFF, MMT_NOTE_ON, MMT_AFTERTOUCH, MMT_CONTROL_CHANGE, MMT_PROGRAM_CHANGE, MMT_CHANNEL_AFTERTOUCH, MMT_PITCH_BEND,
						MMT_ALL_SOUND_OFF, MMT_RESET_ALL_CTRLS, MMT_LOCAL_CONTROL, MMT_ALL_NOTES_OFF,
						MMT_SYSEX, MMT_TIMECODE, MMT_SONG_POSITION_POINTER, MMT_SONG_SELECT, MMT_TUNE_REQUEST, MMT_SYSEX_END,
						MMT_CLOCK, MMT_START, MMT_CONTINUE, MMT_STOP, MMT_ACTIVE_SENSING, MMT_RESET };


class MidiMessage
{
public:
	MidiMessage( MidiRaw data );
	~MidiMessage();
	inline MidiRaw * getData() { return &data_; }
	std::string hexData();
	inline bool equals( MidiRaw * const & v ) const { return data_ == *v; }
	
protected:
	int type;
	MidiRaw data_;
};

inline bool operator==( MidiMessage const & m, MidiRaw * const & v )
{
	return m.equals(v);
}

class ScheduledMidiMessage : public Scheduled, public MidiMessage
{
public:
	ScheduledMidiMessage (long unsigned int time, MidiRaw data);
	~ScheduledMidiMessage ();

protected:
	int bar_, beat_, tick_;

};

MidiRaw note_on_trigger (char, char) ;

MidiRaw note_off_trigger (char, char) ;

class MidiWaiter
{
public:
	void addCommand( MidiRaw, BaseCommand * );
	void deleteCommand( MidiRaw, BaseCommand * );

	// Singleton
	static MidiWaiter *getInstance ()
	{
		if ( NULL == singleton_ )
			singleton_ =  new MidiWaiter;

		return singleton_;
	}

	static void kill ()
	{
		if ( NULL != singleton_ )
			{
				delete singleton_;
				singleton_ = NULL;
			}
	}
	
private:
	// Constructor
	MidiWaiter ();
	// Destructor
	~MidiWaiter ();
	std::map<MidiRaw, std::vector<BaseCommand>> filters_;
	static MidiWaiter * singleton_;
	
};

#endif

