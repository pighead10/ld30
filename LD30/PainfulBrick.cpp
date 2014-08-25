#include "stdafx.h"
#include "PainfulBrick.h"
#include "ResourceManager.h"

PainfulBrick::PainfulBrick(ResourceManager<sf::Texture, std::string>* resourceManager){
	resourceManager_ = resourceManager;
	sprite_.setTexture(resourceManager_->get("painfulbrick"));
	setPosition(sfld::Vector2f(0, 0));
	IS_DESTROYED = false;
	type_ = TYPE_BRICK;
}

void PainfulBrick::update(int frameTime){
	scrollDown(frameTime);
}

void PainfulBrick::hitSomething(Entity* entity){
	entity->takeDamage(20);
	if (entity->getType() == TYPE_PLAYER){
		destroy();
	}
}