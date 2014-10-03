#include "soundWrapper.h"

#include <vorbis/vorbisfile.h>
#include <ogg/ogg.h>

#include <iostream>

ALCdevice* sound::device = nullptr;
ALCcontext* sound::context = nullptr;
unsigned int sound::bufferAmount = 0;
unsigned int sound::frozenCategories = 0;
ALuint sound::sources[sourceCount];
sound* sound::playingSounds[sourceCount];
bool sound::pauseState[sourceCount];

std::vector<sound*> sound::loadedSounds;
irr::core::vector3df sound::listenerPos;
irr::core::vector3df sound::listenerUp;
irr::core::vector3df sound::listenerTarget;

bool sound::initSounds() {
	if (sound::device==nullptr && sound::context==nullptr) {
		sound::device = alcOpenDevice(nullptr);
		if (!device) {
			std::cout<<"No available sound device; Try installing OpenAL if you haven't already\n";
			return false;
		}
		sound::context = alcCreateContext(device,nullptr);
		if (!context) {
			alcCloseDevice(sound::device);
			sound::device = nullptr;
			std::cout<<"Failed to create sound context\n";
			return false;
		}
		alcMakeContextCurrent(context);

		for (unsigned char i=0;i<sourceCount;i++) {
			alGenSources(1,&sound::sources[i]);
			alSourceStop(sound::sources[i]);
			ALenum error = alGetError();
			if (error != AL_NO_ERROR) {
				alcDestroyContext(sound::context);
				sound::context = nullptr;
				alcMakeContextCurrent(nullptr);
				alcCloseDevice(sound::device);
				sound::device = nullptr;
				std::cout<<"Error generating source "<<i<<"\n";
				return false;
			}
		}

		alListener3f(AL_POSITION,0.0f,0.0f,0.0f);
		listenerPos = irr::core::vector3df(0.f,0.f,0.f);
		listenerUp = irr::core::vector3df(0.f,1.f,0.f);
		listenerTarget = irr::core::vector3df(0.f,0.f,1.f);
		float orientation[]={listenerTarget.X,listenerTarget.Y,listenerTarget.Z,listenerUp.X,listenerUp.Y,listenerUp.Z};
		alListenerfv(AL_ORIENTATION,orientation);
		alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED);

		std::cout << "OpenAL initialized successfully\n";
		return true;
	} else {
		std::cout<<"OpenAL is already initialized\n";
		return true;
	}
}

void sound::killSounds() {

	for (unsigned int i=0;i<sound::loadedSounds.size();i++) {
		alDeleteBuffers(1,&sound::loadedSounds[i]->buffer);
		delete sound::loadedSounds[i];
	}

	for (unsigned int i=0;i<sourceCount;i++) {
		alDeleteSources(1,&sound::sources[i]);
	}

	alcMakeContextCurrent(nullptr);
	if (sound::context!=nullptr) {
		alcDestroyContext(sound::context);
		sound::context=nullptr;
	}
	if (sound::device!=nullptr) {
		alcCloseDevice(sound::device);
		sound::device=nullptr;
	}
}

bool sound::isActive() {
	if (sound::device==nullptr || sound::context==nullptr) {
		std::cout<<"OpenAL has not been initialized, cannot execute sound functions\n";
	}
	return !(sound::device==nullptr || sound::context==nullptr);
}

bool sound::loadOGG(const std::string &filename,std::vector<char> &buffer,ALenum &format,ALsizei &freq,bool isPanned) {
	if (!sound::isActive()) return false;
	//TODO: write a better version of this
    buffer.resize(0);
    int endian = 0;
    int bitStream;
    long bytes;
    char arry[32768];
    FILE *f;
    f=fopen(filename.c_str(),"rb");
    if (f==nullptr) {
        std::cout<<"\""<<filename<<"\" failed to load.\n";
        return false;
    }
    std::cout << "Loading \"" << filename << "\"\n";
    vorbis_info *pInfo;
    OggVorbis_File oggfile;
    ov_open(f,&oggfile,"",0);
    pInfo = ov_info(&oggfile,-1);
    if (pInfo->channels == 1) {
        format = AL_FORMAT_MONO16;
    } else {
        format = AL_FORMAT_STEREO16;
    }
    freq = pInfo->rate;
    if (isPanned && format==AL_FORMAT_STEREO16) {
		//OpenAL does not perform automatic panning or attenuation with stereo tracks
		format = AL_FORMAT_MONO16;
		freq*=2;
    }
    do {
        bytes = ov_read(&oggfile,arry,32768,endian,2,1,&bitStream);
        buffer.insert(buffer.end(),arry,arry+bytes);
    } while (bytes>0);

    ov_clear(&oggfile);

    return true;
}

sound::sound() {}
sound::~sound() {}

