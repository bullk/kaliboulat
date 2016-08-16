#ifndef INC_SCHEDULED_H
#define INC_SCHEDULED_H


class Scheduled
{
public:
	Scheduled (unsigned long);
	~Scheduled ();
	unsigned long getTime ();
	void setTime (unsigned long);
	

protected:
	unsigned long time_;

};


#endif
