#ifndef INC_MYCLOCK_H
#define INC_MYCLOCK_H

#include <chrono>

// Clock

class Clock
{

public:
	Clock ();
	~Clock ();
	void start ();
	void stop ();
	void rewind ();
	unsigned int update ();
	bool getState ();
	int getHour ();
	int getMinute ();
	int getSecond ();
	int getBar ();
	int getBeat ();
	int getTick ();

protected:
	bool state;
	std::chrono::time_point<std::chrono::system_clock> startTime;
	std::chrono::time_point<std::chrono::system_clock> now;
	long long unsigned int delta;
	int tempo, ticks_per_beat, beats_per_bar;
	float tick_duration;
	int bar, beat, tick;
	int hour, minute, second;
	long unsigned int previous_ticks, now_ticks;

};

#endif
