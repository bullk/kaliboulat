#include "Clock.h"

Clock::Clock ()
{
	state = false;
	tempo = 120;
	ticks_per_beat = 960;
	beats_per_bar = 4;
	tick_duration = 60.0f / (tempo * ticks_per_beat);
	rewind ();
}

Clock::~Clock ()
{}

void Clock::start ()
{
	if (not(state))
	{
		startTime = std::chrono::system_clock::now();
		state = true;
	}
}

void Clock::stop ()
{
	state = false;
}
	
void Clock::rewind ()
{
	bar = 1;
	beat = 1;
	tick = 0;
	hour = 0;
	minute = 0;
	second = 0;
	previous_ticks = 0;
	now_ticks = 0;
}
	
unsigned int Clock::update ()
{
	unsigned int midi_ticks = 0;
	if (state) {

		now = std::chrono::system_clock::now();
		delta = std::chrono::duration_cast<std::chrono::microseconds> (now - startTime).count();
		// h:m:s
		int tot_s = delta / 1000000;
		int tot_m = tot_s / 60;
		hour = tot_m / 60;
		minute = tot_m % 60;
		second = tot_s % 60;

		// MIDI (bar, beat, tick)
		now_ticks = (long unsigned int) (delta / (1000000 * tick_duration));
		unsigned int nbeats = now_ticks / ticks_per_beat;
		tick = now_ticks % ticks_per_beat;
		beat = 1 + nbeats % beats_per_bar;
		bar = 1 + nbeats / beats_per_bar;

		// how many MIDI ticks ?
		if (now_ticks < previous_ticks)
			previous_ticks = now_ticks;
		unsigned int ticks_delta = now_ticks - previous_ticks;
		if ((previous_ticks == 0) and (ticks_delta == 0)) midi_ticks = 1;
		if (ticks_delta > 0) midi_ticks = ticks_delta;

		// MIDI Clock update
		previous_ticks = now_ticks;
	}
	return midi_ticks;
}

bool Clock::getState () { return state; }

int Clock::getHour () { return hour; }

int Clock::getMinute () { return minute; }

int Clock::getSecond () { return second; }

int Clock::getBar () { return bar; }

int Clock::getBeat () { return beat; }

int Clock::getTick () { return tick; }


