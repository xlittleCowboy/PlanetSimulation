#pragma once

#include <SFML/Graphics.hpp>

#include "Planet.h"
#include "PlanetHandle.h"

int main()
{
    const int WindowWidth = 800;
    const int WindowHeight = 800;

    sf::RenderWindow Window(sf::VideoMode(WindowWidth, WindowHeight), "Planet Simulation", sf::Style::Close);
    Window.setVerticalSyncEnabled(false);
    Window.setFramerateLimit(60);

    PlanetHandle* MainPlanetHandle = new PlanetHandle();

    sf::Clock DeltaClock;

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

        float Deltatime = DeltaClock.restart().asSeconds();

        MainPlanetHandle->Update(Window, sf::Vector2f(WindowWidth, WindowHeight), Event, Deltatime);

        Window.display();
    }

    delete MainPlanetHandle;

    return 0;
}