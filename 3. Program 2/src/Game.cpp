#include "Game.h"
#include <iostream>
#include <random>
#include <math.h>

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
		// Increment the current frame
		++m_currentFrame;
		if (m_currentFrame % 60 == 0)
		{
			sEnemySpawner();
		}
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
			m_player->cTransform->pos.y -= 10;
	}
	if (m_player->cInput->down == true)
	{
		if (m_player->cShape->circle.getPosition().y + m_player->cCollision->radius < 800)
			m_player->cTransform->pos.y += 10;
	}
	if (m_player->cInput->left == true)
	{
		if (m_player->cShape->circle.getPosition().x - m_player->cCollision->radius > 0)
			m_player->cTransform->pos.x -= 10;
	}
	if (m_player->cInput->right == true)
	{
		if (m_player->cShape->circle.getPosition().x + m_player->cCollision->radius < 1200)
			m_player->cTransform->pos.x += 10;
	}
	for (auto &x : m_entities.getEntities())
	{
		// Positions
		x->cTransform->pos.x += x->cTransform->velocity.x;
		x->cTransform->pos.y += x->cTransform->velocity.y;

		if (x->cTransform->pos.x + x->cCollision->radius > 1200 || x->cTransform->pos.x - x->cCollision->radius < 0)
			x->cTransform->velocity.x *= -1;

		if (x->cTransform->pos.y + x->cCollision->radius > 800 || x->cTransform->pos.y - x->cCollision->radius < 0)
			x->cTransform->velocity.y *= -1;

		x->cShape->circle.setPosition(x->cTransform->pos.x + x->cTransform->velocity.x,
									  x->cTransform->pos.y + x->cTransform->velocity.y);

		// Rotations
		x->cShape->circle.setRotation(m_player->cShape->circle.getRotation() + m_player->cTransform->angle);
	}
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
			if (event.key.code == sf::Keyboard::Q)
			{
				m_window.close();
			}
			if (event.key.code == sf::Keyboard::W)
			{
				m_player->cInput->up = true;
			}
			if (event.key.code == sf::Keyboard::S)
			{
				m_player->cInput->down = true;
			}
			if (event.key.code == sf::Keyboard::A)
			{
				m_player->cInput->left = true;
			}
			if (event.key.code == sf::Keyboard::D)
			{
				m_player->cInput->right = true;
			}
		}
		if (event.type == sf::Event::KeyReleased)
		{
			if (event.key.code == sf::Keyboard::W)
			{
				m_player->cInput->up = false;
			}
			if (event.key.code == sf::Keyboard::S)
			{
				m_player->cInput->down = false;
			}
			if (event.key.code == sf::Keyboard::A)
			{
				m_player->cInput->left = false;
			}
			if (event.key.code == sf::Keyboard::D)
			{
				m_player->cInput->right = false;
			}
		}
		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				std::cout << "(" << event.mouseButton.x << "," << event.mouseButton.y << ")" << std::endl;
				Vec2 pos(event.mouseButton.x, event.mouseButton.y);
				spawnBullet(m_player, pos);
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
	// we can do it using radius . calculate the length of radius of 2 objects, if its less than the distance between them then they are colliding
	// need nested loop:
	for (auto &x : m_entities.getEntities("Bullet"))
	{
		for (auto &y : m_entities.getEntities("Enemy"))
		{
				if (sqrt((x->cTransform->pos.x - y->cTransform->pos.x) * (x->cTransform->pos.x - y->cTransform->pos.x) + (x->cTransform->pos.y - y->cTransform->pos.y) * (x->cTransform->pos.y - y->cTransform->pos.y)) <=
					x->cCollision->radius + y->cCollision->radius)
				{
					x->destroy();
					y->destroy();
				}
		}
	}
}

void Game::spawnPlayer()
{

	m_player = m_entities.addEntity("Player");
	m_player->cTransform = std::make_shared<CTransform>(Vec2(400, 200), Vec2(0, 0), 3);
	m_player->cCollision = std::make_shared<CCollision>(100);
	m_player->cShape = std::make_shared<CShape>(m_player->cCollision->radius, 5, sf::Color(10, 10, 10), sf::Color(255, 0, 0), 3);
	m_player->cShape->circle.setPosition(m_player->cTransform->pos.x, m_player->cTransform->pos.y);
	m_player->cInput = std::make_shared<CInput>();
}
void Game::sEnemySpawner()
{
	spawnEnemy();
}
void Game::spawnEnemy()
{
	auto enemy = m_entities.addEntity("Enemy");

	std::random_device dev;
	std::mt19937 mt(dev());
	enemy->cCollision = std::make_shared<CCollision>(30);

	std::uniform_real_distribution<float> x(0 + enemy->cCollision->radius, 1200 - enemy->cCollision->radius);
	std::uniform_real_distribution<float> y(0 + enemy->cCollision->radius, 800 - enemy->cCollision->radius);

	std::uniform_int_distribution<int> colorx(0, 256);
	std::uniform_int_distribution<int> sizex(3, 8);

	enemy->cTransform = std::make_shared<CTransform>(Vec2(x(mt), y(mt)), Vec2(1, 1), 3);
	enemy->cShape = std::make_shared<CShape>(enemy->cCollision->radius, sizex(mt), sf::Color(colorx(mt), colorx(mt), colorx(mt)), sf::Color(colorx(mt), colorx(mt), colorx(mt)), 3);
	enemy->cShape->circle.setPosition(enemy->cTransform->pos.x, enemy->cTransform->pos.y);
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> parent)
{
}

// Spawns a bullet from a given entity to a target location
void Game::spawnBullet(std::shared_ptr<Entity> shooter, const Vec2 &mousePos)
{
	// calculate velocity, set transform, shape
	float a = mousePos.x - shooter->cTransform->pos.x;
	float b = mousePos.y - shooter->cTransform->pos.y;

	float hypo = sqrt(a * a + b * b);
	float x = a / hypo;
	float y = b / hypo;
	std::cout << "Unit vector:" << x << "," << y << std::endl;
	auto bullet = m_entities.addEntity("Bullet");
	bullet->cCollision = std::make_shared<CCollision>(20);
	bullet->cTransform = std::make_shared<CTransform>(Vec2(shooter->cTransform->pos.x,
														   shooter->cTransform->pos.y),
													  Vec2(x, y), 10);
	bullet->cLifespan = std::make_shared<CLifeSpan>(100);
	bullet->cShape = std::make_shared<CShape>(20, 10, sf::Color(100, 100, 100), sf::Color(200, 200, 200), 9);
	std::cout << "velocity:" << bullet->cTransform->velocity.x << ":" << bullet->cTransform->velocity.y << std::endl;
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
}
