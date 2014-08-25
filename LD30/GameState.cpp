#include "stdafx.h"
#include "GameState.h"
#include "StateManager.h"
#include "ResourceManager.h"
#include "SoundManager.h"
#include "World.h"
#include "PainfulBrick.h"
#include "Player.h"
#include "PistolWeapon.h"
#include "StandardEnemy.h"
#include "SpeedChange.h"
#include "FlipWorld.h"
#include "BurstWeapon.h"
#include "ParticleEngine.h"

GameState::GameState() = default;
GameState::~GameState() = default;

void GameState::sfmlEvent(sf::Event evt){
	switch(evt.type){
	case sf::Event::Closed:
		getParent().pop();
		break;
	case sf::Event::KeyPressed:
		if (evt.key.code == sf::Keyboard::Return){
			if (dead){
				stageNum_--;
				dead = false;
			}
			world1_->clear();
			world2_->clear();
			nextStage();
		}
		else if (evt.key.code == sf::Keyboard::Escape){
			exit();
		}
		
	default:
		break;
	}
}

void GameState::start(){ 
	stageText.setCharacterSize(18);
	healthText.setCharacterSize(18);

	stageText.setPosition(10, 10);
	healthText.setPosition(200, 10);

	resourceManager_.setDirectory("media/images/");
	resourceManager_.load("painfulbrick", "painfulbrick.png");
	resourceManager_.load("player", "player.png");
	resourceManager_.load("bullet", "bullet.png");
	resourceManager_.load("standardEnemy", "standardenemy.png");
	resourceManager_.load("speedboost", "speedboost.png");
	resourceManager_.load("speedslow", "speedslow.png");
	resourceManager_.load("flipworld", "flipworld.png");
	resourceManager_.load("minibullet", "minibullet.png");
	resourceManager_.load("burstenemy", "burstenemy.png");
	resourceManager_.load("grassbg", "grassbg.png");
	resourceManager_.load("earthbg", "earthbg.png");
	resourceManager_.load("deadscreen", "deadscreen.png");

	font.loadFromFile("media/Victor-pixel.ttf");

	stageText.setFont(font);
	healthText.setFont(font);

	SoundManager::add("bound","media/sound/bound.wav");
	SoundManager::add("enemydead","media/sound/enemydead.wav");
	SoundManager::add("hurt","media/sound/hurt.wav");
	SoundManager::add("shoot","media/sound/shoot.wav");
	SoundManager::add("smallshoot","media/sound/smallshoot.wav");
	SoundManager::add("speeddown","media/sound/speeddown.wav");
	SoundManager::add("speedup","media/sound/speedup.wav");
	SoundManager::add("worldflip","media/sound/worldflip.wav");
	SoundManager::add("dead", "media/sound/dead.wav");
	SoundManager::addMusic("music", "media/sound/music.ogg");

	particleEngine_ = std::unique_ptr<ParticleEngine>(new ParticleEngine());

	deadScreen.setTexture(resourceManager_.get("deadscreen"));

	SoundManager::playMusic("music");

	world1_ = std::unique_ptr<World>(new World(sf::FloatRect(0, 0, 512, 768),sf::FloatRect(0,0,0.5,1),"grassbg",&resourceManager_,this));

	world2_ = std::unique_ptr<World>(new World(sf::FloatRect(512, 0, 512, 768), sf::FloatRect(0.5, 0, 0.5, 1),"earthbg",&resourceManager_,this));


	player_ = std::unique_ptr<Player>(new Player(&resourceManager_,particleEngine_.get()));
	player_->setPosition(sfld::Vector2f(350, 736));

	Weapon* playerweapon = new PistolWeapon(&resourceManager_,200);
	world1_->setPlayer(player_.get());
	world2_->setPlayer(player_.get());

	player_->setWeapon(playerweapon);
	
	messageDisplayed = false;
	messageText.setCharacterSize(50);
	messageText.setFont(font);

	/*SpeedChange* speed = new SpeedChange(&resourceManager_, 2.f, "speedboost");
	world1_->addEntity(speed);
	speed->setPosition(sfld::Vector2f(200, 0));*/

	/*FlipWorld* flip = new FlipWorld(&resourceManager_);
	world1_->addEntity(flip);
	flip->setPosition(sfld::Vector2f(200, 0));*/
	dead = false;
	stageNum_ = 0;
	nextStage();
}

