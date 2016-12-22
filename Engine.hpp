#ifndef INC_ENGINE_H
#define INC_ENGINE_H

#include <iostream> // cout, endl
#include <queue>


enum DataType { AUDIO, MIDI, OSC, COM };


class RessourceFile
{
public:
	RessourceFile (DataType dt, std::string path);
	virtual ~RessourceFile ();
	inline DataType dataType () { return data_type_; }
	inline std::string getPath () { return path_; }
	inline std::string getName () { return name_; }

protected:
	DataType data_type_;
	std::string path_, name_;
};


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
		std::queue<BaseCommand *> bar_, beat_, tick_, main_;
		static Waiter * singleton_;
		
	public:
		void bar ();
		void beat ();
		void tick ();
		void main ();
		inline void addBar (BaseCommand * command) { bar_.push (command); }
		inline void addBeat (BaseCommand * command) { beat_.push (command); }
		inline void addTick (BaseCommand * command) { tick_.push (command); }
		inline void addMain (BaseCommand * command) { main_.push (command); }
		void newProject (std::string name);
		void saveProject ();
		
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
