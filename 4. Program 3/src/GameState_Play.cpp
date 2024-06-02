#include "GameState_Play.h"
#include "Common.h"
#include "Physics.h"
#include "Assets.h"
#include "GameEngine.h"
#include "Components.h"
#include <cmath>
#include <fstream>

GameState_Play::GameState_Play(GameEngine &game, const std::string &levelPath)
    : GameState(game), m_levelPath(levelPath)
{
    init(m_levelPath);
}

void GameState_Play::init(const std::string &levelPath)
{
    loadLevel(levelPath);

    spawnPlayer();
}

void GameState_Play::loadLevel(const std::string &filename)
{
    m_entityManager = EntityManager();

    std::ifstream file;
    file.open(filename);
    std::string word;
    std::string animation_name;
    float x;
    float y;
    while (file >> word)
    {
        if (word == "Dec")
        {
            file >> animation_name >> x >> y;
            auto entity = m_entityManager.addEntity("dec");
            entity->addComponent<CTransform>(Vec2(x, y));
            entity->addComponent<CAnimation>(m_game.getAssets().getAnimation(animation_name), true);
        }
        else if (word == "Tile")
        {
            file >> animation_name >> x >> y;
            auto entity = m_entityManager.addEntity(animation_name); // Diffrentiate between floor and brick
            entity->addComponent<CTransform>(Vec2(x, y));
            entity->addComponent<CAnimation>(m_game.getAssets().getAnimation(animation_name), true);
            entity->addComponent<CBoundingBox>(m_game.getAssets().getAnimation(animation_name).getSize());
        }
        else if (word == "Player")
        {
            file >> m_playerConfig.X >> m_playerConfig.Y >>
                m_playerConfig.CX >> m_playerConfig.CY >>
                m_playerConfig.SPEED >> m_playerConfig.MAXSPEED >>
                m_playerConfig.JUMP >> m_playerConfig.GRAVITY >> m_playerConfig.WEAPON;
        }
    }
}

void GameState_Play::spawnPlayer()
{
    // here is a sample player entity which you can use to construct other entities
    m_player = m_entityManager.addEntity("player");
    m_player->addComponent<CTransform>(Vec2(m_playerConfig.X, m_playerConfig.Y));
    m_player->getComponent<CTransform>()->prevPos = {m_playerConfig.X, m_playerConfig.Y};
    m_player->addComponent<CBoundingBox>(Vec2(m_playerConfig.CX, m_playerConfig.CY));
    m_player->addComponent<CAnimation>(m_game.getAssets().getAnimation("Stand"), true);
    m_player->addComponent<CInput>();
    m_player->addComponent<CState>("stand");
    m_player->addComponent<CGravity>(m_playerConfig.GRAVITY);
    m_player->getComponent<CTransform>()->speed.y = m_playerConfig.GRAVITY;
}

void GameState_Play::spawnBullet(std::shared_ptr<Entity> entity)
{
    auto bullet = m_entityManager.addEntity("bullet");
    float direction = 11.0f;
    if (m_player->getComponent<CTransform>()->scale.x < 0)
    {
        direction = -11.0f;
    }

    bullet->addComponent<CAnimation>(m_game.getAssets().getAnimation(m_playerConfig.WEAPON), true);
    bullet->addComponent<CTransform>(Vec2(entity->getComponent<CTransform>()->pos.x, entity->getComponent<CTransform>()->pos.y),
                                     Vec2(direction, 0.0f),
                                     Vec2(1.0f, 1.0f),
                                     1.0f);
    bullet->addComponent<CLifeSpan>(40);
    bullet->addComponent<CBoundingBox>(Vec2(m_game.getAssets().getAnimation(m_playerConfig.WEAPON).getSize().x,
                                            m_game.getAssets().getAnimation(m_playerConfig.WEAPON).getSize().y));
}

void GameState_Play::update()
{
    m_entityManager.update();
    sMovement();
    sLifespan();
    sCollision();
    sAnimation();
    sUserInput();
    sRender();
}

