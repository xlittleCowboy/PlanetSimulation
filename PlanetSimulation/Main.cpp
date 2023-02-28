#pragma once

#include <SFML/Graphics.hpp>

#include "Planet.h"
#include "PlanetHandle.h"

/*
    SPAWN NEW PLANETS WITH LMB
    HOLD LMB AND DRAG IT IN THE DIRECTION YOU WANT NEW PLANET TO MOVE
    USE SCROLLWHEEL TO CHANGE RADIUS & MASS OF NEW PLANET
*/

int main()
{
    // Window setup
    const int WindowWidth = 800;
    const int WindowHeight = 800;

    sf::RenderWindow Window(sf::VideoMode(WindowWidth, WindowHeight), "Planet Simulation", sf::Style::Close);

    PlanetHandle* MainPlanetHandle = new PlanetHandle(false, false); // Enable/disable hit sound & gravity force

    sf::Clock DeltaClock;

    // Main lopp
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

            MainPlanetHandle->EventsHandle(Window, Event);
     
        }

        // Draw & Display
        Window.clear();

        float DeltaTime = DeltaClock.restart().asSeconds();

        MainPlanetHandle->Update(Window, sf::Vector2f(WindowWidth, WindowHeight), Event, DeltaTime);

        Window.display();
    }

    delete MainPlanetHandle;

    return 0;
}