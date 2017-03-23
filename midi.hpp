#ifndef INC_MIDI_H
#define INC_MIDI_H

#include <string>
#include <vector>
#include <map>
//#include <memory>
#include <RtMidi.h>
#include "Scheduled.hpp"
#include "Waiter.hpp"

typedef std::vector<unsigned char> RawMidi;

void midiInit ();
void midiPanic (RtMidiOut * midiout);
void midiCallback (double timeStamp, RawMidi *message, void *userData);
std::string char_vector_to_hex (RawMidi v);

// MIDI message types filtered by status byte, sorted by categories (events, channel mode, system common, realtime)
// source : https://www.midi.org/specifications/item/table-1-summary-of-midi-message

enum MidiMessageType { MMT_NOTE_OFF, MMT_NOTE_ON, MMT_AFTERTOUCH, MMT_CONTROL_CHANGE, MMT_PROGRAM_CHANGE, MMT_CHANNEL_AFTERTOUCH, MMT_PITCH_BEND,
						MMT_ALL_SOUND_OFF, MMT_RESET_ALL_CTRLS, MMT_LOCAL_CONTROL, MMT_ALL_NOTES_OFF,
						MMT_SYSEX, MMT_TIMECODE, MMT_SONG_POSITION_POINTER, MMT_SONG_SELECT, MMT_TUNE_REQUEST, MMT_SYSEX_END,
						MMT_CLOCK, MMT_START, MMT_CONTINUE, MMT_STOP, MMT_ACTIVE_SENSING, MMT_RESET };


class MidiMessage
{
public:
	MidiMessage( RawMidi data );
	~MidiMessage();
	inline RawMidi * getData() { return &data_; }
	std::string hexData();
	inline bool equals( RawMidi * const & v ) const { return data_ == *v; }
	
protected:
	int type;
	RawMidi data_;
};

inline bool operator==( MidiMessage const & m, RawMidi * const & v )
{
	return m.equals(v);
}

class ScheduledMidiMessage : public Scheduled, public MidiMessage
{
public:
	ScheduledMidiMessage (long unsigned int time, RawMidi data);
	~ScheduledMidiMessage ();

protected:
	int bar_, beat_, tick_;

};

RawMidi note_on_trigger( int, int );

RawMidi note_off_trigger( int, int );

class MidiWaiter
{
public:
	void addCommand( RawMidi, BaseCommand * );
	void deleteCommand( RawMidi, Clip * );
	bool inCommands ( RawMidi m );
	void execute ( RawMidi m );
	inline void preempt() { preemption_ = true; }
	inline void release() { preemption_ = false; }
	inline bool getPreemption() { return preemption_; }
	inline unsigned int assignmentSize() { return assignment_.size(); }
	RawMidi getAssignment();
	void setAssignment( RawMidi m );

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
	std::map<RawMidi, std::vector<BaseCommand *>> filters_;
	static MidiWaiter * singleton_;
	bool preemption_;
	RawMidi assignment_;
	
};

#endif