sound* sound::getSound(const std::string &filename,bool isPanned) {
	if (!sound::isActive()) return nullptr;
	for (unsigned int i=0;i<sound::loadedSounds.size();i++) {
		if (sound::loadedSounds[i]->name == filename && isPanned==sound::loadedSounds[i]->panned) {
			std::cout<<"\""<<filename<<"\" is already in memory\n";
			sound::loadedSounds[i]->grab();
			return sound::loadedSounds[i];
		}
	}

	std::vector<char> bufData; ALenum format; ALsizei freq;
	if (loadOGG(filename,bufData,format,freq,isPanned)) {
		sound* newSound = new sound;

		newSound->grabs = 0;
		newSound->grab();
		newSound->category = 0; //default category

		newSound->panned = isPanned;

		alGenBuffers(1,&newSound->buffer);

		alBufferData(newSound->buffer,format,&bufData[0],static_cast<ALsizei>(bufData.size()),freq);

		newSound->name = filename;

		sound::loadedSounds.push_back(newSound);

		ALenum error = alGetError();
		if (error != AL_NO_ERROR) {
			std::cout<<"Error loading \""<<filename<<"\": ";
			switch (error) {
				case AL_INVALID_NAME:
					std::cout<<"AL_INVALID_NAME\n";
				break;

				case AL_INVALID_ENUM:
					std::cout<<"AL_INVALID_ENUM\n";
				break;

				case AL_INVALID_VALUE:
					std::cout<<"AL_INVALID_VALUE\n";
				break;

				case AL_INVALID_OPERATION:
					std::cout<<"AL_INVALID_OPERATION\n";
				break;

				case AL_OUT_OF_MEMORY:
					std::cout<<"AL_OUT_OF_MEMORY\n";
				break;
			}
		}

		return newSound;

	} else {
		std::cout<<"Could not load sound \""<<filename<<"\"";
		return nullptr;
	}
}

void sound::drop() {
	if (grabs>0) grabs--;
}

void sound::grab() {
	grabs++;
}

unsigned char sound::playSound(const irr::core::vector3df &sourcePos,float near,float far,bool isLooping) {
	if (!sound::isActive()) return 0;
	if (this==nullptr) return 0;
	if (sound::frozenCategories & (1<<category)) return 0;
	ALuint selectedSound = 0;
	bool foundCandidate = false;
	unsigned char i;
	for (i=0;i<sourceCount;i++) {
		ALint state;

		alGetSourcei(sound::sources[i], AL_SOURCE_STATE, &state);

		if (state != AL_PLAYING && state != AL_PAUSED) {
			selectedSound = sound::sources[i]; foundCandidate = true;
			break;
		}
	}
	if (foundCandidate) {
		alSourcei(selectedSound, AL_BUFFER, buffer);
		alSourcei(selectedSound, AL_SOURCE_RELATIVE, AL_FALSE);
		alSourcef(selectedSound, AL_REFERENCE_DISTANCE, near);
		alSourcef(selectedSound, AL_MAX_DISTANCE, far);
		alSource3f(selectedSound,AL_POSITION,sourcePos.X,sourcePos.Y,sourcePos.Z);
		alSourcei(selectedSound,AL_LOOPING,isLooping);
		alSourceRewind(selectedSound);
		alSourcePlay(selectedSound);
		sound::playingSounds[i] = this;
		sound::pauseState[i] = false;
		return i;
	} else {
		std::cout<<"Can't play new sound, all sources are being used\n";
		return 0;
	}
}

unsigned char sound::playSound(bool isLooping) {
	if (!sound::isActive()) return 0;
	if (this==nullptr) return 0;
	if (sound::frozenCategories & (1<<category)) return 0;
	ALuint selectedSound = 0;
	bool foundCandidate = false;
	unsigned char i;
	for (i=0;i<sourceCount;i++) {
		ALint state;

		alGetSourcei(sources[i], AL_SOURCE_STATE, &state);

		if (state != AL_PLAYING && state != AL_PAUSED) {
			selectedSound = sources[i]; foundCandidate = true;
			break;
		}
	}
	if (foundCandidate) {
		alSourcei(selectedSound, AL_BUFFER, buffer);
		alSourcei(selectedSound, AL_SOURCE_RELATIVE, AL_TRUE);
		alSourcef(selectedSound, AL_REFERENCE_DISTANCE, 100.f);
		alSourcef(selectedSound, AL_MAX_DISTANCE, 200.f);
		alSource3f(selectedSound,AL_POSITION,0.f,0.f,0.f);
		alSourcei(selectedSound,AL_LOOPING,isLooping);
		alSourceRewind(selectedSound);
		alSourcePlay(selectedSound);
		sound::playingSounds[i] = this;
		sound::pauseState[i] = false;
		return i;
	} else {
		std::cout<<"Can't play new sound, all sources are being used\n";
		return 0;
	}
}

