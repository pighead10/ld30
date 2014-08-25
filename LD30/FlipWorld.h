#pragma once

#include "Entity.h"

class FlipWorld : public Entity{
public:
	FlipWorld(ResourceManager<sf::Texture, std::string>* resourceManager);
	void update(int frameTime);
	void hitSomething(Entity* entity);
};