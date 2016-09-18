#ifndef INC_MYCLOCK_H
#define INC_MYCLOCK_H

#include <chrono>
#include "Engine.hpp"

// Clock

class Clock
{

private:
	bool state;
	std::chrono::time_point<std::chrono::system_clock> startTime;
	std::chrono::time_point<std::chrono::system_clock> now;
	long long unsigned int delta;
	int tempo, ticks_per_beat, beats_per_bar;
	float tick_duration;
	int bar, beat, tick;
	int hour, minute, second;
	long unsigned int previous_ticks, now_ticks;

public:
	Clock ();
	~Clock ();
	void start ();
	void stop ();
	void rewind ();
	unsigned int update ();
	bool getState ();
	inline bool isStarted () { return state; }
	int getHour ();
	int getMinute ();
	int getSecond ();
	int getBar ();
	int getBeat ();
	int getTick ();

};

#endif
