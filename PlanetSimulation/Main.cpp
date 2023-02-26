#pragma once

#include <SFML/Graphics.hpp>

#include "Planet.h"
#include "PlanetHandle.h"

int main()
{
    const int WindowWidth = 800;
    const int WindowHeight = 800;

    sf::RenderWindow Window(sf::VideoMode(WindowWidth, WindowHeight), "Planet Simulation", sf::Style::Close);
    Window.setVerticalSyncEnabled(true);
   // Window.setFramerateLimit(60);

    PlanetHandle* MainPlanetHandle = new PlanetHandle();

    sf::Clock DeltaClock;

    sf::Font FPSFont;
    sf::Text FPSText;
    
    sf::Clock FPSClock;
    FPSFont.loadFromFile("arial.ttf");
    FPSText.setFont(FPSFont);
    FPSText.setFillColor(sf::Color::White);
    FPSText.setCharacterSize(24);
    FPSText.setPosition(sf::Vector2f(WindowWidth / 2, 0));

    while (Window.isOpen())
    {
        sf::Event Event;
        while (Window.pollEvent(Event))
        {
            // Close Events
            if (Event.type == sf::Event::Closed)
            {
                Window.close();
            }
            if (Event.type == sf::Event::KeyPressed)
            {
                if (Event.key.code == sf::Keyboard::Escape)
                {
                    Window.close();
                }
            }

            MainPlanetHandle->EventHandle(Window, Event);
     
        }

        Window.clear();

        float DeltaTime = DeltaClock.restart().asSeconds();

        MainPlanetHandle->Update(Window, sf::Vector2f(WindowWidth, WindowHeight), Event, DeltaTime);

        if (FPSClock.getElapsedTime().asSeconds() >= 0.5f)
        {
            FPSText.setString("FPS:" + std::to_string(1 / DeltaTime));

            FPSClock.restart();
        }

        Window.draw(FPSText);

        Window.display();
    }

    delete MainPlanetHandle;

    return 0;
}