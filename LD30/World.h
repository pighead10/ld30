#pragma once

#include "Vector.h"

class Entity;
class Player;
class GameState;
#include "ResourceManager.h"

class World{
public:
	enum BOUNDING_SIDE{
		BOUNDING_LEFT,
		BOUNDING_RIGHT
	};
	World(const sf::FloatRect& bounds,const sf::FloatRect& viewport,std::string textureName,ResourceManager<sf::Texture,std::string>* resourceManager,GameState* gameState);
	~World();

	void render(sf::RenderTarget* target);
	void update(int frameTime);

	void addEntity(Entity* entity);
	sf::View* getView();
	sfld::Vector2f getSize() const;
	sf::FloatRect getBounds() const;
	sfld::Vector2f getOffset() const;

	void setScrollSpeed(float scrollSpeed);

	float getScrollSpeed() const;
	sfld::Vector2f getScrollVelocity() const;

	void setPlayer(Player* player);
	Player* getPlayer() const;

	void flipView();
	void screenShake(float mag, int length);
	void clear();

	void displayMessage(std::string text);
	void resetScrollSpeed();
private:
	GameState* gameState_;
	float shakeMag;
	bool screenShaking;
	int shakeTimer;
	int shakeLength;

	float bgy;
	ResourceManager<sf::Texture, std::string>* resourceManager_;

	bool alreadyflipped;
	bool isFlipping;
	float flipSpeed;
	int flipTicks;
	int flipCount;

	Player* player_;

	std::vector<std::unique_ptr<Entity>> entityQueue_;
	std::vector<std::unique_ptr<Entity>> entities_;

	void init_bounds(const sf::FloatRect& bounds,const sf::FloatRect& viewport);

	sf::FloatRect bounds_;
	sf::View view_;

	sf::Sprite bg1;
	sf::Sprite bg2;

	float scrollSpeed_;
	sf::View constantView_;
};