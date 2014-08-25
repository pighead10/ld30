#include "stdafx.h"
#include "StandardEnemy.h"
#include "Weapon.h"
#include "Player.h"
#include "SoundManager.h"
#include "ParticleEngine.h"

StandardEnemy::StandardEnemy(ResourceManager<sf::Texture, std::string>* resourceManager,std::string textureName, ParticleEngine* particleEngine,
	int health,float speed):timer_(10000),speed_(speed){
	particleEngine_ = particleEngine;
	direction_.y = 0;
	resourceManager_ = resourceManager;
	sprite_.setTexture(resourceManager->get(textureName));
	IS_DESTROYED = false;
	type_ = TYPE_ENEMY;
	health_ = health;
}

void StandardEnemy::takeDamage(int amount){
	health_ -= amount;
	if (health_ <= 0){
		parent_->screenShake(0.5f, 500);
		particleEngine_->generateMiniExplosionEffect(getCentre()+parent_->getOffset());
		SoundManager::play("enemydead");
		destroy();
	}
}

void StandardEnemy::hitSomething(Entity* entity){
	entity->takeDamage(20);
	if (entity->getType() == TYPE_PLAYER){
		destroy();
	}
}

void StandardEnemy::update(int frameTime){
	scrollDown(frameTime);
	weapon_->update(frameTime);
	Player* player = parent_->getPlayer();
	sfld::Vector2f pos = player->getCentre();
	float dist = sfld::Vector2f(getCentre() - pos).length();
	float lowy = parent_->getSize().y - 200;
	if (getCentre().y >= lowy){
		speed_ = 0.6f;
		direction_ = pos - getCentre();
		direction_ = direction_.normalise();
	}
	else{
		if (timer_ >= 1500){
			int n = rand() % 2;
			if (n == 0){
				direction_.x = -1;
			}
			else{
				direction_.x = 1;
			}
			timer_ = 0;
		}
	}

	move(direction_ * speed_);
	weapon_->fire(sfld::Vector2f(0, 1), getCentre()+sfld::Vector2f(0,getRadius()+30), parent_->getScrollVelocity());

	timer_ += frameTime;
}

void StandardEnemy::setWeapon(Weapon* weapon){
	weapon_ = weapon;
	weapon_->setWorld(parent_);
}

void StandardEnemy::hitBounds(World::BOUNDING_SIDE boundingSide){
	if (boundingSide == World::BOUNDING_LEFT){
		direction_.x = 1;
	}
	else{
		direction_.x = -1;
	}
}