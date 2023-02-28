#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>

class Planet
{
private:
	const float GravitationalConstant = pow(10, -2);

	float Mass;

	sf::CircleShape Shape;
	sf::Vector2f Velocity;

	sf::SoundBuffer HitSoundBuffer;
	sf::Sound HitSound;

	bool EnableGravityForce;

	void HandleBordersCollision(const sf::Vector2f& BorderSize);
	void HandlePlanetsCollision(std::vector<Planet*> Planets);
	void HandleGravityForces(std::vector<Planet*> Planets);

	float GetIntersectionWithAnotherPlanet(Planet* OtherPlanet);
	void KickOutOfAnotherPlanet(Planet* OtherPlanet);

	const sf::Vector2f& GetVelocityOnPlanetCollision(Planet* OtherPlanet);

public:
	Planet(float Mass, float Radius, sf::Vector2f StartPosition, sf::Vector2f StartDirection, sf::Color Color,
		bool PlayHitSound, bool EnableGravityForce);

	void Update(sf::RenderWindow& Window, const sf::Vector2f& BorderSize, std::vector<Planet*> Planets, float DeltaTime);

	const sf::Vector2f& GetVelocity() { return Velocity; }
	void SetVelocity(const sf::Vector2f& Velocity);

	float GetDistanceToPlanet(const Planet* OtherPlanet);

	const sf::Vector2f& GetCenterPosition();

	const sf::CircleShape& GetShape() const { return Shape; }

	float GetMass() { return Mass; }
};

