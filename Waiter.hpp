#ifndef INC_WAITER_H
#define INC_WAITER_H

#include <iostream> // cout, endl
#include <queue>
#include <memory>

class Clip;

class BaseCommand
{
	public:
		virtual void execute() = 0;
		virtual bool isFrom( void * ) =0;
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
	bool isFrom ( void * o ) { return ( o == object_ ); }
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
		std::queue<BaseCommand *> bar_, beat_, tick_, main_;
		static Waiter * singleton_;
		
	public:
		void panic ();
		void start ();
		void pause ();
		void stop ();
		void bar ();
		void beat ();
		void tick ();
		void main ();
		inline void addBar (BaseCommand * command) { bar_.push (command); }
		inline void addBeat (BaseCommand * command) { beat_.push (command); }
		inline void addTick (BaseCommand * command) { tick_.push (command); }
		inline void addMain (BaseCommand * command) { main_.push (command); }
		void newProject (std::string);
		void loadProject (std::string);
		void saveProject ();
		void importAudioFile (std::string);
		void importMidiFile (std::string);
		void selectClip( std::shared_ptr<Clip> );
		void deleteClip( std::shared_ptr<Clip> );
		//void closeProject ();
		
		// Singleton
		static Waiter *getInstance ()
		{
			if ( NULL == singleton_ )
				singleton_ =  new Waiter;

			return singleton_;
		}

		static void kill ()
		{
			if ( NULL != singleton_ )
				{
					delete singleton_;
					singleton_ = NULL;
				}
		}
	
};

#endif
