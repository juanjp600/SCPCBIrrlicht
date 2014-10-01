#ifndef SOUNDWRAPPER_H_INCLUDED
#define SOUNDWRAPPER_H_INCLUDED

#include <irrlicht.h>
#include <string>
#include <vector>

#include <AL/al.h>
#include <AL/alc.h>

const unsigned int sourceCount = 10;

class sound {
	private:
		bool panned,looped;

		static irr::core::vector3df listenerPos;
		static irr::core::vector3df listenerUp;
		static irr::core::vector3df listenerTarget;

		static std::vector<sound*> loadedSounds;

		static ALCdevice* device;
		static ALCcontext* context;

		static bool loadOGG(const std::string &filename,std::string &buffer,ALenum &format,ALsizei &freq,bool isPanned=true);

		std::string name;

		static bool isActive();

		ALuint buffer;
		ALuint source[sourceCount];
		unsigned char lastSource = 0;

		sound();
		~sound();
	public:
		static bool initSounds();

		static sound* getSound(const std::string &filename,bool isPanned=true);
		void loopSound(bool newState);
		bool isLoop() { return looped; }
		bool isPanned() { return panned; }
		void playSound(bool isLooping=false); //sound with no distance from the listener
		void playSound(const irr::core::vector3df &sourcePos,float near,float far,bool isLooping=false); //sound at a specific position and attenuation
		bool isPlaying();
		static void setListenerPos(const irr::core::vector3df &newPos);
		static void setListenerOrientation(const irr::core::vector3df &up,const irr::core::vector3df &target);
		static irr::core::vector3df getListenerPos();
		static irr::core::vector3df getListenerUp();
		static irr::core::vector3df getListenerTarget();
};

#endif // SOUNDWRAPPER_H_INCLUDED
