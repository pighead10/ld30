#include "stdafx.h"
#include "BurstWeapon.h"
#include "Bullet.h"
#include "SoundManager.h"

BurstWeapon::BurstWeapon(ResourceManager<sf::Texture, std::string>* resourceManager){
	resourceManager_ = resourceManager;
	reloadThreshold_ = 1500;
	amountFired = 0;
	isFiring = false;
	minitimer = 0;
}

void BurstWeapon::fire(sfld::Vector2f direction, sfld::Vector2f position, sfld::Vector2f startVelocity){
	if (timer_ >= reloadThreshold_){
		SoundManager::play("smallshoot");
		isFiring = true;
		timer_ = 0;
		minitimer = 10000;
		direction_ = direction;
		position_ = position;
		startVelocity_ = startVelocity;
		amountFired = 0;
	}
}

void BurstWeapon::update(int frameTime){
	timer_ += frameTime;
	if (isFiring){
		minitimer += frameTime;
		if (minitimer > 50){
			actuallyFire(direction_, position_, startVelocity_);
			amountFired++;
			minitimer = 0;
			if (amountFired >= 4){
				isFiring = false;
			}
		}
	}
}

void BurstWeapon::actuallyFire(sfld::Vector2f direction,sfld::Vector2f position,sfld::Vector2f startVelocity){
	for (auto& it : worlds_){
		generateProjectile(direction, position, startVelocity, it);
	}
}

void BurstWeapon::generateProjectile(sfld::Vector2f direction, sfld::Vector2f position, sfld::Vector2f startVelocity, World* world){
	Bullet* bullet = new Bullet(resourceManager_, "minibullet", direction, startVelocity, 10);
	world->addEntity(bullet);
	bullet->setPosition(position);
}