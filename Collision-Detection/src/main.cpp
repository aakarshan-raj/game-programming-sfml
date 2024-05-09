#include <SFML/Graphics.hpp>
#include <iostream>

const int WINDOW_HEIGHT = 700;
const int WINDOW_WIDTH = 1000;

const int SHAPE_ONE_HEIGHT = 100;
const int SHAPE_ONE_WIDTH = 100;

const int SHAPE_TWO_HEIGHT = 100;
const int SHAPE_TWO_WIDTH = 100;

int SHAPE_ONE_POSITION_X = 200;
int SHAPE_ONE_POSITION_Y = 100;

int SHAPE_TWO_POSITION_X = 500;
int SHAPE_TWO_POSITION_Y = 300;

bool detect_collision(sf::RectangleShape *shape_one, sf::RectangleShape *shape_two)
{
    bool vertical_collision = (shape_one->getPosition().x - (shape_one->getSize().x / 2) < shape_two->getPosition().x + (shape_two->getSize().x / 2) &&
                               shape_two->getPosition().x - (shape_two->getSize().x / 2) < shape_one->getPosition().x + (shape_one->getSize().x / 2));
    bool horizontal_collision = (shape_one->getPosition().y - (shape_one->getSize().y / 2) < shape_two->getPosition().y + (shape_two->getSize().y / 2) &&
                                 shape_two->getPosition().y - (shape_two->getSize().y / 2) < shape_one->getPosition().y + (shape_one->getSize().y / 2));

    if (vertical_collision && horizontal_collision)
    {
        return true;
    }

    return false;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "collision detection");
    sf::Font font;
    if (!font.loadFromFile("font/Oxygen-Regular.ttf"))
    {
        std::cout << "Cant load Fonts" << std::endl;
    }
    sf::RectangleShape shape_one(sf::Vector2f(SHAPE_ONE_WIDTH, SHAPE_ONE_HEIGHT));
    sf::RectangleShape shape_two(sf::Vector2f(SHAPE_TWO_WIDTH, SHAPE_TWO_HEIGHT));

    shape_one.setOrigin(SHAPE_ONE_WIDTH / 2, SHAPE_ONE_HEIGHT / 2);
    shape_two.setOrigin(SHAPE_TWO_WIDTH / 2, SHAPE_TWO_HEIGHT / 2);

    shape_one.setFillColor(sf::Color::Green);
    shape_two.setFillColor(sf::Color::Red);

    sf::RectangleShape rect1;
    rect1.setSize(sf::Vector2f(SHAPE_ONE_WIDTH, SHAPE_ONE_HEIGHT));
    rect1.setOrigin(sf::Vector2f(SHAPE_ONE_WIDTH / 2, SHAPE_ONE_HEIGHT / 2));
    rect1.setPosition(SHAPE_ONE_POSITION_X, SHAPE_ONE_POSITION_Y);
    rect1.setFillColor(sf::Color(0, 0, 0, 0));
    rect1.setOutlineColor(sf::Color(0, 0, 0, 255));
    rect1.setOutlineThickness(1);

    sf::RectangleShape rect2;
    rect2.setSize(sf::Vector2f(SHAPE_TWO_WIDTH, SHAPE_TWO_HEIGHT));
    rect2.setOrigin(sf::Vector2f(SHAPE_TWO_WIDTH / 2, SHAPE_TWO_HEIGHT / 2));
    rect2.setPosition(SHAPE_TWO_POSITION_X, SHAPE_TWO_POSITION_Y);
    rect2.setFillColor(sf::Color(0, 0, 0, 0));
    rect2.setOutlineColor(sf::Color(0, 0, 0, 255));
    rect2.setOutlineThickness(1);

    // Status
    bool collision = false;
    sf::RectangleShape status;
    status.setSize(sf::Vector2f(200, WINDOW_HEIGHT));
    status.setPosition(WINDOW_WIDTH - 200, 0);
    status.setFillColor(sf::Color::Black);
    status.setOutlineColor(sf::Color(0, 0, 0, 255));
    status.setOutlineThickness(1);
    sf::Text collision_text("Collision: ", font, 15);
    collision_text.setPosition(sf::Vector2f(WINDOW_WIDTH - 190, 10));
    collision_text.setColor(sf::Color::Red);

    sf::Text collision_status_text("false", font, 15);
    collision_status_text.setPosition(sf::Vector2f(WINDOW_WIDTH - 120, 10));
    collision_status_text.setColor(sf::Color::Red);

    shape_one.setPosition(SHAPE_ONE_POSITION_X, SHAPE_ONE_POSITION_Y);
    shape_two.setPosition(SHAPE_TWO_POSITION_X, SHAPE_TWO_POSITION_Y);

    bool draw_rect1 = false;
    bool draw_rect2 = false;

    sf::Text shape_one_center_x(std::to_string(SHAPE_ONE_POSITION_X), font, 15);
    sf::Text shape_one_center_y(std::to_string(SHAPE_ONE_POSITION_Y), font, 15);

    shape_one_center_x.setPosition(sf::Vector2f(SHAPE_ONE_POSITION_X - 25, SHAPE_ONE_POSITION_Y - 10));
    shape_one_center_y.setPosition(sf::Vector2f(SHAPE_ONE_POSITION_X + 10, SHAPE_ONE_POSITION_Y - 10));

    sf::Text shape_two_center_x(std::to_string(SHAPE_TWO_POSITION_X), font, 15);
    sf::Text shape_two_center_y(std::to_string(SHAPE_TWO_POSITION_Y), font, 15);

    shape_two_center_x.setPosition(sf::Vector2f(SHAPE_TWO_POSITION_X - 25, SHAPE_TWO_POSITION_Y - 10));
    shape_two_center_y.setPosition(sf::Vector2f(SHAPE_TWO_POSITION_X + 10, SHAPE_TWO_POSITION_Y - 10));

    shape_one_center_x.setColor(sf::Color::Black);
    shape_one_center_y.setColor(sf::Color::Black);
    shape_two_center_x.setColor(sf::Color::Black);
    shape_two_center_y.setColor(sf::Color::Black);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                }
                if (draw_rect1)
                {
                    if (event.key.code == sf::Keyboard::W)
                    {
                        shape_one.setPosition(sf::Vector2f(shape_one.getPosition().x, shape_one.getPosition().y - 10));
                        rect1.setPosition(sf::Vector2f(rect1.getPosition().x, rect1.getPosition().y - 10));

                        shape_one_center_x.setPosition(sf::Vector2f(shape_one_center_x.getPosition().x, shape_one_center_x.getPosition().y - 10));
                        shape_one_center_y.setPosition(sf::Vector2f(shape_one_center_y.getPosition().x, shape_one_center_y.getPosition().y - 10));
                    }
                    if (event.key.code == sf::Keyboard::S)
                    {
                        shape_one.setPosition(sf::Vector2f(shape_one.getPosition().x, shape_one.getPosition().y + 10));
                        rect1.setPosition(sf::Vector2f(rect1.getPosition().x, rect1.getPosition().y + 10));

                        shape_one_center_x.setPosition(sf::Vector2f(shape_one_center_x.getPosition().x, shape_one_center_x.getPosition().y + 10));
                        shape_one_center_y.setPosition(sf::Vector2f(shape_one_center_y.getPosition().x, shape_one_center_y.getPosition().y + 10));
                    }
                    if (event.key.code == sf::Keyboard::A)
                    {
                        shape_one.setPosition(sf::Vector2f(shape_one.getPosition().x - 10, shape_one.getPosition().y));
                        rect1.setPosition(sf::Vector2f(rect1.getPosition().x - 10, rect1.getPosition().y));

                        shape_one_center_x.setPosition(sf::Vector2f(shape_one_center_x.getPosition().x - 10, shape_one_center_x.getPosition().y));
                        shape_one_center_y.setPosition(sf::Vector2f(shape_one_center_y.getPosition().x - 10, shape_one_center_y.getPosition().y));
                    }
                    if (event.key.code == sf::Keyboard::D)
                    {
                        shape_one.setPosition(sf::Vector2f(shape_one.getPosition().x + 10, shape_one.getPosition().y));
                        rect1.setPosition(sf::Vector2f(rect1.getPosition().x + 10, rect1.getPosition().y));

                        shape_one_center_x.setPosition(sf::Vector2f(shape_one_center_x.getPosition().x + 10, shape_one_center_x.getPosition().y));
                        shape_one_center_y.setPosition(sf::Vector2f(shape_one_center_y.getPosition().x + 10, shape_one_center_y.getPosition().y));
                    }
                    shape_one_center_x.setString(std::to_string((int)shape_one.getPosition().x));
                    shape_one_center_y.setString(std::to_string((int)shape_one.getPosition().y));
                }
                else if (draw_rect2)
                {
                    if (event.key.code == sf::Keyboard::W)
                    {
                        shape_two.setPosition(sf::Vector2f(shape_two.getPosition().x, shape_two.getPosition().y - 10));
                        rect2.setPosition(sf::Vector2f(rect2.getPosition().x, rect2.getPosition().y - 10));

                        shape_two_center_x.setPosition(sf::Vector2f(shape_two_center_x.getPosition().x, shape_two_center_x.getPosition().y - 10));
                        shape_two_center_y.setPosition(sf::Vector2f(shape_two_center_y.getPosition().x, shape_two_center_y.getPosition().y - 10));
                    }
                    if (event.key.code == sf::Keyboard::S)
                    {
                        shape_two.setPosition(sf::Vector2f(shape_two.getPosition().x, shape_two.getPosition().y + 10));
                        rect2.setPosition(sf::Vector2f(rect2.getPosition().x, rect2.getPosition().y + 10));

                        shape_two_center_x.setPosition(sf::Vector2f(shape_two_center_x.getPosition().x, shape_two_center_x.getPosition().y + 10));
                        shape_two_center_y.setPosition(sf::Vector2f(shape_two_center_y.getPosition().x, shape_two_center_y.getPosition().y + 10));
                    }
                    if (event.key.code == sf::Keyboard::A)
                    {
                        shape_two.setPosition(sf::Vector2f(shape_two.getPosition().x - 10, shape_two.getPosition().y));
                        rect2.setPosition(sf::Vector2f(rect2.getPosition().x - 10, rect2.getPosition().y));

                        shape_two_center_x.setPosition(sf::Vector2f(shape_two_center_x.getPosition().x - 10, shape_two_center_x.getPosition().y));
                        shape_two_center_y.setPosition(sf::Vector2f(shape_two_center_y.getPosition().x - 10, shape_two_center_y.getPosition().y));
                    }
                    if (event.key.code == sf::Keyboard::D)
                    {
                        shape_two.setPosition(sf::Vector2f(shape_two.getPosition().x + 10, shape_two.getPosition().y));
                        rect2.setPosition(sf::Vector2f(rect2.getPosition().x + 10, rect2.getPosition().y));

                        shape_two_center_x.setPosition(sf::Vector2f(shape_two_center_x.getPosition().x + 10, shape_two_center_x.getPosition().y));
                        shape_two_center_y.setPosition(sf::Vector2f(shape_two_center_y.getPosition().x + 10, shape_two_center_y.getPosition().y));
                    }
                    shape_two_center_x.setString(std::to_string((int)shape_two.getPosition().x));
                    shape_two_center_y.setString(std::to_string((int)shape_two.getPosition().y));
                }
            }
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    if (shape_one.getPosition().x - (SHAPE_ONE_WIDTH / 2) < event.mouseButton.x &&
                        shape_one.getPosition().x + (SHAPE_ONE_WIDTH / 2) > event.mouseButton.x &&
                        shape_one.getPosition().y - (SHAPE_ONE_HEIGHT / 2) < event.mouseButton.y &&
                        shape_one.getPosition().y + (SHAPE_ONE_HEIGHT / 2) > event.mouseButton.y)
                    {
                        std::cout << "Shape one clicked" << std::endl;
                        draw_rect1 = true;
                        draw_rect2 = false;
                    }

                    if (shape_two.getPosition().x - (SHAPE_TWO_WIDTH / 2) < event.mouseButton.x &&
                        shape_two.getPosition().x + (SHAPE_TWO_WIDTH / 2) > event.mouseButton.x &&
                        shape_two.getPosition().y - (SHAPE_TWO_HEIGHT / 2) < event.mouseButton.y &&
                        shape_two.getPosition().y + (SHAPE_TWO_HEIGHT / 2) > event.mouseButton.y)
                    {
                        std::cout << "Shape two clicked" << std::endl;
                        draw_rect2 = true;
                        draw_rect1 = false;
                    }
                    if (draw_rect1)
                    {
                        if (!(shape_one.getPosition().x - (SHAPE_ONE_WIDTH / 2) < event.mouseButton.x &&
                              shape_one.getPosition().x + (SHAPE_ONE_WIDTH / 2) > event.mouseButton.x &&
                              shape_one.getPosition().y - (SHAPE_ONE_HEIGHT / 2) < event.mouseButton.y &&
                              shape_one.getPosition().y + (SHAPE_ONE_HEIGHT / 2) > event.mouseButton.y))
                        {
                            draw_rect1 = false;
                        }
                    }
                    if (draw_rect2)
                    {
                        if (!((shape_two.getPosition().x - (SHAPE_TWO_WIDTH / 2) < event.mouseButton.x &&
                               shape_two.getPosition().x + (SHAPE_TWO_WIDTH / 2) > event.mouseButton.x &&
                               shape_two.getPosition().y - (SHAPE_TWO_HEIGHT / 2) < event.mouseButton.y &&
                               shape_two.getPosition().y + (SHAPE_TWO_HEIGHT / 2) > event.mouseButton.y)))
                        {
                            draw_rect2 = false;
                        }
                    }
                }
            }
        }

        // Collision Detection

        if (detect_collision(&shape_one, &shape_two))
        {
            std::cout << "COLLISIOn" << std::endl;
            collision_status_text.setString("True");
            collision_status_text.setColor(sf::Color::Green);
        }
        else
        {
            std::cout << " NO COLLISIOn" << std::endl;
            collision_status_text.setString("False");
            collision_status_text.setColor(sf::Color::Red);
        }

        window.clear(sf::Color::White);

        window.draw(shape_one);
        window.draw(shape_two);

        // Rendering texts
        window.draw(shape_one_center_x);
        window.draw(shape_one_center_y);
        window.draw(shape_two_center_x);
        window.draw(shape_two_center_y);

        window.draw(status);
        window.draw(collision_text);
        window.draw(collision_status_text);

        if (draw_rect1)
            window.draw(rect1);

        if (draw_rect2)
            window.draw(rect2);
        window.display();
    }

    return 0;
}