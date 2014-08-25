#include "MenuState.h"
#include "GameState.h"
#include "StateManager.h"
#include "ResourceManager.h"

MenuState::MenuState(){

}

MenuState::~MenuState(){

}

void MenuState::pause(){

}

void MenuState::exit(){

}

void MenuState::resume(){

}

void MenuState::update(int frameTime){

}


void MenuState::start(){
	playselected = true;
	help = false;

	resourceManager_.setDirectory("media/images/");
	resourceManager_.load("playselected", "playselected.png");
	resourceManager_.load("helpselected", "helpselected.png");
	resourceManager_.load("helpscreen", "helpscreen.png");

	playselectedMenu.setTexture(resourceManager_.get("playselected"));
	helpselectedMenu.setTexture(resourceManager_.get("helpselected"));
	helpscreen.setTexture(resourceManager_.get("helpscreen"));
}

void MenuState::sfmlEvent(sf::Event evt){
	if (evt.type == sf::Event::KeyPressed){
		if (evt.key.code == sf::Keyboard::W || evt.key.code == sf::Keyboard::Up || evt.key.code == sf::Keyboard::S || evt.key.code == sf::Keyboard::Down){
			playselected = !playselected;
		}
		else if (evt.key.code == sf::Keyboard::Return){
			if (playselected){
				getParent().push(new GameState());
			}
			else{
				help = !help;
			}
		}
	}
}

void MenuState::render(sf::RenderTarget* target){
	if (help){
		target->draw(helpscreen);
	}
	else{
		if (playselected){
			target->draw(playselectedMenu);
		}
		else{
			target->draw(helpselectedMenu);
		}
	}
}