void GameState_Play::sMovement()
{
    int moveX = 0;
    int moveY = 0;

    if (m_player->getComponent<CInput>()->up == true)
    {
        moveY = 18;
        m_player->getComponent<CTransform>()->prevPos.y = m_player->getComponent<CTransform>()->pos.y;
    }
    else if (m_player->getComponent<CInput>()->left == true)
    {
        moveX = -m_playerConfig.SPEED;
        m_player->getComponent<CTransform>()->prevPos.x = m_player->getComponent<CTransform>()->pos.x;
    }
    else if (m_player->getComponent<CInput>()->right == true)
    {
        moveX = m_playerConfig.SPEED;
        m_player->getComponent<CTransform>()->prevPos.x = m_player->getComponent<CTransform>()->pos.x;
    }
    else if (m_player->getComponent<CInput>()->down == true)
    {
        moveY = -m_playerConfig.JUMP;
        m_player->getComponent<CTransform>()->prevPos.y = m_player->getComponent<CTransform>()->pos.y;
    }

    m_player->getComponent<CTransform>()->pos += {moveX, moveY};

    m_player->getComponent<CTransform>()->prevPos.y = m_player->getComponent<CTransform>()->pos.y;

    m_player->getComponent<CTransform>()->pos.y += -8.5; // Gravity

    for (auto &bullet : m_entityManager.getEntities("bullet"))
    {
        if (m_player->getComponent<CTransform>()->scale.x == 1)
        {
            bullet->getComponent<CTransform>()->pos.x += 20; // Bullet speed
        }
        else
        {
            bullet->getComponent<CTransform>()->pos.x -= 20; // Bullet speed
        }
    }
}

void GameState_Play::sLifespan()
{
    for (auto &e : m_entityManager.getEntities())
    {
        if (e->hasComponent<CLifeSpan>())
        {
            if (e->getComponent<CLifeSpan>()->lifespan < 0)
            {
                e->destroy();
            }
            else
            {
                e->getComponent<CLifeSpan>()->lifespan -= 1;
            }
        }
    }
}

void GameState_Play::sCollision()
{

    for (auto const &f : m_entityManager.getEntities("bullet"))
    {
        for (auto const &e : m_entityManager.getEntities("Brick"))
        {
            Vec2 value = Physics::GetOverlap(e, f);
            if (value.x > 0 && value.y > 0)
            {
                e->addComponent<CLifeSpan>(7);
                f->destroy();
            }
        }
    }

    for (auto const &e : m_entityManager.getEntities("Ground"))
    {
        Vec2 value = Physics::GetOverlap(e, m_player);
        if (value.y > 0 && value.x > 0)
        {

            Vec2 prev_overlap = Physics::GetPrevOverlap(e, m_player);

            if (prev_overlap.x > 0)
            {
                if (m_player->getComponent<CTransform>()->pos.y > e->getComponent<CTransform>()->pos.y)
                {
                    m_player->getComponent<CTransform>()->pos.y += 8.5;
                }
                if (m_player->getComponent<CTransform>()->pos.y < e->getComponent<CTransform>()->pos.y) // shouldnt happend for ground
                    m_player->getComponent<CTransform>()->pos.y -= 8.5;
            }

            else if (prev_overlap.y > 0)
            {
                if (m_player->getComponent<CTransform>()->pos.x < e->getComponent<CTransform>()->pos.x)
                    m_player->getComponent<CTransform>()->pos.x -= 8.5;
                if (m_player->getComponent<CTransform>()->pos.x > e->getComponent<CTransform>()->pos.x)
                    m_player->getComponent<CTransform>()->pos.x += 8.5;
            }
        }
    }

    for (auto const &e : m_entityManager.getEntities("Brick"))
    {
        Vec2 value = Physics::GetOverlap(e, m_player);
        if (value.y > 0 && value.x > 0)
        {

            Vec2 prev_overlap = Physics::GetPrevOverlap(e, m_player);

            if (prev_overlap.x > 0)
            {
                if (m_player->getComponent<CTransform>()->pos.y > e->getComponent<CTransform>()->pos.y)
                {
                    m_player->getComponent<CTransform>()->pos.y += 8.5;
                }
                if (m_player->getComponent<CTransform>()->pos.y < e->getComponent<CTransform>()->pos.y)
                    m_player->getComponent<CTransform>()->pos.y -= 8.5;
            }

            else if (prev_overlap.y > 0)
            {
                if (m_player->getComponent<CTransform>()->pos.x < e->getComponent<CTransform>()->pos.x)
                    m_player->getComponent<CTransform>()->pos.x -= 8.5;
                if (m_player->getComponent<CTransform>()->pos.x > e->getComponent<CTransform>()->pos.x)
                    m_player->getComponent<CTransform>()->pos.x += 8.5;
            }
        }
    }

    for (auto const &e : m_entityManager.getEntities("Block"))
    {
        Vec2 value = Physics::GetOverlap(e, m_player);
        if (value.y > 0 && value.x > 0)
        {

            Vec2 prev_overlap = Physics::GetPrevOverlap(e, m_player);

            if (prev_overlap.x > 0)
            {
                if (m_player->getComponent<CTransform>()->pos.y > e->getComponent<CTransform>()->pos.y)
                {
                    m_player->getComponent<CTransform>()->pos.y += 8.5;
                }
                if (m_player->getComponent<CTransform>()->pos.y < e->getComponent<CTransform>()->pos.y)
                    m_player->getComponent<CTransform>()->pos.y -= 8.5;
            }

            else if (prev_overlap.y > 0)
            {
                if (m_player->getComponent<CTransform>()->pos.x < e->getComponent<CTransform>()->pos.x)
                    m_player->getComponent<CTransform>()->pos.x -= 8.5;
                if (m_player->getComponent<CTransform>()->pos.x > e->getComponent<CTransform>()->pos.x)
                    m_player->getComponent<CTransform>()->pos.x += 8.5;
            }
        }
    }
}

