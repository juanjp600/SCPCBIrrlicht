#include "soundWrapper.h"

#include <vorbis/vorbisfile.h>
#include <ogg/ogg.h>

#include <iostream>

ALCdevice* sound::device = nullptr;
ALCcontext* sound::context = nullptr;

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
			std::cout<<"Failed to create sound context\n";
			return false;
		}
		alcMakeContextCurrent(context);

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

bool sound::isActive() {
	if (sound::device==nullptr || sound::context==nullptr) {
		std::cout<<"OpenAL has not been initialized, cannot execute sound functions\n";
	}
	return !(sound::device==nullptr || sound::context==nullptr);
}

bool sound::loadOGG(const std::string &filename,std::string &buffer,ALenum &format,ALsizei &freq,bool isPanned) {
	if (!isActive()) return false;
	//TODO: write a better version of this
    buffer = "";
    int endian = 0;
    int bitStream;
    long bytes;
    char arry[32768];
    FILE *f;
    f=fopen(filename.c_str(),"rb");
    if (f==NULL) {
        std::cout<<"\""<<filename<<"\" failed to load.\n";
        return false;
    }
    std::cout << "Loading \"" << filename << "\"\n";
    vorbis_info *pInfo;
    OggVorbis_File oggfile;
    ov_open(f,&oggfile,nullptr,0);
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
	if (!isActive()) return nullptr;
	for (unsigned int i=0;i<sound::loadedSounds.size();i++) {
		if (sound::loadedSounds[i]->name == filename && isPanned==sound::loadedSounds[i]->panned) {
			std::cout<<"\""<<filename<<"\" is already in memory\n";
			return sound::loadedSounds[i];
		}
	}

	std::string bufData; ALenum format; ALsizei freq;
	if (loadOGG(filename,bufData,format,freq,isPanned)) {
		sound* newSound = new sound;

		newSound->panned = isPanned;

		alGenBuffers(1,&newSound->buffer);
		alGenSources(sourceCount,newSound->source);
		newSound->lastSource = 0;

		alBufferData(newSound->buffer,format,&bufData[0],static_cast<ALsizei>(bufData.size()),freq);
		for (unsigned char i=0;i<sourceCount;i++) {
			alSourcei(newSound->source[i], AL_BUFFER, newSound->buffer);
			alSource3f(newSound->source[i],AL_POSITION,0.0f,0.0f,0.0f);
		}

		newSound->name = filename;

		sound::loadedSounds.push_back(newSound);

		return newSound;

	} else {
		std::cout<<"Could not load sound \""<<filename<<"\"";
		return nullptr;
	}
}

void sound::playSound(const irr::core::vector3df &sourcePos,float near,float far,bool isLooping) {
	if (!isActive()) return;
	if (this==nullptr) return;
	alSourcef(source[lastSource], AL_REFERENCE_DISTANCE, near);
	alSourcef(source[lastSource], AL_MAX_DISTANCE, far);
	alSource3f(source[lastSource],AL_POSITION,sourcePos.X,sourcePos.Y,sourcePos.Z);
	alSourcei(source[lastSource],AL_LOOPING,isLooping);
	alSourcePlay(source[lastSource]);
    lastSource=(lastSource+1)%sourceCount;
}

void sound::playSound(bool isLooping) {
	sound::playSound(listenerPos,5.f,10.f,isLooping);
}

bool sound::isPlaying() {
	if (!isActive()) return false;
	for (unsigned int i=0;i<sourceCount;i++) {
		ALenum state;

		alGetSourcei(source[i], AL_SOURCE_STATE, &state);

		if (state == AL_PLAYING) return true;
	}
	return false;
}

void sound::setListenerPos(const irr::core::vector3df &newPos) {
	if (!isActive()) return;
	sound::listenerPos = newPos;
	alListener3f(AL_POSITION,newPos.X,newPos.Y,newPos.Z);
}

void sound::setListenerOrientation(const irr::core::vector3df &up,const irr::core::vector3df &target) {
	if (!isActive()) return;
	listenerUp = up;
	listenerTarget = target;
	float orientation[]={-listenerTarget.X,listenerTarget.Y,-listenerTarget.Z,listenerUp.X,listenerUp.Y,listenerUp.Z};
	alListenerfv(AL_ORIENTATION,orientation);
}

irr::core::vector3df sound::getListenerPos() {
	if (!isActive()) return irr::core::vector3df(0,0,0);
	return sound::listenerPos;
}