void sound::pauseSound(unsigned char sourceNum) {
	if (sound::frozenCategories & (1<<category)) return;
	sourceNum%=sourceCount;
	if (sound::playingSounds[sourceNum]==this) {
		sound::pauseState[sourceNum]=true;
		alSourcePause(sound::sources[sourceNum]);
	}
}

void sound::pauseSound() {
	if (sound::frozenCategories & (1<<category)) return;
	for (unsigned char i=0;i<sourceCount;i++) {
		if (sound::playingSounds[i]==this) pauseSound(i);
	}
}

void sound::freezeCategory(unsigned char categ) {
	if (categ>31) return;
	if (!(sound::frozenCategories & (1<<categ))) {
		sound::frozenCategories = (sound::frozenCategories | (1<<categ));
		for (unsigned int i=0;i<sourceCount;i++) {
			if (sound::playingSounds[i]->category==categ) {
				ALint state;

				alGetSourcei(sound::sources[i], AL_SOURCE_STATE, &state);

				if (state == AL_PLAYING) {
					alSourcePause(sound::sources[i]);
				}
			}
		}
	}
}

void sound::unfreezeCategory(unsigned char categ) {
	if (categ>31) return;
	if (sound::frozenCategories & (1<<categ)) {
		sound::frozenCategories = (sound::frozenCategories ^ (1<<categ));
		for (unsigned int i=0;i<sourceCount;i++) {
			if (sound::playingSounds[i]->category==categ) {
				ALint state;

				alGetSourcei(sound::sources[i], AL_SOURCE_STATE, &state);

				if (state == AL_PAUSED && sound::pauseState[i] == false) {
					alSourcePlay(sound::sources[i]);
				}
			}
		}
	}
}

void sound::stopSound(unsigned char sourceNum) {
	if (sound::frozenCategories & (1<<category)) return;
	sourceNum%=sourceCount;
	if (sound::playingSounds[sourceNum]==this) {
		sound::pauseState[sourceNum]=false;
		alSourceStop(sound::sources[sourceNum]);
		alSourceRewind(sound::sources[sourceNum]);
	}
}

void sound::stopSound() {
	if (sound::frozenCategories & (1<<category)) return;
	for (unsigned char i=0;i<sourceCount;i++) {
		stopSound(i);
	}
}

void sound::resumeSound(unsigned char sourceNum) {
	if (sound::frozenCategories & (1<<category)) return;
	sourceNum%=sourceCount;
	if (sound::playingSounds[sourceNum]==this) {
		ALint state;
		alGetSourcei(sound::sources[sourceNum],AL_SOURCE_STATE,&state);
		if (state==AL_PAUSED) {
			alSourcePlay(sound::sources[sourceNum]);
			sound::pauseState[sourceNum]=false;
		}
	}

}

void sound::resumeSound() {
	if (sound::frozenCategories & (1<<category)) return;
	for (unsigned char i=0;i<sourceCount;i++) {
		if (sound::playingSounds[i]==this) resumeSound(i);
	}
}

bool sound::isPlaying() {
	if (!sound::isActive()) return false;
	if (this == nullptr) return false;
	if (sound::frozenCategories & (1<<category)) return false;

	for (unsigned char i=0;i<sourceCount;i++) {
		if (sound::playingSounds[i]==this) {
			ALint state;

			alGetSourcei(sound::sources[i], AL_SOURCE_STATE, &state);

			if (state == AL_PLAYING) return true;
		}
	}
	return false;
}

void sound::setListenerPos(const irr::core::vector3df &newPos) {
	if (!sound::isActive()) return;
	sound::listenerPos = newPos;
	alListener3f(AL_POSITION,newPos.X,newPos.Y,newPos.Z);
}

void sound::setListenerOrientation(const irr::core::vector3df &up,const irr::core::vector3df &target) {
	if (!sound::isActive()) return;
	listenerUp = up;
	listenerTarget = target;
	float orientation[]={-listenerTarget.X,listenerTarget.Y,-listenerTarget.Z,listenerUp.X,listenerUp.Y,listenerUp.Z};
	alListenerfv(AL_ORIENTATION,orientation);
}

irr::core::vector3df sound::getListenerPos() {
	if (!sound::isActive()) return irr::core::vector3df(0,0,0);
	return sound::listenerPos;
}

void sound::processDrops() {
	for (unsigned int i=0;i<sound::loadedSounds.size();i++) {
		if (sound::loadedSounds[i]->grabs<=0) {
			for (unsigned char j=0;j<sourceCount;j++) {
				if (sound::playingSounds[i]==sound::loadedSounds[i]) {
					alSourceStop(sound::sources[j]);
					sound::playingSounds[j] = nullptr;
				}
			}
			alDeleteBuffers(1,&sound::loadedSounds[i]->buffer);
			delete sound::loadedSounds[i];
			sound::loadedSounds.erase(sound::loadedSounds.begin()+i);
			i--;
			std::terminate();
		}
	}
}
