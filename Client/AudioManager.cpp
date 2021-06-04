#include "AudioManager.h"

#include <iostream>
#include <fmod_errors.h>

void AudioManager::init() {
	FMOD_RESULT result;

	result = FMOD::System_Create(&system);

	if (result != FMOD_OK) {
		std::cerr << "Failed to create FMOD system" << std::endl;
	}

	result = system->init(2048, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, 0);
	if (result != FMOD_OK) {
		std::cerr << "Failed to initialize FMOD system" << std::endl;
	}

	this->volume = 0.5f;

	loadSound(SOUND_MEOW);
	loadSound(SOUND_WOOF);
	loadSound(SOUND_STEP);
	loadSound(SOUND_PEW);
	loadSound(SOUND_DEATH);
	loadSound(SOUND_DUB);
	loadSound(SOUND_RIFLE);
	loadSound(SOUND_SHOTGUN);
	loadSound(SOUND_FLASHBANG);
	loadSound(SOUND_FREEZE);
	loadSound(SOUND_EASY_GAME);
}


void AudioManager::loadSound(std::string filePath) {
	FMOD::Sound* sound;
	FMOD_RESULT result = system->createSound(filePath.c_str(), FMOD_3D, nullptr, &sound);

	FMOD::SoundGroup* test;
	system->createSoundGroup("test group", &test);
	sound->setSoundGroup(test);
	

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

	// 3rd parameter is true to pause sound on load.
	FMOD_RESULT result = system->playSound(sound, nullptr, true, &channel);
	result = channel->setVolume(this->volume);
	result = channel->setPaused(false);
}

void AudioManager::playSound(std::string soundName, glm::vec3 position) {
	FMOD::Sound* sound = mSounds[soundName];

	FMOD::Channel *channel;

	// 3rd parameter is true to pause sound on load.
	FMOD_RESULT result = system->playSound(sound, nullptr, true, &channel);

	// set position
	FMOD_VECTOR fmodPos = {
		position.x,
		position.y,
		position.z
	};

	channel->set3DAttributes(&fmodPos, nullptr);
	
	result = channel->setVolume(this->volume);
	result = channel->setPaused(false);
}


void AudioManager::adjustVolume(float dVolume) {
	// Change volume variable
	this->volume += dVolume;

	// Clamp
	if (this->volume > 1.0f)
		this->volume = 1.0f;
	else if (this->volume < 0.0f)
		this->volume = 0.0f;
}

void AudioManager::setListenerPosition(glm::vec3 position, glm::vec3 forward, glm::vec3 up)
{
	FMOD_VECTOR fmodPos = {
		position.x,
		position.y,
		position.z
	};

	FMOD_VECTOR fmodForward = {
		forward.x,
		forward.y,
		forward.z
	};
	
	FMOD_VECTOR fmodUp = {
		up.x,
		up.y,
		up.z
	};

	system->set3DListenerAttributes(0, &fmodPos, nullptr, &fmodForward, &fmodUp);
}

