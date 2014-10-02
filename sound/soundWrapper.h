#ifndef SOUNDWRAPPER_H_INCLUDED
#define SOUNDWRAPPER_H_INCLUDED

#include <irrlicht.h>
#include <string>
#include <vector>

#include <AL/al.h>
#include <AL/alc.h>

const unsigned char sourceCount = 28;

class sound {
	private:
		static unsigned int bufferAmount;

		static irr::core::vector3df listenerPos;
		static irr::core::vector3df listenerUp;
		static irr::core::vector3df listenerTarget;

		static std::vector<sound*> loadedSounds;

		static ALCdevice* device;
		static ALCcontext* context;

		static ALuint sources[sourceCount];
		static sound* playingSounds[sourceCount];
		static bool pauseState[sourceCount];

		static unsigned int frozenCategories;

		static bool loadOGG(const std::string &filename,std::vector<char> &buffer,ALenum &format,ALsizei &freq,bool isPanned=true);

		static bool isActive();
		//------------------------------------------
		bool panned,looped;

		unsigned int grabs = 0;

		unsigned char category = 0;

		std::string name;

		ALuint buffer;
		//bool paused[sourceCount];
		//unsigned char lastSource = 0;

		sound();
		~sound();
	public:
		static bool initSounds(); //initialize OpenAl
		static void killSounds(); //Destroy context, device and sounds

		static sound* getSound(const std::string &filename,bool isPanned=true); //load sound if it's not loaded

		void drop(); //when you don't use the sound, drop it
		void grab(); //grab the sound when you want to keep it after it's been dropped

		bool isLoop() { return looped; } //is the sound looping?
		bool isPanned() { return panned; } //can the sound use OpenAL's automatic attenuation and panning?

		unsigned char playSound(bool isLooping=false); //start playing sound with no distance from the listener
		unsigned char playSound(const irr::core::vector3df &sourcePos,float near,float far,bool isLooping=false); //start playing sound at a specific position and attenuation

		void pauseSound(unsigned char sourceNum); //pause the specified source
		void pauseSound(); //pause all sources used by the sound

		static void freezeCategory(unsigned char categ); //freeze a category of sounds
		static void unfreezeCategory(unsigned char categ); //unfreeze a category of sounds, only sounds that were playing will be resumed

		void stopSound(unsigned char sourceNum); //stop the specified source
		void stopSound(); //stop all sources used by the sound

		void resumeSound(unsigned char sourceNum); //resume the specified source if it's paused
		void resumeSound(); //resume all paused sources used by the sound

		void moveSource(unsigned char sourceNum,const irr::core::vector3df &sourcePos);

		bool isPlaying(); //is the sound playing?

		static void setListenerPos(const irr::core::vector3df &newPos); //set the listener's position
		static void setListenerOrientation(const irr::core::vector3df &up,const irr::core::vector3df &target); //set the listener's orientation
		static irr::core::vector3df getListenerPos(); //get the listener's position
		static irr::core::vector3df getListenerUp(); //get the listener's up vector
		static irr::core::vector3df getListenerTarget(); //get the listener's target vector

		static void processDrops();
};

#endif // SOUNDWRAPPER_H_INCLUDED
