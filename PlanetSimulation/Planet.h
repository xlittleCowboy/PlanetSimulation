#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

class Planet
{
private:
	float Mass;

	sf::CircleShape Shape;
	sf::Vector2f Velocity;

	float GetDistance(const Planet* OtherPlanet);
	void CalculateGravityForces(std::vector<Planet*> Planets);

	void BorderCollisionCheck(const sf::Vector2f& BorderSize);
	void PlanetsCollisionCheck(std::vector<Planet*> Planets);

public:
	Planet(float Mass, float Radius, sf::Vector2f StartPosition, sf::Vector2f StartDirection, sf::Color Color);

	void Update(sf::RenderWindow& Window, const sf::Vector2f& BorderSize, std::vector<Planet*> Planets, float DeltaTime);

	const sf::CircleShape& GetShape() const { return Shape; }
	const sf::Vector2f& GetVelocity() { return Velocity; }
	float GetMass() { return Mass; }

	void SetVelocity(const sf::Vector2f& Velocity);
	const sf::Vector2f& GetNormalizedVector(const sf::Vector2f& Vector);
};

