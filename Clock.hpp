#ifndef INC_MYCLOCK_H
#define INC_MYCLOCK_H

#include <chrono>


// Clock

class Clock
{

private:
	bool state_;
	std::chrono::time_point<std::chrono::system_clock> startTime_;
	std::chrono::time_point<std::chrono::system_clock> now_;
	long long unsigned int delta_;
	int tempo_, ticks_per_beat_, beats_per_bar_;
	float tick_duration_;
	int bar_, beat_, tick_;
	int hour_, minute_, second_;
	long unsigned int previous_ticks_, now_ticks_;

public:
	Clock ();
	~Clock ();
	void start ();
	void stop ();
	void rewind ();
	unsigned int update ();
	bool getState () { return state_; }
	inline bool isStarted () { return state_; }
	int getHour () { return hour_; }
	int getMinute () { return minute_; }
	int getSecond () { return second_; }
	int getBar () { return bar_; }
	int getBeat () { return beat_; }
	int getTick () { return tick_; }

};

#endif
