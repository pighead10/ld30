#include "stdafx.h"
#include "Entity.h"
#include "World.h"

void Entity::setWorld(World* world){
	parent_ = world;
}

void Entity::setOffset(const sfld::Vector2f& offset){
	offset_ = offset;
}

void Entity::render(sf::RenderTarget* target){
	target->draw(sprite_);
}

void Entity::setPosition(const sfld::Vector2f& position){
	position_ = position;
	sprite_.setPosition(position + offset_);
}

sfld::Vector2f Entity::getCentre() const{
	return position_ + sfld::Vector2f(getRadius() / 2.f, getRadius() / 2.f);
}

sfld::Vector2f Entity::calculateCentre(const sfld::Vector2f& position, float rad) const{
	return position + sfld::Vector2f(rad / 2.f, rad / 2.f);
}

bool Entity::isInsideBounds() const{
	return isInsideBounds(getCentre(), parent_->getSize());
}

bool Entity::isInsideBounds(const sfld::Vector2f& centre) const{
	return isInsideBounds(centre, parent_->getSize());
}

bool Entity::isInsideBounds(const sfld::Vector2f& centre,sfld::Vector2f& bounds) const{
	if (centre.x + getRadius() > bounds.x || centre.x - getRadius() < 0
		|| centre.y + getRadius()  > bounds.y || centre.y - getRadius() < 0){
		return false;
	}
	return true;
}

void Entity::setWeapon(Weapon* weapon){

}

void Entity::destroy(){
	IS_DESTROYED = true;
}

bool Entity::deleteAfterOOB() const{
	return true;
}

float Entity::getRadius() const{
	return sprite_.getLocalBounds().width/2.f;
}

void Entity::update(int frameTime){
	
}

void Entity::move(const sfld::Vector2f& offset){
	setPosition(getPosition() + offset);
}

void Entity::scrollDown(int frameTime){
	setPosition(getPosition() + sfld::Vector2f(0, parent_->getScrollSpeed()));
}

sfld::Vector2f Entity::getPosition() const{
	return position_;
}

bool Entity::isDestroyed() const{
	return IS_DESTROYED;
}

void Entity::hitSomething(Entity* entity){

}

Entity::ENTITY_TYPE Entity::getType() const{
	return type_;
}

void Entity::hitBounds(World::BOUNDING_SIDE boundingSide){

}

bool Entity::isCollided(Entity* ent) const{
	float rad1 = getRadius();
	float rad2 = ent->getRadius();
	if (sfld::Vector2f(ent->getCentre() - getCentre()).length() <= getRadius() + ent->getRadius()){
		return true;
	}
	return false;
}



