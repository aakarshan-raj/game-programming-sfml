#include <SFML/Graphics.hpp>
#include <iostream>

const int WINDOW_HEIGHT = 700;
const int WINDOW_WIDTH = 700;

const int SHAPE_ONE_HEIGHT = 100;
const int SHAPE_ONE_WIDTH = 100;

const int SHAPE_TWO_HEIGHT = 100;
const int SHAPE_TWO_WIDTH = 100;

int SHAPE_ONE_POSITION_X = 200;
int SHAPE_ONE_POSITION_Y = 100;

int SHAPE_TWO_POSITION_X = 500;
int SHAPE_TWO_POSITION_Y = 300;

int main()
{
    sf::RenderWindow window(sf::VideoMode(700, 700), "collision detection");

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
    rect1.setOutlineColor(sf::Color(255, 255, 255, 255));
    rect1.setOutlineThickness(1);

    sf::RectangleShape rect2;
    rect2.setSize(sf::Vector2f(SHAPE_TWO_WIDTH, SHAPE_TWO_HEIGHT));
    rect2.setOrigin(sf::Vector2f(SHAPE_TWO_WIDTH / 2, SHAPE_TWO_HEIGHT / 2));
    rect2.setPosition(SHAPE_TWO_POSITION_X, SHAPE_TWO_POSITION_Y);
    rect2.setFillColor(sf::Color(0, 0, 0, 0));
    rect2.setOutlineColor(sf::Color(255, 255, 255, 255));
    rect2.setOutlineThickness(1);

    shape_one.setPosition(SHAPE_ONE_POSITION_X, SHAPE_ONE_POSITION_Y);
    shape_two.setPosition(SHAPE_TWO_POSITION_X, SHAPE_TWO_POSITION_Y);

    bool draw_rect1 = false;
    bool draw_rect2 = false;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
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

        window.clear();

        window.draw(shape_one);
        window.draw(shape_two);
        if (draw_rect1)
            window.draw(rect1);

        if (draw_rect2)
            window.draw(rect2);
        window.display();
    }

    return 0;
}