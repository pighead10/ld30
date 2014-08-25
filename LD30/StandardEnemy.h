#pragma once

#include "Entity.h"

class StandardEnemy : public Entity{
public:
	StandardEnemy(ResourceManager<sf::Texture, std::string>* resourceManager,std::string textureName,ParticleEngine* particleEngine,
		int health=10,float speed=0.1f);
	void update(int frameTime);
	void setWeapon(Weapon* weapon);
	void hitBounds(World::BOUNDING_SIDE boundingSide);
	void takeDamage(int amount);
	void hitSomething(Entity* entity);
private:
	sfld::Vector2f direction_;
	Weapon* weapon_;
	int timer_;
	int health_;
	float speed_;
};