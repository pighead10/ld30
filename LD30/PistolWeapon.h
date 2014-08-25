#pragma once

#include "Weapon.h"

class PistolWeapon : public Weapon{
public:
	PistolWeapon(ResourceManager<sf::Texture,std::string>* resourceManager,int reloadTheshold=500);
private:
	void generateProjectile(sfld::Vector2f direction,sfld::Vector2f position,sfld::Vector2f startVelocity,World* world);
};