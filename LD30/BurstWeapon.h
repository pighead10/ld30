#pragma once

#include "Weapon.h"

class BurstWeapon : public Weapon{
public:
	BurstWeapon(ResourceManager<sf::Texture, std::string>* resourceManager);
	virtual void fire(sfld::Vector2f direction, sfld::Vector2f position, sfld::Vector2f startVelocity);
	void update(int frameTime);
private:
	sfld::Vector2f direction_;
	sfld::Vector2f position_;
	sfld::Vector2f startVelocity_;

	bool isFiring;
	int amountFired;
	int minitimer;

	void actuallyFire(sfld::Vector2f direction, sfld::Vector2f position, sfld::Vector2f startVelocity);
	void actuallySimulFire(sfld::Vector2f target, sfld::Vector2f position, sfld::Vector2f startVelocity);

	void generateProjectile(sfld::Vector2f direction, sfld::Vector2f position, sfld::Vector2f startVelocity, World* world);
};