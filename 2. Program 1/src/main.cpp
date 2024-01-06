#include <SFML/Graphics.hpp>
#include <iostream>

const int width = 1200;
const int height = 800;
const int FrameRate = 60;
const char *Title = "Bound It!";
const char *BottomText = "Sample Text";

int main()
{
    sf::RenderWindow window(sf::VideoMode(width, height), Title); // WINDOW
    window.setFramerateLimit(FrameRate);

    float radius = 50.0f; // CIRCLE
    sf::CircleShape circle(radius);
    circle.setPosition(500, 500);
    circle.setFillColor(sf::Color(100, 250, 50));
    int speed = 10;
    int vertical_motion = speed;
    int horizontal_motion = speed;

    sf::Font font; // TEXT
    if (!font.loadFromFile("font/tech.ttf"))
    {
        std::cout << "Error Loading Font" << std::endl;
        exit(-1);
    }
    sf::Text text(BottomText, font, 24);
    text.setFillColor(sf::Color::Red);
    text.setPosition(0, height - text.getCharacterSize());

    while (window.isOpen()) // GAME LOOP
    {
        sf::Event event;
        while (window.pollEvent(event)) // EVENT LOOP
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::W)
                {
                    vertical_motion = -speed;
                    std::cout << "UP PRESSED" << std::endl;
                }
                if (event.key.code == sf::Keyboard::S)
                {
                    vertical_motion = speed;

                    std::cout << "DOWN PRESSED" << std::endl;
                }
                if (event.key.code == sf::Keyboard::A)
                {
                    horizontal_motion = -speed;
                    std::cout << "LEFT PRESSED" << std::endl;
                }
                if (event.key.code == sf::Keyboard::D)
                {
                    horizontal_motion = speed;

                    std::cout << "RIGHT PRESSED" << std::endl;
                }
            }
        }
        window.clear();
        circle.setPosition(circle.getPosition().x + horizontal_motion, circle.getPosition().y + vertical_motion);
        window.draw(circle);
        window.draw(text);
        window.display();
    }
    return 0;
}