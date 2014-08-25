#include "stdafx.h"
#include "PistolWeapon.h"
#include "Bullet.h"
#include "World.h"

PistolWeapon::PistolWeapon(ResourceManager<sf::Texture,std::string>* resourceManager,int reloadThreshold){
	resourceManager_ = resourceManager;
	reloadThreshold_ = reloadThreshold;
	timer_ = reloadThreshold_;
}

void PistolWeapon::generateProjectile(sfld::Vector2f direction,sfld::Vector2f position,sfld::Vector2f startVelocity,World* world){
	Bullet* bullet = new Bullet(resourceManager_, "bullet", direction,startVelocity,10);
	world->addEntity(bullet);
	bullet->setPosition(position);
}