void GameState_Play::sUserInput()
{
    static bool up_pressed = false;
    static int loop = 0;

    sf::Event event;
    while (m_game.window().pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            m_game.quit();
        }

        if (!up_pressed)
        {

            if (event.type == sf::Event::KeyPressed)
            {
                switch (event.key.code)
                {
                case sf::Keyboard::Escape:
                {
                    m_game.popState();
                    break;
                }
                case sf::Keyboard::Z:
                {
                    init(m_levelPath);
                    break;
                }
                case sf::Keyboard::R:
                {
                    m_drawTextures = !m_drawTextures;
                    break;
                }
                case sf::Keyboard::F:
                {
                    m_drawCollision = !m_drawCollision;
                    break;
                }
                case sf::Keyboard::P:
                {
                    setPaused(!m_paused);
                    break;
                }
                case sf::Keyboard::W:
                {
                    m_player->getComponent<CInput>()->up = true;
                    m_player->getComponent<CState>()->state = "Air";
                    up_pressed = true;
                    break;
                }
                case sf::Keyboard::A:
                {
                    m_player->getComponent<CInput>()->left = true;
                    m_player->getComponent<CState>()->state = "Run";
                    m_player->getComponent<CTransform>()->scale = {-1.0f, 1};
                    break;
                }
                case sf::Keyboard::D:
                {
                    m_player->getComponent<CInput>()->right = true;
                    m_player->getComponent<CState>()->state = "Run";
                    m_player->getComponent<CTransform>()->scale = {1, 1};
                    break;
                }
                case sf::Keyboard::S:
                {
                    m_player->getComponent<CInput>()->down = true;
                    m_player->getComponent<CState>()->state = "Run";
                    m_player->getComponent<CTransform>()->scale = {1, 1};
                    break;
                }
                case sf::Keyboard::Space:
                {
                    spawnBullet(m_player);
                    break;
                }
                }
            }
        }

        if (event.type == sf::Event::KeyReleased)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::Escape:
            {
                break;
            }
            case sf::Keyboard::W:
            {
                m_player->getComponent<CInput>()->up = false;
                m_player->getComponent<CState>()->state = "Stand";
                break;
            }
            case sf::Keyboard::A:
            {
                m_player->getComponent<CInput>()->left = false;
                m_player->getComponent<CState>()->state = "Stand";
                break;
            }
            case sf::Keyboard::D:
            {
                m_player->getComponent<CInput>()->right = false;
                m_player->getComponent<CState>()->state = "Stand";
                break;
            }
            case sf::Keyboard::S:
            {
                m_player->getComponent<CInput>()->down = false;
                m_player->getComponent<CState>()->state = "Stand";
                break;
            }
            }
        }
    }
    if (up_pressed)
    {
        std::cout << "Loop is:" << loop << std::endl;
        if (loop < 20)
        {
            m_player->getComponent<CInput>()->up = true;
            m_player->getComponent<CState>()->state = "Air";
            loop++;
        }
        else
        {
            m_player->getComponent<CInput>()->up = false;
            m_player->getComponent<CState>()->state = "Stand";
            up_pressed = false;
            loop = 0;
        }
    }
}

