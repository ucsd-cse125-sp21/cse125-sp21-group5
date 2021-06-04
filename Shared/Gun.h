#pragma once
#include <string>
#include <boost/serialization/string.hpp>

class Gun
{
public:
	std::string name;

	unsigned int fire_rate;
	unsigned int clip_size;
	unsigned int reload_time;
	unsigned int fire_rate_reset;
	unsigned int clip_size_reset;
	unsigned int reload_time_reset;
	unsigned int bullets_per_shot;
	float damage_per_bullet;
	float x_spread;
	float y_spread;
	float recoil;

	void decrement_reload_time()
	{
		if (reload_time > 0)
		{
			reload_time--;
			if (reload_time == 0)
				clip_size = clip_size_reset;
		}
	}

	void decrement_fire_rate()
	{
		if (fire_rate > 0)
			fire_rate--;
		return;
	}

	void reload()
	{
		if (reload_time == 0)
			reload_time = reload_time_reset;
	}

	bool fire()
	{
		// Check if player needs to reload
		if (clip_size == 0)
		{
			// Don't reset during reload
			reload();
			return false;
		}

		// Don't fire until ready
		if (fire_rate > 0)
			return false;

		// Shoot bullet(s)
		clip_size -= 1;

		// Reset fire_rate
		fire_rate = fire_rate_reset;

		return true;
	}

	Gun()
	{
		name = "Gun";
		fire_rate = 0;
		fire_rate_reset = 0;
		clip_size = 0;
		clip_size_reset = 0;
		reload_time = 0;
		reload_time_reset = 0;
		bullets_per_shot = 0;
		damage_per_bullet = 0.0f;
		x_spread = 0.0f;
		y_spread = 0.0f;
		recoil = 0.0f;
	};

	template <typename Archive>
	void serialize(Archive& ar, const unsigned int version) {
		ar& name;
		ar& fire_rate;
		ar& fire_rate_reset;
		ar& clip_size;
		ar& clip_size_reset;
		ar& reload_time;
		ar& reload_time_reset;
		ar& bullets_per_shot;
		ar& damage_per_bullet;
		ar& x_spread;
		ar& y_spread;
	}
};

class Pistol : public Gun
{
public:
	Pistol()
	{
		name = "Pistol";
		fire_rate = 0;
		fire_rate_reset = 30;
		clip_size = 6;
		clip_size_reset = 6;
		reload_time = 0;
		reload_time_reset = 60;
		bullets_per_shot = 1;
		damage_per_bullet = 30.0f;
		x_spread = 0.0f;
		y_spread = 0.0f;
		recoil = 1.0f;
	}
};

class Shotgun : public Gun
{
public:
	Shotgun()
	{
		name = "Shotgun";
		fire_rate = 0;
		fire_rate_reset = 60;
		clip_size = 2;
		clip_size_reset = 2;
		reload_time = 0;
		reload_time_reset = 120;
		bullets_per_shot = 5;
		damage_per_bullet = 7.5f;
		x_spread = 0.5f;
		y_spread = 0.5f;
		recoil = 3.0f;
	}
};

class Rifle : public Gun {
public:
	Rifle() {
		name = "Rifle";
		fire_rate = 0;
		fire_rate_reset = 10;
		clip_size = 36;
		clip_size_reset = 36;
		reload_time = 0;
		reload_time_reset = 90;
		bullets_per_shot = 1;
		damage_per_bullet = 6.0f;
		x_spread = 0.01f;
		y_spread = 0.01f;
		recoil = 0.5f;
	}
};

class FOV : public Gun
{
public:
	FOV()
	{
		name = "Flashbang Launcher";
		fire_rate = 0;
		fire_rate_reset = 10;
		clip_size = 1;
		clip_size_reset = 1;
		reload_time = 0;
		reload_time_reset = 300;
		bullets_per_shot = 1;
		damage_per_bullet = 0.0f;
		x_spread = 0.0f;
		y_spread = 0.0f;
		recoil = 1.0f;
	}
};

class FOG : public Gun
{
public:
	FOG()
	{
		name = "Smoke Grenade Launcher";
		fire_rate = 0;
		fire_rate_reset = 10;
		clip_size = 1;
		clip_size_reset = 1;
		reload_time = 0;
		reload_time_reset = 300;
		bullets_per_shot = 1;
		damage_per_bullet = 0.0f;
		x_spread = 0.0f;
		y_spread = 0.0f;
		recoil = 1.0f;
	}
};

class Stun : public Gun
{
public:
	Stun()
	{
		name = "Stun Grenade Launcher";
		fire_rate = 0;
		fire_rate_reset = 10;
		clip_size = 1;
		clip_size_reset = 1;
		reload_time = 0;
		reload_time_reset = 480;
		bullets_per_shot = 1;
		damage_per_bullet = 0.0f;
		x_spread = 0.0f;
		y_spread = 0.0f;
		recoil = 1.0f;
	}
};
