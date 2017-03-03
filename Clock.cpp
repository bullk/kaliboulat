#include "Clock.hpp"
#include "Engine.hpp"


Clock::Clock ()
{
	state_ = false;
	tempo_ = 120;
	ticks_per_beat_ = 960;
	beats_per_bar_ = 4;
	metrics();
	rewind();
}

Clock::~Clock ()
{}

void Clock::metrics()
{
	tick_duration_ = 60.0f / (tempo_ * ticks_per_beat_);
}

void Clock::start()
{
	if ( not(state_) )
	{
		startTime_ = startTime_ + (std::chrono::system_clock::now() - pauseTime_);
		pauseTime_ = startTime_;
		state_ = true;
	}
}

void Clock::pause()
{
	if (state_)
	{
		pauseTime_ = std::chrono::system_clock::now();
		state_ = false;
	}
}

void Clock::stop ()
{
	state_ = false;
	rewind ();
}
	
void Clock::rewind ()
{
	bar_ = 0;
	beat_ = 0;
	tick_ = 0;
	hour_ = 0;
	minute_ = 0;
	second_ = 0;
	previous_ticks_ = 0;
	now_ticks_ = 0;
	delta_ = 0;
}
	
unsigned int Clock::update ()
{
	unsigned int midi_ticks = 0;
	if (state_) {
		
		unsigned int previous_bar = bar_;
		unsigned int previous_beat = beat_;
		
		now_ = std::chrono::system_clock::now();
		delta_ = std::chrono::duration_cast<std::chrono::microseconds> (now_ - startTime_).count();
		// h:m:s
		int tot_s = delta_ / 1000000;
		int tot_m = tot_s / 60;
		hour_ = tot_m / 60;
		minute_ = tot_m % 60;
		second_ = tot_s % 60;

		// MIDI (bar_, beat_, tick_)
		now_ticks_ = (long unsigned int) (delta_ / (1000000 * tick_duration_));
		unsigned int nbeats = now_ticks_ / ticks_per_beat_;
		tick_ = now_ticks_ % ticks_per_beat_;
		beat_ = 1 + nbeats % beats_per_bar_;
		if ( previous_beat != beat_ ) Waiter::getInstance () -> beat ();
		bar_ = 1 + nbeats / beats_per_bar_;
		if ( previous_bar != bar_ ) Waiter::getInstance () -> bar ();

		// how many MIDI ticks ?
		if (now_ticks_ < previous_ticks_)
			previous_ticks_ = now_ticks_;
		unsigned int ticks_delta = now_ticks_ - previous_ticks_;
		if ( (previous_ticks_ == 0) and (ticks_delta == 0) ) midi_ticks = 1;
		if ( ticks_delta > 0 ) midi_ticks = ticks_delta;

		// MIDI Clock update
		previous_ticks_ = now_ticks_;

		// DEBUG
		//char bbt[13];
		//sprintf (bbt, "%02d:%02d:%03d   ", daClock -> getBar(), daClock -> getBeat(), diApp.clock -> getTick());
		//std::cout << bbt << midi_ticks << std::endl;
	}
	return midi_ticks;
}

