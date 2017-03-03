#ifndef INC_MYCLOCK_H
#define INC_MYCLOCK_H

#include <chrono>


// Clock

class Clock
{

private:
	bool state_;
	std::chrono::time_point<std::chrono::system_clock> startTime_;
	std::chrono::time_point<std::chrono::system_clock> pauseTime_;
	std::chrono::time_point<std::chrono::system_clock> now_;
	std::chrono::time_point<std::chrono::system_clock> previousTime_;
	long long unsigned int delta_, elapsedTime_;
	int tempo_;
	unsigned int ticks_per_beat_, beats_per_bar_;
	float tick_duration_;
	unsigned int bar_, beat_, tick_;
	unsigned int hour_, minute_, second_;
	long unsigned int previous_ticks_, now_ticks_, elapsed_ticks;

public:
	Clock();
	~Clock();
	void metrics();
	void start();
	void pause();
	void stop();
	void rewind();
	unsigned int update();
	inline bool getState() { return state_; }
	inline bool atZero() { return ( delta_ == 0 ); }
	inline bool isStarted() { return state_; }
	inline int * getTempoP() { return &tempo_; }
	inline unsigned int getHour() { return hour_; }
	inline unsigned int getMinute() { return minute_; }
	inline unsigned int getSecond() { return second_; }
	inline unsigned int getBar() { return bar_; }
	inline unsigned int getBeat() { return beat_; }
	inline unsigned int getTick() { return tick_; }
	inline unsigned int getTicksPerBeat() { return ticks_per_beat_; }
	inline void setTicksPerBeat( unsigned int tpb ) { ticks_per_beat_ = tpb; }

};

#endif