void GameState::pause(){
}

void GameState::resume(){
}

void GameState::exit(){
	exit_ = true;
}

void GameState::displayMessage(std::string message){
	messageDisplayed = true;
	messageText.setString(message);
	messageText.setPosition(512 - messageText.getLocalBounds().width / 2, 384 - messageText.getLocalBounds().height / 2);
	messageTimer = 0;
}

void GameState::update(int frameTime){
	if (messageDisplayed){
		messageTimer += frameTime;
		if (messageTimer >= 2000){
			messageText.setPosition(messageText.getPosition().x, messageText.getPosition().y - 1 * frameTime);
			if (messageText.getPosition().y < -100){
				messageDisplayed = false;
			}
		}
	}


	world1_->update(frameTime);
	world2_->update(frameTime);
	if (!dead){
		player_->update(frameTime);
	}
	stageTime_ += frameTime;

	if (player_->getHealth() > 0){
		doGeneration(frameTime);
	}
	else{
		dead = true;
	}
	SoundManager::update(frameTime);
	particleEngine_->update(frameTime);

	if (exit_){
		getParent().pop();
	}
}

void GameState::nextStage(){
	
	stageNum_++;
	world1_->clear();
	world2_->clear();

	std::string msg = "Next stage!";

	if (stageNum_ == 1){
		msg = "Avoid grey blocks! Shoot enemies!";
	}
	else if (stageNum_ == 4){
		msg = "Purple enemies shoot more!";
	}
	else if (stageNum_ == 5){
		msg = "Purple circles flip the world!";
	}
	else if (stageNum_ == 6){
		msg = "Final stage! It's difficult!";
	}else if (stageNum_ == 7){
		msg = "You won! No end screen! :(";
	}

	displayMessage(msg);


	//reset all values
	nextthing = 0;
	smalltimer = 0;
	stageTime_ = 0;
	walltimer = 0;
	wallthreshold = 0;
	player_->setHealth(200);
	currentleft = true;
	enemytimer = 0;
	sped = false;
	nextenemy = 0;
	flipped = false;
	enemytimer2 = 0;
	nextenemy2 = 0;
	counts = 0;
}

