#include "stdafx.h"
#include "World.h"
#include "Entity.h"
#include "Player.h"
#include "SoundManager.h"
#include "GameState.h"

World::World(const sf::FloatRect& bounds, const sf::FloatRect& viewport, std::string textureName, ResourceManager<sf::Texture, std::string>* resourceManager,GameState* gameState) :scrollSpeed_(0.3f){
	init_bounds(bounds,viewport);
	isFlipping = false;
	alreadyflipped = false;
	resourceManager_ = resourceManager;
	bg1.setTexture(resourceManager->get(textureName));
	bg2.setTexture(resourceManager->get(textureName));
	bgy = 0;
	gameState_ = gameState;
}

World::~World(){

}

Player* World::getPlayer() const{
	return player_;
}

void World::displayMessage(std::string text){
	gameState_->displayMessage(text);
}

float World::getScrollSpeed() const{
	return scrollSpeed_;
}

void World::setScrollSpeed(float speed){
	scrollSpeed_ = speed;
}

sf::FloatRect World::getBounds() const{
	return bounds_;
}

sfld::Vector2f World::getSize() const{
	return sfld::Vector2f(bounds_.width, bounds_.height);
}

sfld::Vector2f World::getOffset() const{
	return sfld::Vector2f(bounds_.left, bounds_.top);
}

void World::setPlayer(Player* player){
	player_ = player;
	player_->addWorld(this);
}

void World::resetScrollSpeed(){
	scrollSpeed_ = 0.3f;
}

void World::flipView(){
	SoundManager::play("worldflip");
	isFlipping = true;
	flipSpeed = 1.f;
	flipTicks = 180 / (int)flipSpeed;
	flipCount = 0;
	displayMessage("VORTEX! WORLD FLIPPED!");
}

void World::init_bounds(const sf::FloatRect& bounds,const sf::FloatRect& viewport){
	bounds_ = bounds;
	view_.reset(bounds_);
	constantView_.reset(bounds_);
	constantView_.setViewport(viewport);
	view_.setViewport(viewport);
}

void World::addEntity(Entity* entity){
	entity->setOffset(getOffset());
	entity->setWorld(this);
	entityQueue_.push_back(std::unique_ptr<Entity>(entity));
}

sf::View* World::getView(){
	return &view_;
}

sfld::Vector2f World::getScrollVelocity() const{
	return getScrollSpeed() * sfld::Vector2f(0, 1);
}

void World::screenShake(float mag, int length){
	screenShaking = true;
	shakeMag = mag;
	shakeLength = length;
	shakeTimer = 0;
}

void World::clear(){
	setScrollSpeed(0.3f);
	view_.reset(bounds_);
	constantView_.reset(bounds_);
	isFlipping = false;
	alreadyflipped = false;
	entities_.clear();
}

void World::update(int frameTime){
	if (screenShaking){
		float xr = rand() % 9 - 5;
		float yr = rand() % 9 - 5;
		view_.setCenter(constantView_.getCenter() + sfld::Vector2f(xr*shakeMag, yr*shakeMag));
		shakeTimer += frameTime;
		if (shakeTimer >= shakeLength){
			screenShaking = false;
			view_.setCenter(constantView_.getCenter());
		}
	}

	bg1.setPosition(getOffset().x,bgy - getSize().y);
	bg2.setPosition(getOffset().x, bgy);

	if (bgy > getSize().y){
		bgy = 0;
	}
	bgy += getScrollSpeed()/ 5.f;
	

	for (auto& it = entities_.begin(); it != entities_.end();){
		if ((*it)->isDestroyed()){
			it = entities_.erase(it);
		}
		else{
			(*it)->update(frameTime);
			it++;
		}
	}

	for (auto& it = entities_.begin(); it != entities_.end(); it++){
		if (!(*it)->isInsideBounds()){
			sfld::Vector2f pos = (*it)->getPosition();
			float leftdist = pos.x;
			float rightdist = getSize().x - pos.x;

			if (pos.y < -100 || pos.y > getSize().y+(*it)->getRadius()){
				if ((*it)->deleteAfterOOB()){
					(*it)->destroy();
				}
			}
			if (leftdist < rightdist){
				(*it)->hitBounds(BOUNDING_LEFT);
			}
			else{
				(*it)->hitBounds(BOUNDING_RIGHT);
			}
		}
		for (auto& it2 = entities_.begin(); it2 != entities_.end(); it2++){
			if (it == it2){
				continue;
			}
			else{
				if ((*it)->isCollided(it2->get())){
					(*it)->hitSomething(it2->get());
				}
			}
		}
		if ((*it)->isCollided(player_)){
			(*it)->hitSomething(player_);
			player_->hitSomething(it->get());
		}
	}

	for (auto& it : entityQueue_){
		entities_.push_back(std::move(it));
	}
	entityQueue_.clear();

	if (!isFlipping && alreadyflipped){
		view_.setRotation(180);
		constantView_.setRotation(180);
	}

	if (isFlipping){
		flipCount++;
		float toadd = 0;
		if (alreadyflipped){
			toadd += 180;
		}
		view_.setRotation(flipSpeed*flipCount + toadd);
		constantView_.setRotation(flipSpeed*flipCount + toadd);
		if (flipCount >= flipTicks){
			alreadyflipped = !alreadyflipped;
			isFlipping = false;
		}
	}

	
}

void World::render(sf::RenderTarget* target){
	target->draw(bg1);
	target->draw(bg2);
	for (auto& it : entities_){
		if (it->isInsideBounds(it->getCentre())){
			it->render(target);
		}
	}

	
}