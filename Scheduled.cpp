#include "Scheduled.hpp"

Scheduled::Scheduled (long unsigned int t) { time_ = t; }

Scheduled::~Scheduled () {}

long unsigned int Scheduled::getTime () { return time_; }

void Scheduled::setTime (long unsigned int t) { time_ = t; }
