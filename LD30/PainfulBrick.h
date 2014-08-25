#pragma once

#include "Entity.h"

class PainfulBrick : public Entity{
public:
	PainfulBrick(ResourceManager<sf::Texture,std::string>* resourceManager);
	void update(int frameTime);
	void hitSomething(Entity* entity);
};