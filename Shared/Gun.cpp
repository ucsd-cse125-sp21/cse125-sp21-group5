#include "Gun.h"

// Decreases the fire rate timer
void Gun::decrement_fire_rate()
{
	if (fire_rate > 0)
		fire_rate--;
	return;
}

// Decreases the reload_timer
// Once time is up, clip is filled to capacity
void Gun::decrement_reload_time()
{
	if (reload_time > 0)
	{
		reload_time--;
		if (reload_time == 0)
			clip_size = clip_size_reset;
	}
}

void Gun::reload()
{
	if (reload_time == 0)
		reload_time = reload_time_reset;
}

bool Gun::fire()
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
	clip_size -= bullets_per_shot;

	// Reset fire_rate
	fire_rate = fire_rate_reset;

	return true;
}

Gun::Gun()
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
	x_recoil = 0.0f;
	y_recoil = 0.0f;
}

Pistol::Pistol()
{
	name = "Pistol";
	fire_rate = 0;
	fire_rate_reset = 10;
	clip_size = 6;
	clip_size_reset = 6;
	reload_time = 10;
	reload_time_reset = 10;
	bullets_per_shot = 1;
	damage_per_bullet = 10.0f;
	x_recoil = 0.0f;
	y_recoil = 0.0f;
}

Shotgun::Shotgun()
{
	name = "Shotgun";
	fire_rate = 0;
	fire_rate_reset = 10;
	clip_size = 6;
	clip_size_reset = 6;
	reload_time = 10;
	reload_time_reset = 10;
	bullets_per_shot = 1;
	damage_per_bullet = 10.0f;
	x_recoil = 0.0f;
	y_recoil = 0.0f;
}

Rifle::Rifle()
{
	name = "Rifle";
	fire_rate = 0;
	fire_rate_reset = 10;
	clip_size = 6;
	clip_size_reset = 6;
	reload_time = 10;
	reload_time_reset = 10;
	bullets_per_shot = 1;
	damage_per_bullet = 10.0f;
	x_recoil = 0.0f;
	y_recoil = 0.0f;
}