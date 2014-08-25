#pragma once

#include "Entity.h"

class Bullet : public Entity{
public:
	Bullet(ResourceManager<sf::Texture, std::string>* resourceManager,std::string textureName,sfld::Vector2f direction,sfld::Vector2f startVelocity,int damage);
	void update(int frameTime);
	void hitBounds(World::BOUNDING_SIDE boundingSide);
	void hitSomething(Entity* entity);
private:
	int boundsHit_;
	int damage_;
	sfld::Vector2f startVelocity_;
	sfld::Vector2f direction_;
	float speed_;
};