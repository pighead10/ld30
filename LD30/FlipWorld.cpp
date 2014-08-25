#include "stdafx.h"
#include "FlipWorld.h"
#include "Player.h"

FlipWorld::FlipWorld(ResourceManager<sf::Texture, std::string>* resourceManager){
	resourceManager_ = resourceManager;
	sprite_.setTexture(resourceManager->get("flipworld"));
	IS_DESTROYED = false;
	type_ = TYPE_BRICK;
}

void FlipWorld::update(int frameTime){
	scrollDown(frameTime);
}

void FlipWorld::hitSomething(Entity* entity){
	if (entity->getType() == TYPE_PLAYER){
		parent_->flipView();
		destroy();
	}
}