#pragma once

#include <iostream>
#include <vector>
#include <ctime>

#include "Planet.h"

class PlanetHandle
{
private:
    sf::Clock DeltaClock;

	std::vector<Planet*> Planets;

    float StartRadius; 
    float StartMass;

    float RadiusToMassMultiplier;

    float RadiusIncreaser;
    float MassIncreaser;

    float MaxRadius;
    float MaxMass;

    float MinRadius;
    float MinMass;

    sf::Color StartColor;

    bool IsLMBPressed;
    sf::Vector2f StartPosition;
    sf::Vector2f StartVelocity;

    void ChangeRadius(float Delta);

    void DrawSampleShape(sf::RenderWindow& Window);

    void DrawDirectionShape(sf::RenderWindow& Window);

    sf::RectangleShape GetDirectionShape(const sf::Vector2f& StartPosition, const sf::Vector2f& Direction);

    float Clamp(float Value, float Min, float Max);

public:
    PlanetHandle();

    ~PlanetHandle();

    void Update(sf::RenderWindow& Window, const sf::Vector2f& BorderSize, const sf::Event& Event, float DeltaTime);

    void EventHandle(sf::RenderWindow& Window, const sf::Event& Event);

    void SpawnPlanet(const sf::Vector2i& MousePosition);
};

