#include "Planet.h"
#include <iostream>

Planet::Planet(float Mass, float Radius, sf::Vector2f StartPosition, sf::Vector2f StartDirection, sf::Color Color)
{
	this->Mass = Mass;
	this->Velocity = StartDirection;

	Shape.setRadius(Radius);
	Shape.setFillColor(Color);
	Shape.setPosition(StartPosition - sf::Vector2f(1.0f, 1.0f) * Radius);
}

void Planet::Update(sf::RenderWindow& Window, const sf::Vector2f& BorderSize, std::vector<Planet*> Planets, float DeltaTime)
{
	BorderCollisionCheck(BorderSize);
	PlanetsCollisionCheck(Planets);
	//CalculateGravityForces(Planets);

	Shape.move(Velocity * DeltaTime);

	Window.draw(Shape);
}

void Planet::SetVelocity(const sf::Vector2f& Velocity)
{
	this->Velocity = Velocity;
}

const sf::Vector2f& Planet::GetNormalizedVector(const sf::Vector2f& Vector)
{
	return Vector / (sqrt(Vector.x * Vector.x + Vector.y * Vector.y));
}

void Planet::BorderCollisionCheck(const sf::Vector2f& BorderSize)
{
	if (Shape.getPosition().x <= 0 || Shape.getPosition().x >= BorderSize.x - Shape.getRadius() * 2)
	{
		Velocity.x *= -1;
	}
	if (Shape.getPosition().y <= 0 || Shape.getPosition().y >= BorderSize.y - Shape.getRadius() * 2)
	{
		Velocity.y *= -1;
	}
}

void Planet::PlanetsCollisionCheck(std::vector<Planet*> Planets)
{
	if (Planets.size() <= 1) return;

	for (auto OtherPlanet = Planets.begin(); OtherPlanet != Planets.end(); ++OtherPlanet)
	{
		if (*OtherPlanet == this) continue;

		if (GetDistance(*OtherPlanet) <= (Shape.getRadius() + (*OtherPlanet)->GetShape().getRadius()))
		{
			sf::Vector2f OtherPlanetVelocity = (*OtherPlanet)->GetVelocity();

			sf::Vector2f DeltaVelocity = Velocity - OtherPlanetVelocity;

			float OtherPlanetMass = (*OtherPlanet)->GetMass();

			sf::Vector2f ThisPlanetPosition = Shape.getPosition() + sf::Vector2f(1.0f, 1.0f) * Shape.getRadius();
			sf::Vector2f OtherPlanetPosition = (*OtherPlanet)->GetShape().getPosition()
				+ sf::Vector2f(1.0f, 1.0f) * (*OtherPlanet)->GetShape().getRadius();

			sf::Vector2f DeltaPosition = ThisPlanetPosition - OtherPlanetPosition;
			float DistanceDeltaPosition = sqrt(DeltaPosition.x * DeltaPosition.x + DeltaPosition.y * DeltaPosition.y);

			float VelocityAndPositionProduct = DeltaVelocity.x * DeltaPosition.x + DeltaVelocity.y * DeltaPosition.y;

			Velocity -= ((2 * OtherPlanetMass * VelocityAndPositionProduct * DeltaPosition) /
				((Mass + OtherPlanetMass) * DistanceDeltaPosition * DistanceDeltaPosition));

			DeltaVelocity *= -1.0f;
			DeltaPosition *= -1.0f;

			DistanceDeltaPosition = sqrt(DeltaPosition.x * DeltaPosition.x + DeltaPosition.y * DeltaPosition.y);
			VelocityAndPositionProduct = DeltaVelocity.x * DeltaPosition.x + DeltaVelocity.y * DeltaPosition.y;

			(*OtherPlanet)->SetVelocity(OtherPlanetVelocity - ((2 * Mass * VelocityAndPositionProduct * DeltaPosition) /
				((Mass + OtherPlanetMass) * DistanceDeltaPosition * DistanceDeltaPosition)));
		}
		else
		{
			CalculateGravityForces(Planets);
		}
	}
}

void Planet::CalculateGravityForces(std::vector<Planet*> Planets)
{
	if (Planets.size() <= 1) return;

	for (auto OtherPlanet = Planets.begin(); OtherPlanet != Planets.end(); ++OtherPlanet)
	{
		if (*OtherPlanet == this) continue;

		sf::CircleShape OtherPlanerShape = (*OtherPlanet)->GetShape();

		float X = OtherPlanerShape.getPosition().x + OtherPlanerShape.getRadius() - Shape.getPosition().x - Shape.getRadius();
		float Y = OtherPlanerShape.getPosition().y + OtherPlanerShape.getRadius() - Shape.getPosition().y - Shape.getRadius();

		float Distance = GetDistance(*OtherPlanet);

		sf::Vector2f Force(X / Distance, Y / Distance);
		Force *= (float)((*OtherPlanet)->GetMass() * Mass / pow(Distance, 2));

		Velocity += Force;
	}
}

float Planet::GetDistance(const Planet* OtherPlanet)
{
	sf::CircleShape OtherPlanetShape = OtherPlanet->GetShape();

	float DirectionX = OtherPlanetShape.getPosition().x + OtherPlanetShape.getRadius() - Shape.getPosition().x - Shape.getRadius();
	float DirectionY = OtherPlanetShape.getPosition().y + OtherPlanetShape.getRadius() - Shape.getPosition().y - Shape.getRadius();

	float Distance = sqrt(pow(DirectionX, 2) + pow(DirectionY, 2));
	if (Distance < 1.0f)
	{
		Distance = 1.0f;
	}

	return Distance;
}