#pragma once

#include "Vector.h"
#include "ResourceManager.h"

class Entity;
class World;

class Weapon{
public:
	Weapon();
	virtual void fire(sfld::Vector2f direction,sfld::Vector2f position,sfld::Vector2f startVelocity);
	virtual void simulFire(sfld::Vector2f target, sfld::Vector2f position, sfld::Vector2f startVelocity);
	virtual void update(int frameTime);
	void setWorld(World* world);
	void setParent(Entity* parent);
	virtual void addWorld(World* world);
protected:
	ResourceManager<sf::Texture, std::string>* resourceManager_;

	virtual void generateProjectile(sfld::Vector2f direction, sfld::Vector2f position, sfld::Vector2f startVelocity,World* world);

	int timer_;
	int reloadThreshold_;

	Entity* parent_;

	std::vector<World*> worlds_;
};