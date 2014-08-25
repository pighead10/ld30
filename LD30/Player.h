#pragma once

#include "Entity.h"


class Player : public Entity{
public:
	Player(ResourceManager<sf::Texture,std::string>* resourceManager,ParticleEngine* particleEngine);
	void update(int frameTime);
	void render(sf::RenderTarget* target);

	void addWorld(World* world);

	void setPosition(const sfld::Vector2f& position);

	void setWeapon(Weapon* weapon);
	void takeDamage(int amount);
	void resetScrollSpeed();
	void setScrollSpeed(float speed);
	void setHealth(int amount);

	int getHealth() const;
private:
	bool dead;
	int animationTimer;
	int health;
	Weapon* weapon_;

	float speed;
	std::vector<World*> worlds_;
};