#include "stdafx.h"
#include "Bullet.h"
#include "SoundManager.h"

Bullet::Bullet(ResourceManager<sf::Texture, std::string>* resourceManager,std::string textureName,sfld::Vector2f direction,
	sfld::Vector2f startVelocity,int damage)
:direction_(direction),speed_(2.0f),startVelocity_(startVelocity),damage_(damage){
	resourceManager_ = resourceManager;
	sprite_.setTexture(resourceManager_->get(textureName));
	IS_DESTROYED = false;
	type_ = TYPE_PROJECTILE;
	boundsHit_ = 0;
}

void Bullet::update(int frameTime){
	move(startVelocity_);
	move(direction_ * speed_);
}

void Bullet::hitBounds(World::BOUNDING_SIDE boundingSide){
	//SoundManager::play("bound");
	direction_.x = -direction_.x;
	boundsHit_++;
	if (boundsHit_ >= 4){
		destroy();
	}
}

void Bullet::hitSomething(Entity* entity){
	if (entity->getType() != TYPE_PROJECTILE){
		entity->takeDamage(20);
		destroy();
	}
}

