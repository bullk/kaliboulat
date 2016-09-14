#ifndef INC_ENGINE_H
#define INC_ENGINE_H

#include <iostream> // cout, endl
#include <queue>

enum DataType { AUDIO, MIDI, OSC, COM };

class BaseCommand
{
	public:
		virtual void execute() = 0;
};

template <typename O, typename M>
class Command : public BaseCommand
{
	public:
	Command (O object, M method)
	{
		object_ = object;
		method_ = method;
	}
	void execute ()	{ (object_->*method_)(); }
	protected:
		O object_;
		M method_;
};


class Waiter
{
	private:
		// Constructor
		Waiter ();
		// Destructor
		~Waiter ();
		
	public:
		void bar ();
		void beat ();
		void tick ();
		void main ();
		inline void addBar (BaseCommand * command) { bar_.push (command); }
		inline void addBeat (BaseCommand * command) { beat_.push (command); }
		inline void addTick (BaseCommand * command) { tick_.push (command); }
		inline void addMain (BaseCommand * command) { main_.push (command); }
		
		// Singleton
		static Waiter *getInstance ()
		{
			if ( NULL == _singleton )
				{
					std::cout << "creating Waiter singleton." << std::endl;
					_singleton =  new Waiter;
				}

			return _singleton;
		}
		static void kill ()
		{
			if ( NULL != _singleton )
				{
					delete _singleton;
					_singleton = NULL;
				}
		}
	
	private:
		std::queue<BaseCommand *> bar_, beat_, tick_, main_;
		static Waiter * _singleton;
		
	};


#endif


