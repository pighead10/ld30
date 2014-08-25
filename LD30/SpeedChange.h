#pragma once

#include "Entity.h"

class SpeedChange : public Entity{
public:
	SpeedChange(ResourceManager<sf::Texture, std::string>* resourceManager,float speedMod,std::string textureName,int length);
	void update(int frameTime);
	void hitSomething(Entity* entity);
	bool deleteAfterOOB() const;
private:
	float length_;
	float speedMod_;
	Player* player_;
	float originalScrollSpeed;
	int timer_;
	bool activated;
};

