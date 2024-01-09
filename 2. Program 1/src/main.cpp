#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <memory>
#include <map>

const int width = 1200;
const int height = 800;
const int FrameRate = 1000;
const char *Title = "Bound It!";
const char *BottomText = "Sample Text";

void read_file(char *, std::map<std::string, std::pair<std::shared_ptr<sf::Shape>, std::pair<float, float>>> &,
               std::shared_ptr<sf::RenderWindow> &);

int main()
{
    std::shared_ptr<sf::RenderWindow> window = std::make_shared<sf::RenderWindow>();
    std::map<std::string, std::pair<std::shared_ptr<sf::Shape>, std::pair<float, float>>> Stuff;
    read_file("config/config", Stuff, window);

    window->setFramerateLimit(FrameRate);

    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window->close();
            }
        }

        window->clear();
        for (auto &[key, value] : Stuff)
        {
            window->draw(*(value.first));
            value.first->move(value.second.first, value.second.second);
            if ((value.first)->getPosition().x < 0)
            {
                value.second.first *= -1;
            }
            if ((value.first)->getPosition().y < 0)
            {
                value.second.second *= -1;
            }
            if ((value.first)->getPosition().x > window->getSize().x)
            {
                value.second.first *= -1;
            }
            if ((value.first)->getPosition().y > window->getSize().y)
            {
                value.second.second *= -1;
            }
        }

        window->display();
    }
    return 0;
}

void read_file(char *filepath, std::map<std::string, std::pair<std::shared_ptr<sf::Shape>, std::pair<float, float>>> &Stuff,
               std::shared_ptr<sf::RenderWindow> &window)
{
    std::ifstream file_handler;
    file_handler.open(filepath);
    std::string line;
    while (file_handler >> line)
    {
        if (line == "Window")
        {
            float width, height;
            file_handler >> width >> height;
            std::cout << "Window " << width << " " << height << std::endl;
            window->create(sf::VideoMode(width, height), "hi");
        }
        else if (line == "Font")
        {
            std::string font_path;
            float size;
            int R;
            int G;
            int B;
            file_handler >> font_path >> size >> R >> G >> B;
            std::cout << "Font " << font_path << " " << size << " " << R << " " << G << " " << B << std::endl;
        }
        else if (line == "Circle")
        {
            std::string name;
            float x, y, x_speed, y_speed, radius;
            int R, G, B;
            file_handler >> name >> x >> y >> x_speed >> y_speed >> R >> G >> B >> radius;
            std::cout << "Circle " << name << " " << x << " " << y << " " << x_speed << " "
                      << y_speed << " " << R << " " << G << " " << B << " " << radius << std::endl;
            std::shared_ptr<sf::Shape> c1 = std::make_shared<sf::CircleShape>(radius);
            c1->setPosition(x, y);
            c1->move(x_speed, y_speed);
            c1->setFillColor(sf::Color(R, G, B));
            Stuff.insert(std::make_pair(name, std::pair(c1, std::pair(x_speed, y_speed))));
        }
        else if (line == "Rectangle")
        {

            std::string name;
            float x, y, x_speed, y_speed, width, height;
            int R, G, B;
            file_handler >> name >> x >> y >> x_speed >> y_speed >> R >> G >> B >> width >> height;
            std::cout << "Rectangle" << name << " " << x << " " << y << " " << x_speed << " "
                      << y_speed << " " << R << " " << G << " " << B << " " << width << " " << height << std::endl;
            std::shared_ptr<sf::Shape> r1 = std::make_shared<sf::RectangleShape>(sf::Vector2f(width, height));
            r1->setPosition(x, y);
            r1->move(x_speed, y_speed);
            r1->setFillColor(sf::Color(R, G, B));
            Stuff.insert(std::make_pair(name, std::pair(r1, std::pair(x_speed, y_speed))));
        }
        else
        {
            std::cout << "Undefined Structure" << std::endl;
            exit(-1);
        }
    }
}