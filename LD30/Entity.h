#pragma once

#include "Vector.h"
#include <SFML/Graphics.hpp>
#include "ResourceManager.h"

#include "World.h"

class Weapon;
class ParticleEngine;

class Entity{
public:
	enum ENTITY_TYPE{
		TYPE_PROJECTILE,
		TYPE_ENEMY,
		TYPE_PLAYER,
		TYPE_BRICK,
	};
	void setWorld(World* world);
	void setOffset(const sfld::Vector2f& offset);

	virtual void update(int frameTime);
	virtual void render(sf::RenderTarget* target);

	virtual void setPosition(const sfld::Vector2f& position);
	sfld::Vector2f getPosition() const;

	sfld::Vector2f getCentre() const;
	sfld::Vector2f calculateCentre(const sfld::Vector2f& position,float rad) const;

	void move(const sfld::Vector2f& offset);

	virtual bool isDestroyed() const;

	virtual float getRadius() const;

	bool isInsideBounds() const;
	bool isInsideBounds(const sfld::Vector2f& centre) const;
	bool isInsideBounds(const sfld::Vector2f& centre,sfld::Vector2f& bounds) const;

	virtual void hitSomething(Entity* entity);
	virtual void hitBounds(World::BOUNDING_SIDE boundingSide);

	ENTITY_TYPE getType() const;

	bool isCollided(Entity* entity) const;

	virtual void setWeapon(Weapon* weapon);

	virtual void takeDamage(int amount){}

	virtual bool deleteAfterOOB() const;
	void destroy();
protected:
	ParticleEngine* particleEngine_;
	bool IS_DESTROYED;
	
	ENTITY_TYPE type_;

	virtual void scrollDown(int frameTime);

	sfld::Vector2f position_;

	ResourceManager<sf::Texture, std::string>* resourceManager_;
	sf::Sprite sprite_;
	sfld::Vector2f offset_;
	World* parent_;
};