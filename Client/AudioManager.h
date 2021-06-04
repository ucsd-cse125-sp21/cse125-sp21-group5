#pragma once

#include <fmod.hpp>
#include <unordered_map>

#include <glm/glm.hpp>

#define SOUND_WOOF "./res/sounds/environmentSounds/woof.ogg"
#define SOUND_MEOW "./res/sounds/environmentSounds/meow.ogg"

#define SOUND_STEP "./res/sounds/playerSounds/step.ogg"
#define SOUND_DEATH "./res/sounds/playerSounds/death.ogg"

#define SOUND_PEW "./res/sounds/gunSounds/shoot.ogg"
#define SOUND_RIFLE "./res/sounds/gunSounds/rifle.ogg"
#define SOUND_SHOTGUN "./res/sounds/gunSounds/shotgun.ogg"
#define SOUND_FLASHBANG "./res/sounds/gunSounds/flashbang.ogg"
#define SOUND_FREEZE "./res/sounds/gunSounds/freeze.ogg"
#define SOUND_FOG "./res/sounds/gunSounds/fog.ogg"

#define SOUND_DUB "./res/sounds/miscSounds/dub2.ogg"
#define SOUND_EASY_GAME "./res/sounds/miscSounds/easyGame.ogg"
#define SOUND_DAB "./res/sounds/miscSounds/dab.ogg"
#define SOUND_FLAG_CAPTURE "./res/sounds/miscSounds/capture.ogg"


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
	void playBackgroundMusic();
	void adjustBackgroundChannelVolume();
	void loadBackgroundSound(std::string filePath);
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

	FMOD::Channel* backgroundChannel;

	void loadSound(std::string filePath);
};

