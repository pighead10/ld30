#pragma once

#include "BaseState.h"
#include "ResourceManager.h"

#include <memory>

class MenuState : public BaseState{
public:
	MenuState();
	~MenuState();

	void sfmlEvent(sf::Event evt);

	void render(sf::RenderTarget* target);
	void start();

	void pause();
	void exit();
	void resume();
	void update(int frameTime);
private:
	bool playselected;
	bool help;

	ResourceManager<sf::Texture, std::string> resourceManager_;

	sf::Sprite playselectedMenu;
	sf::Sprite helpselectedMenu;
	sf::Sprite helpscreen;
};