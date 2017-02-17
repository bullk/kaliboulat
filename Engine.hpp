#ifndef INC_ENGINE_H
#define INC_ENGINE_H

#include <iostream> // cout, endl
#include <queue>
#include <memory>

class Clip;

inline std::string user_dir()
{
	return std::string(getenv ("HOME")) + "/Documents/Kaliboulat";
}

std::string name_from_path (std::string);


enum DataType { NONE, AUDIO, MIDI, OSC, COM };


class RessourceFile
{
public:
	RessourceFile (DataType dt);
	RessourceFile (DataType dt, std::string path);
	virtual ~RessourceFile ();
	inline DataType dataType () { return data_type_; }
	inline std::string getPath () { return path_; }
	void setPath (std::string path);
	inline std::string getName () { return name_; }
	inline void setName (std::string name) { name_ = name; }
	
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
		void selectClip (std::shared_ptr<Clip>);
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
