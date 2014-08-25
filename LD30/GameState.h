#pragma once

#include "BaseState.h"
#include "ResourceManager.h"

class World;
class Player;

class ParticleEngine;

class GameState : public BaseState{
public:
	GameState();
	~GameState();
	void sfmlEvent(sf::Event evt);

	void start();
	void pause();
	void resume();
	void exit();

	void update(int frameTime);
	void render(sf::RenderTarget* target);

	void doGeneration(int frameTime);
	void nextStage();

	void displayMessage(std::string message);
private:
	bool exit_;
	bool messageDisplayed;
	sf::Text messageText;
	int messageTimer;

	bool dead;

	sf::Font font;
	sf::Text stageText;
	sf::Text healthText;

	sf::Sprite deadScreen;

	bool sped;
	bool flipped;
	void spawnWall(int blockn, float startx, World* world);

	bool currentleft;
	int wallthreshold;
	int walltimer;
	int nextthing;
	int nextenemy;
	int smalltimer;
	int enemytimer;
	int enemytimer2;
	int nextenemy2;

	int counts;

	int stageNum_;
	int stageTime_;
	std::unique_ptr<Player> player_;

	std::unique_ptr<World> world1_;
	std::unique_ptr<World> world2_;

	ResourceManager<sf::Texture,std::string> resourceManager_;
	std::unique_ptr<ParticleEngine> particleEngine_;
};