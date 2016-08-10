#ifndef INC_SCHEDULED_H
#define INC_SCHEDULED_H


class Scheduled
{
public:
	Scheduled (long unsigned int);
	~Scheduled ();
	long unsigned int getTime ();
	void setTime (long unsigned int);
	

protected:
	long unsigned int time_;

};


#endif
