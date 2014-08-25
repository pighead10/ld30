#include "stdafx.h"
#include "Player.h"
#include "World.h"
#include "Weapon.h"
#include "SFLD.h"
#include "SoundManager.h"
#include "ParticleEngine.h"

Player::Player(ResourceManager<sf::Texture, std::string>* resourceManager,ParticleEngine* particleEngine) :speed(0.6f){
	resourceManager_ = resourceManager;
	particleEngine_ = particleEngine;
	sprite_.setTexture(resourceManager_->get("player"));
	weapon_ = NULL;
	IS_DESTROYED = false;
	type_ = TYPE_PLAYER;
	health = 200;
	animationTimer = 0;
	dead = false;
}

void Player::addWorld(World* world){
	worlds_.push_back(world);
}

void Player::setHealth(int amount){
	health = amount;
	dead = false;
}

void Player::setScrollSpeed(float speed){
	for (auto& it : worlds_){
		it->setScrollSpeed(speed);
	}
}

void Player::setWeapon(Weapon* weapon){
	weapon_ = weapon;
	for (auto& it : worlds_){
		weapon->addWorld(it);
	}
}

void Player::resetScrollSpeed(){
	for (auto& it : worlds_){
		it->resetScrollSpeed();
	}
}

void Player::takeDamage(int amount){
	health -= amount;
	for (auto& it : worlds_){
		it->screenShake(1, 1000);
		particleEngine_->generateBloodEffect(getCentre()+it->getOffset());
	}
	SoundManager::play("hurt");
	if (health <= 0){
		if (!dead){
			for (auto& it : worlds_){
				it->screenShake(1.5, 2000);
				particleEngine_->generateExplosionEffect(getCentre() + it->getOffset());
			}
			SoundManager::play("dead");
			dead = true;
		}
	}
	
}

int Player::getHealth() const{
	return health;
}

void Player::update(int frameTime){
	using namespace sf;
	weapon_->update(frameTime);
	sfld::Vector2f d(0, 0);
	if (Keyboard::isKeyPressed(Keyboard::W)){
		//d.y -= 1;
	}if (Keyboard::isKeyPressed(Keyboard::S)){
	//	d.y += 1;
	}if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left)){
		d.x -= 1;
	}if (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right)){
		d.x += 1;
	}

	if (Mouse::isButtonPressed(Mouse::Left)){
		if (weapon_){
			sfld::Vector2i mousePosi = Mouse::getPosition(*SFLD::window_);
			sfld::Vector2f mousePos = sfld::Vector2f((float)mousePosi.x, (float)mousePosi.y);

			sfld::Vector2f pos;

			//determine which world the mouse is in
			for (auto& it : worlds_){
				float minx = it->getOffset().x;
				float maxx = minx + it->getSize().x;

				if (mousePos.x >= minx && mousePos.x < maxx){
					pos = getCentre() + it->getOffset();
				}
			}
			
			sfld::Vector2f dir = sfld::Vector2f(mousePos - pos).normalise();
			weapon_->fire(dir,getCentre()+sfld::Vector2f(0,-25-getRadius()) ,sfld::Vector2f(0,0));
		}
	}

	d = d.normalise();
	setPosition(getPosition() + d*speed*(float)frameTime);
}

void Player::render(sf::RenderTarget* target){
	for (auto& it : worlds_){
		if (isInsideBounds(getCentre(), it->getSize())){
			sprite_.setPosition(getPosition() + it->getOffset());
			target->draw(sprite_);
		}
	}
}

void Player::setPosition(const sfld::Vector2f& position){
	bool canMove = true;
	for (auto& it : worlds_){
		if (!isInsideBounds(calculateCentre(position, getRadius()), it->getSize())){
			canMove = false;
			break;
		}
	}
	if (canMove){
		position_ = position;
	}
}