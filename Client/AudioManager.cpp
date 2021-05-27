#include "AudioManager.h"

#include <iostream>
#include <fmod_errors.h>

void AudioManager::init() {
	FMOD_RESULT result;

	result = FMOD::System_Create(&system);

	if (result != FMOD_OK) {
		std::cerr << "Failed to create FMOD system" << std::endl;
	}

	result = system->init(2048, FMOD_INIT_NORMAL, 0);
	if (result != FMOD_OK) {
		std::cerr << "Failed to initialize FMOD system" << std::endl;
	}

	loadSound(SOUND_MEOW);
	loadSound(SOUND_WOOF);
	loadSound(SOUND_STEP);
	loadSound(SOUND_SHOOT);
	loadSound(SOUND_DEATH);
}


void AudioManager::loadSound(std::string filePath) {
	FMOD::Sound* sound;
	FMOD_RESULT result = system->createSound(filePath.c_str(), FMOD_DEFAULT, nullptr, &sound);

	if (result != FMOD_OK) {
		std::cerr << "Failed to load sound file " << filePath << std::endl;
		std::cerr << FMOD_ErrorString(result) << std::endl;
		return;
	}
	
	// save sound pointer to map
	mSounds[filePath] = sound;
}

void AudioManager::playSound(std::string soundName) {
	FMOD::Sound* sound = mSounds[soundName];

	FMOD::Channel *channel;
	FMOD_RESULT result = system->playSound(sound, nullptr, false, &channel);
}