void GameState_Play::sAnimation()
{

    if (m_player->getComponent<CState>()->state == m_player->getComponent<CAnimation>()->animation.getName())
    {
    }
    else
    {
        if (m_player->getComponent<CState>()->state == "Stand")
        {
            m_player->getComponent<CAnimation>()->animation = m_game.getAssets().getAnimation("Stand");
        }
        else if (m_player->getComponent<CState>()->state == "Air")
        {
            m_player->getComponent<CAnimation>()->animation = m_game.getAssets().getAnimation("Air");
        }
        else if (m_player->getComponent<CState>()->state == "Run")
        {
            m_player->getComponent<CAnimation>()->animation = m_game.getAssets().getAnimation("Run");
        }
    }

    for (auto &e : m_entityManager.getEntities("Brick"))
    {
        if (e->hasComponent<CLifeSpan>())
        {
            if (e->getComponent<CLifeSpan>()->lifespan == 5)
            {
                e->getComponent<CAnimation>()->animation = m_game.getAssets().getAnimation("Explosion");
            }
        }

        e->getComponent<CAnimation>()->animation.update();
    }

    for (auto &e : m_entityManager.getEntities())
    {

        e->getComponent<CAnimation>()->animation.update();
    }
}

// this function has been completed for you
void GameState_Play::sRender()
{
    // color the background darker so you know that the game is paused
    if (!m_paused)
    {
        m_game.window().clear(sf::Color(100, 100, 255));
    }
    else
    {
        m_game.window().clear(sf::Color(50, 50, 150));
    }

    // set the viewport of the window to be centered on the player if it's far enough right
    auto pPos = m_player->getComponent<CTransform>()->pos;
    float windowCenterX = fmax(m_game.window().getSize().x / 2.0f, pPos.x);
    sf::View view = m_game.window().getView();
    view.setCenter(windowCenterX, m_game.window().getSize().y - view.getCenter().y);
    m_game.window().setView(view);

    // draw all Entity textures / animations
    if (m_drawTextures)
    {
        for (auto e : m_entityManager.getEntities())
        {
            auto transform = e->getComponent<CTransform>();
            // transform->prevPos = transform->pos;
            // transform->pos.x = transform->pos.x + transform->speed.x;
            // transform->pos.y = transform->pos.y + transform->speed.y;

            if (e->hasComponent<CAnimation>())
            {
                auto animation = e->getComponent<CAnimation>()->animation;
                animation.getSprite().setRotation(transform->angle);
                animation.getSprite().setPosition(transform->pos.x, m_game.window().getSize().y - transform->pos.y); // Reverse SFML y-co-ordinate system
                animation.getSprite().setScale(transform->scale.x, transform->scale.y);
                m_game.window().draw(animation.getSprite());
            }
        }
    }

    // draw all Entity collision bounding boxes with a rectangleshape
    if (m_drawCollision)
    {
        for (auto e : m_entityManager.getEntities())
        {
            if (e->hasComponent<CBoundingBox>())
            {
                auto box = e->getComponent<CBoundingBox>();
                auto transform = e->getComponent<CTransform>();
                sf::RectangleShape rect;
                rect.setSize(sf::Vector2f(box->size.x - 1, box->size.y - 1));
                rect.setOrigin(sf::Vector2f(box->halfSize.x, box->halfSize.y));
                rect.setPosition(transform->pos.x, m_game.window().getSize().y - transform->pos.y);
                rect.setFillColor(sf::Color(0, 0, 0, 0));
                rect.setOutlineColor(sf::Color(255, 255, 255, 255));
                rect.setOutlineThickness(1);
                m_game.window().draw(rect);
            }
        }
    }

    m_game.window().display();
}
