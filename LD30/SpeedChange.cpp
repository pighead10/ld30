#include "stdafx.h"
#include "SpeedChange.h"
#include "ResourceManager.h"
#include "Player.h"
#include "SoundManager.h"

SpeedChange::SpeedChange(ResourceManager<sf::Texture, std::string>* resourceManager,float speedMod,std::string textureName,int length){
	speedMod_ = speedMod;
	resourceManager_ = resourceManager;
	sprite_.setTexture(resourceManager->get(textureName));
	type_ = TYPE_BRICK;
	IS_DESTROYED = false;
	activated = false;
	timer_ = 0;
	length_ = length;
}

bool SpeedChange::deleteAfterOOB() const{
	return false;
}

void SpeedChange::update(int frameTime){
	scrollDown(frameTime);
	if (activated){
		player_->setScrollSpeed(0.3f*speedMod_);
		timer_ += frameTime;
		if (timer_ >= length_){
			player_->resetScrollSpeed();
			destroy();
		}
	}
}

void SpeedChange::hitSomething(Entity* entity){
	if (!activated){
		if (entity->getType() == TYPE_PLAYER){
			if (speedMod_ > 1.f){
				SoundManager::play("speedup");
				parent_->displayMessage("SPEED UP!");
			}
			else{
				SoundManager::play("speeddown");
				parent_->displayMessage("SPEED DOWN!");
			}
			activated = true;
			originalScrollSpeed = parent_->getScrollSpeed();
			player_ = parent_->getPlayer();
			player_->setScrollSpeed(0.3f*speedMod_);
		}
	}
}