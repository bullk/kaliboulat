#ifndef INC_SLBus_H
#define INC_SLBus_H

//#include <iostream> // cout, endl
#include <vector>
#include "SLClip.hpp"
#include "Track.hpp"

#include <cereal/types/vector.hpp>
//#include <cereal/archives/binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/types/polymorphic.hpp>

#include <typeinfo>  //for 'typeid' to work  

class SLClip;

class SLBus : public Track
{
	
public:
	// Constructor 
	SLBus( std::string );
	SLBus( std::string, float, float, std::vector<std::shared_ptr<SLClip>> );
	// Destructor 
	~SLBus();

	void addClip( std::string, int );
	void addClip( std::shared_ptr<Clip> clip );
	void addClip( std::shared_ptr<SLClip> clip );
	void deleteClip( unsigned int i );
	inline std::shared_ptr<Clip> getClip( unsigned int i ) const { return clipset_[i]; }
	inline unsigned int nClips() const { return clipset_.size(); }
	void stopAll();
	inline float * getVolume() { return &volume_; }
	
	template <class Archive>
	void serialize(Archive & archive)
	{
		archive (
			CEREAL_NVP( name_ ),
			CEREAL_NVP( hue_ ),
			CEREAL_NVP( volume_ ),
			CEREAL_NVP( clipset_ )
		);
	}
	
	template <class Archive>
	static void load_and_construct( Archive & archive, cereal::construct<SLBus> & construct )
	{
		std::string name;
		float hue, volume;
		std::vector<std::shared_ptr<SLClip>> clipset;
		archive( name, hue, volume, clipset );
		construct( name, hue, volume, clipset );
	}

protected:
	float volume_;
	unsigned int sl_port_;
	pid_t sl_pid_;
	std::vector<std::shared_ptr<SLClip>> clipset_;
	void startSL();


};

CEREAL_REGISTER_TYPE( SLBus )
CEREAL_REGISTER_POLYMORPHIC_RELATION( Track, SLBus )

#endif
