#include "SoundWrapper.h"

#include <vorbis/vorbisfile.h>
#include <ogg/ogg.h>

#include <iostream>

ALCdevice* Sound::device = nullptr;
ALCcontext* Sound::context = nullptr;
unsigned int Sound::bufferAmount = 0;
unsigned int Sound::frozenCategories = 0;
ALuint Sound::sources[SOURCECOUNT];
Sound* Sound::playingSounds[SOURCECOUNT];
bool Sound::pauseState[SOURCECOUNT];

std::vector<Sound*> Sound::loadedSounds;
irr::core::vector3df Sound::listenerPos;
irr::core::vector3df Sound::listenerUp;
irr::core::vector3df Sound::listenerTarget;

bool Sound::initSounds() {
	if (Sound::device==nullptr && Sound::context==nullptr) {
		Sound::device = alcOpenDevice(nullptr);
		if (!device) {
			std::cout<<"No available Sound device; Try installing OpenAL if you haven't already\n";
			return false;
		}
		Sound::context = alcCreateContext(device,nullptr);
		if (!context) {
			alcCloseDevice(Sound::device);
			Sound::device = nullptr;
			std::cout<<"Failed to create Sound context\n";
			return false;
		}
		alcMakeContextCurrent(context);

		for (unsigned char i=0;i<SOURCECOUNT;i++) {
			alGenSources(1,&Sound::sources[i]);
			alSourceStop(Sound::sources[i]);
			ALenum error = alGetError();
			if (error != AL_NO_ERROR) {
				alcDestroyContext(Sound::context);
				Sound::context = nullptr;
				alcMakeContextCurrent(nullptr);
				alcCloseDevice(Sound::device);
				Sound::device = nullptr;
				std::cout<<"Error generating source "<<i<<"\n";
				return false;
			}
			Sound::playingSounds[i] = nullptr;
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

void Sound::killSounds() {

	for (unsigned int i=0;i<Sound::loadedSounds.size();i++) {
		alDeleteBuffers(1,&Sound::loadedSounds[i]->buffer);
		delete Sound::loadedSounds[i];
	}

	for (unsigned int i=0;i<SOURCECOUNT;i++) {
		alDeleteSources(1,&Sound::sources[i]);
	}

	alcMakeContextCurrent(nullptr);
	if (Sound::context!=nullptr) {
		alcDestroyContext(Sound::context);
		Sound::context=nullptr;
	}
	if (Sound::device!=nullptr) {
		alcCloseDevice(Sound::device);
		Sound::device=nullptr;
	}
}

bool Sound::isActive() {
	if (Sound::device==nullptr || Sound::context==nullptr) {
		std::cout<<"OpenAL has not been initialized, cannot execute Sound functions\n";
	}
	return !(Sound::device==nullptr || Sound::context==nullptr);
}

bool Sound::loadOGG(const std::string &filename,std::vector<char> &buffer,ALenum &format,ALsizei &freq,bool isPanned) {
	if (!Sound::isActive()) return false;
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

Sound::Sound() {}
Sound::~Sound() {}

Sound* Sound::getSound(const std::string &filename,bool isPanned,unsigned char categ) {
	if (!Sound::isActive()) return nullptr;
	for (unsigned int i=0;i<Sound::loadedSounds.size();i++) {
		if (Sound::loadedSounds[i]->name == filename && isPanned==Sound::loadedSounds[i]->panned) {
			std::cout<<"\""<<filename<<"\" is already in memory\n";
			Sound::loadedSounds[i]->grab();
			return Sound::loadedSounds[i];
		}
	}

	std::vector<char> bufData; ALenum format; ALsizei freq;
	if (loadOGG(filename,bufData,format,freq,isPanned)) {
		Sound* newSound = new Sound;

		newSound->grabs = 0;
		newSound->grab();
		newSound->category = categ;

		newSound->panned = isPanned;

		alGenBuffers(1,&newSound->buffer);

		alBufferData(newSound->buffer,format,&bufData[0],static_cast<ALsizei>(bufData.size()),freq);

		newSound->name = filename;

		Sound::loadedSounds.push_back(newSound);

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
		std::cout<<"Could not load Sound \""<<filename<<"\"";
		return nullptr;
	}
}

void Sound::drop() {
	if (grabs>0) grabs--;
}

void Sound::grab() {
	grabs++;
	//excessive grabs generally mean there's something wrong
	if (grabs>20) std::cout<<"Warning: "<<name<<" has been grabbed more than 20 times\n";
}

unsigned char Sound::playSound(const irr::core::vector3df &sourcePos,float near,float far,bool isLooping,float gain) {
	if (!Sound::isActive()) return 0;
	if (this==nullptr) return 0;
	if (Sound::frozenCategories & (1<<category)) return 0;
	ALuint selectedSound = 0;
	bool foundCandidate = false;
	unsigned char i;
	for (i=0;i<SOURCECOUNT;i++) {
		ALint state;

		alGetSourcei(Sound::sources[i], AL_SOURCE_STATE, &state);

		if (state != AL_PLAYING && state != AL_PAUSED) {
			selectedSound = Sound::sources[i]; foundCandidate = true;
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
		alSourcef(sources[i],AL_GAIN,gain);
		alSourceRewind(selectedSound);
		alSourcePlay(selectedSound);
		Sound::playingSounds[i] = this;
		Sound::pauseState[i] = false;
		return i;
	} else {
		std::cout<<"Can't play new Sound, all sources are being used\n";
		return 0;
	}
}

unsigned char Sound::playSound(bool isLooping,float gain) {
	if (!Sound::isActive()) return 0;
	if (this==nullptr) return 0;
	if (Sound::frozenCategories & (1<<category)) return 0;
	ALuint selectedSound = 0;
	bool foundCandidate = false;
	unsigned char i;
	for (i=0;i<SOURCECOUNT;i++) {
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
		alSourcef(sources[i],AL_GAIN,gain);
		alSourceRewind(selectedSound);
		alSourcePlay(selectedSound);
		Sound::playingSounds[i] = this;
		Sound::pauseState[i] = false;
		return i;
	} else {
		std::cout<<"Can't play new Sound, all sources are being used\n";
		return 0;
	}
}

void Sound::pauseSound(unsigned char sourceNum) {
	if (Sound::frozenCategories & (1<<category)) return;
	sourceNum%=SOURCECOUNT;
	if (Sound::playingSounds[sourceNum]==this) {
		Sound::pauseState[sourceNum]=true;
		alSourcePause(Sound::sources[sourceNum]);
	}
}

void Sound::pauseSound() {
	if (Sound::frozenCategories & (1<<category)) return;
	for (unsigned char i=0;i<SOURCECOUNT;i++) {
		if (Sound::playingSounds[i]==this) pauseSound(i);
	}
}

void Sound::freezeCategory(unsigned char categ) {
	if (categ>31) return;
	if (!(Sound::frozenCategories & (1<<categ))) {
		Sound::frozenCategories = (Sound::frozenCategories | (1<<categ));
		for (unsigned int i=0;i<SOURCECOUNT;i++) {
			if (Sound::playingSounds[i]!=nullptr) {
				if (Sound::playingSounds[i]->category==categ) {
					ALint state;

					alGetSourcei(Sound::sources[i], AL_SOURCE_STATE, &state);

					if (state == AL_PLAYING) {
						alSourcePause(Sound::sources[i]);
					}
				}
			}
		}
	}
}

void Sound::unfreezeCategory(unsigned char categ) {
	if (categ>31) return;
	if (Sound::frozenCategories & (1<<categ)) {
		Sound::frozenCategories = (Sound::frozenCategories ^ (1<<categ));
		for (unsigned int i=0;i<SOURCECOUNT;i++) {
			if (Sound::playingSounds[i]!=nullptr) {
				if (Sound::playingSounds[i]->category==categ) {
					ALint state;

					alGetSourcei(Sound::sources[i], AL_SOURCE_STATE, &state);

					if (state == AL_PAUSED && Sound::pauseState[i] == false) {
						alSourcePlay(Sound::sources[i]);
					}
				}
			}
		}
	}
}

void Sound::stopSound(unsigned char sourceNum) {
	if (Sound::frozenCategories & (1<<category)) return;
	sourceNum%=SOURCECOUNT;
	if (Sound::playingSounds[sourceNum]==this) {
		Sound::pauseState[sourceNum]=false;
		alSourceStop(Sound::sources[sourceNum]);
		alSourceRewind(Sound::sources[sourceNum]);
	}
}

void Sound::stopSound() {
	if (Sound::frozenCategories & (1<<category)) return;
	for (unsigned char i=0;i<SOURCECOUNT;i++) {
		stopSound(i);
	}
}

void Sound::resumeSound(unsigned char sourceNum) {
	if (Sound::frozenCategories & (1<<category)) return;
	sourceNum%=SOURCECOUNT;
	if (Sound::playingSounds[sourceNum]==this) {
		ALint state;
		alGetSourcei(Sound::sources[sourceNum],AL_SOURCE_STATE,&state);
		if (state==AL_PAUSED) {
			alSourcePlay(Sound::sources[sourceNum]);
			Sound::pauseState[sourceNum]=false;
		}
	}

}

void Sound::resumeSound() {
	if (Sound::frozenCategories & (1<<category)) return;
	for (unsigned char i=0;i<SOURCECOUNT;i++) {
		if (Sound::playingSounds[i]==this) resumeSound(i);
	}
}

void Sound::moveSource(unsigned char sourceNum,const irr::core::vector3df& sourcePos) {
    if (Sound::frozenCategories & (1<<category)) return;
    sourceNum%=SOURCECOUNT;
    if (Sound::playingSounds[sourceNum]==this) {
        alSource3f(Sound::sources[sourceNum],AL_POSITION,sourcePos.X,sourcePos.Y,sourcePos.Z);
    }
}

bool Sound::isPlaying() {
	if (!Sound::isActive()) return false;
	if (this == nullptr) return false;
	if (Sound::frozenCategories & (1<<category)) return false;

	for (unsigned char i=0;i<SOURCECOUNT;i++) {
		if (Sound::playingSounds[i]==this) {
			ALint state;

			alGetSourcei(Sound::sources[i], AL_SOURCE_STATE, &state);

			if (state == AL_PLAYING) return true;
		}
	}
	return false;
}

void Sound::setListenerPos(const irr::core::vector3df &newPos) {
	if (!Sound::isActive()) return;
	Sound::listenerPos = newPos;
	alListener3f(AL_POSITION,newPos.X,newPos.Y,newPos.Z);
}

void Sound::setListenerOrientation(const irr::core::vector3df &up,const irr::core::vector3df &target) {
	if (!Sound::isActive()) return;
	listenerUp = up;
	listenerTarget = target;
	float orientation[]={-listenerTarget.X,listenerTarget.Y,-listenerTarget.Z,listenerUp.X,listenerUp.Y,listenerUp.Z};
	alListenerfv(AL_ORIENTATION,orientation);
}

irr::core::vector3df Sound::getListenerPos() {
	if (!Sound::isActive()) return irr::core::vector3df(0,0,0);
	return Sound::listenerPos;
}

void Sound::processDrops() {
	for (unsigned int i=0;i<Sound::loadedSounds.size();i++) {
		if (Sound::loadedSounds[i]->grabs<=0) {
			for (unsigned char j=0;j<SOURCECOUNT;j++) {
				if (Sound::playingSounds[i]==Sound::loadedSounds[i]) {
					alSourceStop(Sound::sources[j]);
					Sound::playingSounds[j] = nullptr;
				}
			}
			alDeleteBuffers(1,&Sound::loadedSounds[i]->buffer);
			delete Sound::loadedSounds[i];
			Sound::loadedSounds.erase(Sound::loadedSounds.begin()+i);
			i--;
			std::terminate();
		}
	}
}
