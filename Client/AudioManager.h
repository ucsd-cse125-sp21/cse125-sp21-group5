#pragma once

#include <fmod.hpp>
#include <unordered_map>

#include <glm/glm.hpp>

#define SOUND_WOOF "./res/sounds/environmentSounds/woof.ogg"
#define SOUND_MEOW "./res/sounds/environmentSounds/meow.ogg"
#define SOUND_STEP "./res/sounds/playerSounds/step.ogg"
#define SOUND_PEW "./res/sounds/gunSounds/shoot.ogg"
#define SOUND_DEATH "./res/sounds/playerSounds/death.ogg"
#define SOUND_DUB "./res/sounds/miscSounds/dub.ogg"
#define SOUND_RIFLE "./res/sounds/gunSounds/rifle.ogg"

class AudioManager
{
public:

	// START SINGLETON STUFF
	static AudioManager& get() {
		static AudioManager instance;
		return instance;
	}

	// these are all needed to make sure we don't get copies of the renderer singleton
	AudioManager(AudioManager const&) = delete;
	void operator=(AudioManager const&) = delete;
	// END SINGLETON STUFF
	
	void init();
	void playSound(std::string soundName);
	void playSound(std::string soundName, glm::vec3 position);
	void adjustVolume(float dVolume);

	void setListenerPosition(glm::vec3 position, glm::vec3 forward, glm::vec3 up);

	FMOD::System* system;

	// The master volume
	float volume;

private:
	AudioManager() {}

	std::unordered_map<std::string, FMOD::Sound*> mSounds;

	void loadSound(std::string filePath);
};

