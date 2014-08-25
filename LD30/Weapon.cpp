#include "stdafx.h"
#include "Weapon.h"
#include "World.h"
#include "SoundManager.h"

Weapon::Weapon(){

}

void Weapon::fire(sfld::Vector2f direction,sfld::Vector2f position,sfld::Vector2f startVelocity){
	if (timer_ >= reloadThreshold_){
		SoundManager::play("shoot");
		for (auto& it : worlds_){
			generateProjectile(direction, position, startVelocity,it);
		}
		timer_ = 0;
	}
}

void Weapon::simulFire(sfld::Vector2f target, sfld::Vector2f position, sfld::Vector2f startVelocity){
	if (timer_ >= reloadThreshold_){
		for (auto& it : worlds_){
			sfld::Vector2f dir = sfld::Vector2f(target - (position+it->getOffset())).normalise();
			generateProjectile(dir, position, startVelocity, it);
		}
		timer_ = 0;
	}
}

void Weapon::update(int frameTime){
	timer_ += frameTime;
}

void Weapon::addWorld(World* world){
	worlds_.push_back(world);
}

void Weapon::setWorld(World* world){
	worlds_.clear();
	worlds_.push_back(world);
}

void Weapon::setParent(Entity* parent){

}

void Weapon::generateProjectile(sfld::Vector2f direction, sfld::Vector2f position,sfld::Vector2f startVelocity,World* world){

}