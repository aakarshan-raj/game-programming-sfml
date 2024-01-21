#include "Game.h"
#include <iostream>
#include <random>
Game::Game(const std::string &configFile)
{
	init(configFile);
}

void Game::run()
{
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
		spawnEnemy();
		// Increment the current frame
		++m_currentFrame;
	}
}

void Game::init(const std::string &config)
{
	m_window.create(sf::VideoMode(1200, 800), "hi");
	m_window.setFramerateLimit(60);
	spawnPlayer();
	spawnEnemy();
}

void Game::setPaused(bool paused)
{
}

void Game::sMovement()
{

	if (m_player->cInput->up == true)
	{
		if (m_player->cShape->circle.getPosition().y - m_player->cCollision->radius > 0)
			m_player->cShape->circle.move(0, -10);
	}
	if (m_player->cInput->down == true)
	{
		if (m_player->cShape->circle.getPosition().y + m_player->cCollision->radius < 800)
			m_player->cShape->circle.move(0, 10);
	}
	if (m_player->cInput->left == true)
	{
		if (m_player->cShape->circle.getPosition().x - m_player->cCollision->radius > 0)
			m_player->cShape->circle.move(-10, 0);
	}
	if (m_player->cInput->right == true)
	{
		if (m_player->cShape->circle.getPosition().x + m_player->cCollision->radius < 1200)
			m_player->cShape->circle.move(10, 0);
	}
	m_player->cShape->circle.setRotation(m_player->cShape->circle.getRotation() + 10);
}

void Game::sUserInput()
{
	// handle all user input, close, pause, up,down,left,right, mouse click, only set state here. No movements here
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_window.close();
		}
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::W)
			{
				std::cout << "W pressed" << std::endl;
				m_player->cInput->up = true;
			}
			if (event.key.code == sf::Keyboard::S)
			{
				std::cout << "S pressed" << std::endl;
				m_player->cInput->down = true;
			}
			if (event.key.code == sf::Keyboard::A)
			{
				std::cout << "A pressed" << std::endl;
				m_player->cInput->left = true;
			}
			if (event.key.code == sf::Keyboard::D)
			{
				std::cout << "D pressed" << std::endl;
				m_player->cInput->right = true;
			}
		}
		if (event.type == sf::Event::KeyReleased)
		{
			if (event.key.code == sf::Keyboard::W)
			{
				std::cout << "W released" << std::endl;

				m_player->cInput->up = false;
			}
			if (event.key.code == sf::Keyboard::S)
			{
				std::cout << "S released" << std::endl;

				m_player->cInput->down = false;
			}
			if (event.key.code == sf::Keyboard::A)
			{
				std::cout << "A released" << std::endl;

				m_player->cInput->left = false;
			}
			if (event.key.code == sf::Keyboard::D)
			{
				std::cout << "D released" << std::endl;

				m_player->cInput->right = false;
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
	//  Logic to change position for the enemies
	// Draw
	// Display
	m_window.clear();
	for (const std::shared_ptr<Entity> &x : m_entities.getEntities())
	{
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
	m_player->cTransform = std::make_shared<CTransform>(Vec2(400, 200), Vec2(0, 0), 0);
	m_player->cCollision = std::make_shared<CCollision>(100);
	m_player->cShape = std::make_shared<CShape>(m_player->cCollision->radius, 5, sf::Color(10, 10, 10), sf::Color(255, 0, 0), 3);
	m_player->cShape->circle.setPosition(m_player->cTransform->pos.x, m_player->cTransform->pos.y);
	m_player->cInput = std::make_shared<CInput>();
}
void Game::sEnemySpawner()
{
	// call spawnEnemy here
}
void Game::spawnEnemy()
{
	std::random_device dev;
	std::mt19937 mt(dev());
	std::uniform_real_distribution<float> x(0, 1200);
	std::uniform_real_distribution<float> y(0, 800);
    
	auto enemy = m_entities.addEntity("Enemy");
	enemy->cTransform = std::make_shared<CTransform>(Vec2(x(mt), y(mt)), Vec2(0, 0), 0);
	enemy->cCollision = std::make_shared<CCollision>(100);
	enemy->cShape = std::make_shared<CShape>(m_player->cCollision->radius, 3, sf::Color(10, 10, 10), sf::Color(255, 0, 0), 3);
	enemy->cShape->circle.setPosition(enemy->cTransform->pos.x, enemy->cTransform->pos.y);
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