void GameState::doGeneration(int frameTime){
	if (stageNum_ == 1){
		if (stageTime_ >= 2000){
			if (smalltimer >= nextthing){
				nextthing = 500 + rand() % 2000;
				//nextthing += ((15000 - stageTime_) / 4);
				smalltimer = 0;

				int worldn = rand() % 2;
				World* w;
				if (worldn == 0){
					w = world1_.get();
				}
				else{
					w = world2_.get();
				}
				StandardEnemy* enemy = new StandardEnemy(&resourceManager_,"standardEnemy",particleEngine_.get());
				Weapon* weapon = new PistolWeapon(&resourceManager_);
				w->addEntity(enemy);
				enemy->setPosition(sfld::Vector2f((float)(rand() % (int)(w->getSize().x - enemy->getRadius())), -50.f));
				enemy->setWeapon(weapon);

			}
			
			if (walltimer >= wallthreshold){
				int worldn = rand() % 2;
				World* w;
				if (worldn == 0){
					w = world1_.get();
				}
				else{
					w = world2_.get();
				}
				walltimer = 0;
				wallthreshold = 500 + rand() % 1000;
				PainfulBrick* brick = new PainfulBrick(&resourceManager_);
				w->addEntity(brick);
				brick->setPosition(sfld::Vector2f((float)(rand() % (int)(w->getSize().x - brick->getRadius())), -50.f));
			}
			walltimer += frameTime;
			smalltimer += frameTime;

			if (stageTime_ >= 25000){
				nextStage();
			}
		}
	}
	else if (stageNum_ == 2){
		if (stageTime_ >= 2000){
			enemytimer += frameTime;
			smalltimer += frameTime;
			if (smalltimer > 1000){
				if (currentleft){
					currentleft = false;
					spawnWall(8, 0, world1_.get());
				}
				else{
					spawnWall(8, world2_->getSize().x - 8 * 16, world2_.get());
					currentleft = true;
				}
				smalltimer = 0;
			}

			if (enemytimer >= nextthing){
				nextthing = 500 + rand() % 1000;
				int worldn = rand() % 2;
				World* world;
				if (worldn == 0){
					world = world1_.get();
				}
				else{
					world = world2_.get();
				}
				int typen = rand() % 2;
				if (typen == 0){
					StandardEnemy* enemy = new StandardEnemy(&resourceManager_, "standardEnemy", particleEngine_.get());
					float randx = 8*16 + rand() % ((int)world->getSize().x - 8 * 32);
					sfld::Vector2f pos(randx, -50);
					Weapon* weapon = new PistolWeapon(&resourceManager_);
					world->addEntity(enemy);
					enemy->setPosition(pos);
					enemy->setWeapon(weapon);
				}
				else{
					PainfulBrick* brick = new PainfulBrick(&resourceManager_);
					float randx = 8 * 16 + rand() % ((int)world->getSize().x - 8 * 32);
					sfld::Vector2f pos(randx, -50);
					world->addEntity(brick);
					brick->setPosition(pos);
				}
				
				enemytimer = 0;
			}
			if (stageTime_ >= 25000){
				nextStage();
			}
		}
	}
	else if (stageNum_ == 3){
		if (stageTime_ >= 2000){
			if (!sped){
				SpeedChange* speed = new SpeedChange(&resourceManager_, 2.5f, "speedboost",18000);
				world1_->addEntity(speed);
				speed->setPosition(player_->getPosition());
				sped = true;
			}
			smalltimer += frameTime;
			enemytimer += frameTime;
			if (enemytimer >= nextenemy){
				nextenemy = 500 + rand() % 1700;
				enemytimer = 0;
				int worldn = rand() % 2;
				World* w;
				if (worldn == 0){
					w = world1_.get();
				}
				else{
					w = world2_.get();
				}
				StandardEnemy* enemy = new StandardEnemy(&resourceManager_, "standardEnemy", particleEngine_.get());
				Weapon* weapon = new PistolWeapon(&resourceManager_);
				w->addEntity(enemy);
				enemy->setPosition(sfld::Vector2f((float)(rand() % (int)(w->getSize().x - enemy->getRadius())), -50.f));
				enemy->setWeapon(weapon);
			}
			if (smalltimer >= nextthing){
				nextthing = 100 + rand() % 300;
				smalltimer = 0;
				int worldn = rand() % 2;
				World* w;
				if (worldn == 0){
					w = world1_.get();
				}
				else{
					w = world2_.get();
				}
				PainfulBrick* brick = new PainfulBrick(&resourceManager_);
				w->addEntity(brick);
				brick->setPosition(sfld::Vector2f((float)(rand() % (int)(w->getSize().x - brick->getRadius())), -50.f));
			}
			if (stageTime_ >= 22000){
				nextStage();
			}
		}
	}
	else if (stageNum_ == 4){
		if (stageTime_ >= 2000){
			if (!sped){
				SpeedChange* speed = new SpeedChange(&resourceManager_, 0.5f, "speedboost", 30000);
				world1_->addEntity(speed);
				speed->setPosition(player_->getPosition());
				sped = true;
			}
			enemytimer += frameTime;
			smalltimer += frameTime;
			if (enemytimer >= nextenemy){
				nextenemy = 300 + rand() % 800;
				enemytimer = 0;
				int worldn = 0;
				if (stageTime_ >= 20000){
					worldn = 1;
				}
				World* w;
				if (worldn == 0){
					w = world1_.get();
				}
				else{
					w = world2_.get();
				}

				StandardEnemy* enemy = new StandardEnemy(&resourceManager_, "burstenemy", particleEngine_.get(),20, 0.3f);
				Weapon* weapon = new BurstWeapon(&resourceManager_);
				w->addEntity(enemy);
				enemy->setPosition(sfld::Vector2f((float)(rand() % (int)(w->getSize().x - enemy->getRadius())), -50.f));
				enemy->setWeapon(weapon);
			}
			if (smalltimer >= nextthing){
				smalltimer = 0;
				nextthing = 1000 + rand() % 1000;

				int worldn = 1;
				if (stageTime_ >= 20000){
					worldn = 0;
				}
				World* w;
				if (worldn == 0){
					w = world1_.get();
				}
				else{
					w = world2_.get();
				}

				int blockn = 3 + rand() % 10;
				int randx = rand() % (int)w->getSize().x;

				spawnWall(blockn, randx, w);
			}
			if (stageTime_ >= 33000){
				nextStage();
			}
		}
	}
	else if (stageNum_ == 5){
		if (stageTime_ >= 2000){
			if (!flipped){
				flipped = true;
				world1_->flipView();
			}

			enemytimer += frameTime;
			if (enemytimer >= nextenemy){
				nextenemy = 1000 + rand() % 1000;
				enemytimer = 0;

				StandardEnemy* enemy = new StandardEnemy(&resourceManager_, "standardEnemy", particleEngine_.get());
				Weapon* weapon = new PistolWeapon(&resourceManager_);
				world1_->addEntity(enemy);
				enemy->setPosition(sfld::Vector2f((float)(rand() % (int)(world1_->getSize().x - enemy->getRadius())), -50.f));
				enemy->setWeapon(weapon);
			}

			smalltimer += frameTime;
			if (smalltimer >= nextthing){
				nextthing = 1000 + rand() % 2000;
				if (stageTime_ >= 10000){
					nextthing = 500 + rand() % 100;
				}
				smalltimer = 0;

				FlipWorld* flip = new FlipWorld(&resourceManager_);
				int worldn = rand() % 2;
				World* w;
				if (worldn == 0){
					w = world1_.get();
				}
				else{
					w = world2_.get();
				}

				w->addEntity(flip);
				flip->setPosition(sfld::Vector2f((float)(rand() % (int)(world1_->getSize().x - flip->getRadius())), -50.f));
			}

			enemytimer2 += frameTime;
			if (enemytimer2 >= nextenemy2){
				enemytimer2 = 0;
				nextenemy2 = 500 + rand() % 1000;

				StandardEnemy* enemy = new StandardEnemy(&resourceManager_, "standardEnemy", particleEngine_.get());
				Weapon* weapon = new PistolWeapon(&resourceManager_);
				world2_->addEntity(enemy);
				enemy->setPosition(sfld::Vector2f((float)(rand() % (int)(world1_->getSize().x - enemy->getRadius())), -50.f));
				enemy->setWeapon(weapon);
			}

			if (stageTime_ >= 10000){
				walltimer += frameTime;
				if (walltimer >= wallthreshold){
					walltimer = 0;
					wallthreshold = 200 + rand() % 500;

					int worldn = rand() % 2;
					World* w;
					if (worldn == 0){
						w = world1_.get();
					}
					else{
						w = world2_.get();
					}

					PainfulBrick* brick = new PainfulBrick(&resourceManager_);
					w->addEntity(brick);
					brick->setPosition(sfld::Vector2f((float)(rand() % (int)(world1_->getSize().x - brick->getRadius())), -50.f));
				}
			}

			if (stageTime_ >= 20000){
				nextStage();
			}
		}
	}
	else if (stageNum_ == 6){
		if (stageTime_ >= 2000){
			smalltimer += frameTime;
			if (stageTime_ <= 10000){
				enemytimer += frameTime;
				walltimer += frameTime;
				World* w1;
				World* w2;
				if (currentleft){
					w1 = world1_.get();
					w2 = world2_.get();
				}
				else{
					w1 = world2_.get();
					w2 = world1_.get();
				}
				if (smalltimer >= 2000){
					currentleft = !currentleft;
				}
				if (enemytimer >= nextenemy){
					enemytimer = 0;
					nextenemy = 100 + rand() % 500;
					StandardEnemy* enemy = new StandardEnemy(&resourceManager_, "standardEnemy", particleEngine_.get());
					Weapon* weapon = new PistolWeapon(&resourceManager_);
					w1->addEntity(enemy);
					enemy->setPosition(sfld::Vector2f((float)(rand() % (int)(world1_->getSize().x - enemy->getRadius())), -50.f));
					enemy->setWeapon(weapon);
				}
				if (walltimer >= wallthreshold){
					walltimer = 0;
					wallthreshold = 500 % rand() % 700;

					PainfulBrick* brick = new PainfulBrick(&resourceManager_);
					w2->addEntity(brick);
					brick->setPosition(sfld::Vector2f((float)(rand() % (int)(world1_->getSize().x - brick->getRadius())), -50.f));
				}
			}
			else if (stageTime_ <= 20000){
				enemytimer += frameTime;
				smalltimer += frameTime;

				if (smalltimer >= nextthing){
					SpeedChange* change = new SpeedChange(&resourceManager_, 0.5f, "speedslow", 5000);
					smalltimer = 0;
					nextthing = 2000 + rand() % 2000;

					int worldn = rand() % 2;
					World* w;
					if (worldn == 0){
						w = world1_.get();
					}
					else{
						w = world2_.get();
					}
					w->addEntity(change);
					change->setPosition(sfld::Vector2f((float)(rand() % (int)(world1_->getSize().x - change->getRadius())), -50.f));
				}
				if (stageTime_ >= 13000){
					if (enemytimer >= nextenemy){
						enemytimer = 0;
						nextenemy = 300 + rand() % 300;

						int worldn = rand() % 2;
						World* w;
						if (worldn == 0){
							w = world1_.get();
						}
						else{
							w = world2_.get();
						}

						StandardEnemy* enemy = new StandardEnemy(&resourceManager_, "burstenemy", particleEngine_.get());
						Weapon* weapon = new BurstWeapon(&resourceManager_);
						w->addEntity(enemy);
						enemy->setPosition(sfld::Vector2f((float)(rand() % (int)(world1_->getSize().x - enemy->getRadius())), -50.f));
						enemy->setWeapon(weapon);
					}
				}
			}
			else if (stageTime_ < 28000){
				if (!sped){
					world1_->clear();
					world2_->clear();
					SpeedChange* speed = new SpeedChange(&resourceManager_, 2.5f, "speedboost", 9000);
					world1_->addEntity(speed);
					speed->setPosition(player_->getPosition());
					sped = true;
					world1_->flipView();
					world2_->flipView();
				}

				smalltimer += frameTime;
				if (smalltimer >= nextthing){
					smalltimer = 0;
					nextthing = 150 + rand() % 350;
					
					int n = rand() % 2;
					int worldn = rand() % 2;
					World* w;
					if (worldn == 0){
						w = world1_.get();
					}
					else{
						w = world2_.get();
					}

					if (n == 0){
						int blockn = 3 + rand() % 4;
						int randx = rand() % (int)w->getSize().x;
						spawnWall(blockn, randx, w);
					}
					else{
						PainfulBrick* brick = new PainfulBrick(&resourceManager_);
						w->addEntity(brick);
						brick->setPosition(sfld::Vector2f((float)(rand() % (int)(world1_->getSize().x - brick->getRadius())), -50.f));
					}
				}

			}
			else if (stageTime_ < 40000){
				//random mess fun yay

				walltimer += frameTime;
				smalltimer += frameTime;
				enemytimer += frameTime;

				if (walltimer >= wallthreshold){
					walltimer = 0;
					wallthreshold = 500 + rand() % 500;
					int worldn = rand() % 2;
					World* w;
					if (worldn == 0){
						w = world1_.get();
					}
					else{
						w = world2_.get();
					}
					PainfulBrick* brick = new PainfulBrick(&resourceManager_);
					w->addEntity(brick);
					brick->setPosition(sfld::Vector2f((float)(rand() % (int)(world1_->getSize().x - brick->getRadius())), -50.f));
				}
				if (smalltimer >= nextthing){
					smalltimer = 0;
					nextthing = 1000 + rand() % 2000;

					int worldn = rand() % 2;
					World* w;
					if (worldn == 0){
						w = world1_.get();
					}
					else{
						w = world2_.get();
					}

					int n = rand() % 2;
					if (n == 0){
						SpeedChange* change = new SpeedChange(&resourceManager_, 2.f, "speedboost", 3000);
						w->addEntity(change);
						change->setPosition(sfld::Vector2f((float)(rand() % (int)(world1_->getSize().x - change->getRadius())), -50.f));
					}
					else{
						FlipWorld* flip = new FlipWorld(&resourceManager_);
						w->addEntity(flip);
						flip->setPosition(sfld::Vector2f((float)(rand() % (int)(world1_->getSize().x - flip->getRadius())), -50.f));
					}
				}

				if (enemytimer >= nextenemy){
					nextenemy = 500 + rand() % 1000;
					enemytimer = 0;
					int worldn = rand() % 2;
					World* w;
					if (worldn == 0){
						w = world1_.get();
					}
					else{
						w = world2_.get();
					}
					int n = rand() % 2;

					Weapon* weapon;
					std::string tex;
					if (n == 0){
						weapon = new PistolWeapon(&resourceManager_);
						tex = "standardEnemy";
					}
					else{
						weapon = new BurstWeapon(&resourceManager_);
						tex = "burstenemy";
					}

					StandardEnemy* enemy = new StandardEnemy(&resourceManager_, tex, particleEngine_.get());
					w->addEntity(enemy);
					enemy->setPosition(sfld::Vector2f((float)(rand() % (int)(world1_->getSize().x - enemy->getRadius())), -50.f));
					enemy->setWeapon(weapon);
				}
				//END!
			}
			else{
				nextStage();
			}

		}
	}
}


void GameState::spawnWall(int blockn, float startx, World* world){
	for (int i = 0; i < blockn; i++){
		PainfulBrick* brick = new PainfulBrick(&resourceManager_);
		world->addEntity(brick);
		brick->setPosition(sfld::Vector2f(startx + i*brick->getRadius()*2, -50));
	}
}

void GameState::render(sf::RenderTarget* target){
	
	target->setView(*world1_->getView());
	world1_->render(target);
	if (!dead){
		player_->render(target);
	}
	particleEngine_->renderParticles(target);

	target->setView(*world2_->getView());
	world2_->render(target);
	if (!dead){
		player_->render(target);
	}
	particleEngine_->renderParticles(target);

	target->setView(target->getDefaultView());

	if (!dead){
		healthText.setString("Health: " + std::to_string(player_->getHealth()));
		target->draw(healthText);
	}
	else{
		target->draw(deadScreen);
	}

	
	stageText.setString("Stage: " + std::to_string(stageNum_));
	target->draw(stageText);

	if (messageDisplayed){
		target->draw(messageText);
	}
}