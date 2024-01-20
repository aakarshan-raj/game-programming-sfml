#include "Game.h"

Game::Game(const std::string &configFile)
{
	init(configFile);
}

void Game::run()
{
	spawnPlayer();
	while (m_running)
	{
		m_entities.update();

		if (!m_paused)
		{
			sLifeSpan();
			sMovement();
			sCollision();
		}
		sUserInput();
		sRender();

		// Increment the current frame
		++m_currentFrame;
	}
}

void Game::init(const std::string &config)
{
	 m_window.create(sf::VideoMode(1200,800),"hi");
	 
}

void Game::setPaused(bool paused)
{
}

void Game::sMovement()
{
	// All movements here
}

void Game::sUserInput()
{
	// handle all user input, close, pause, up,down,left,right, mouse click, only set state here. No movements here
	sf::Event event;
	while(m_window.pollEvent(event)){
		if(event.type == sf::Event::Closed){
				m_window.close();			
		}
		if(event.type == sf::Event::KeyPressed){
			if(event.key.code == sf::Keyboard::Q){
				m_window.close();
			}
		}
	}
}

void Game::sLifeSpan()
{
}

void Game::sRender()
{
	// Clear
	//  Logic to change position
	// Draw
	// Display
	m_window.clear();
	for(const std::shared_ptr<Entity> &x:m_entities.getEntities()){
	m_window.draw(x->cShape->circle);
	}
	m_window.display();

}

void Game::sCollision()
{
	// check for collisions, call other functions
}

void Game::spawnPlayer()
{

	m_player = m_entities.addEntity("Player");
	m_player->cShape =  std::make_shared<CShape>(100,50,sf::Color(100,200,255),sf::Color(100,200,255),3);
	m_player->cShape->circle.setPosition(400,250);
	
}
void Game::sEnemySpawner()
{
	// call spawnEnemy here
}
void Game::spawnEnemy()
{
	// spawn enemy
	// take account of the last frame of spanwed enemy, read doc for reason
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> parent)
{
}

// Spawns a bullet from a given entity to a target location
void Game::spawnBullet(std::shared_ptr<Entity> shooter, const Vec2 &mousePos)
{
	// calculate velocity, set transform, shape
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
}