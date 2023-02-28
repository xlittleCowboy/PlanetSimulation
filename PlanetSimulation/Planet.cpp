#include "Planet.h"
#include <iostream>

Planet::Planet(float Mass, float Radius, sf::Vector2f StartPosition, sf::Vector2f StartDirection, sf::Color Color,
	bool PlayHitSound, bool EnableGravityForce)
{
	this->Mass = Mass;
	this->Velocity = StartDirection;

	if (PlayHitSound)
	{
		HitSoundBuffer.loadFromFile("Hit.wav");
		HitSound.setBuffer(HitSoundBuffer);
	}

	this->EnableGravityForce = EnableGravityForce;

	Shape.setRadius(Radius);
	Shape.setFillColor(Color);
	Shape.setPosition(StartPosition - sf::Vector2f(1.0f, 1.0f) * Radius);
}

void Planet::Update(sf::RenderWindow& Window, const sf::Vector2f& BorderSize, std::vector<Planet*> Planets, float DeltaTime)
{
	HandleBordersCollision(BorderSize);
	HandlePlanetsCollision(Planets);
	
	if (EnableGravityForce)
	{
		HandleGravityForces(Planets);
	}

	Shape.move(Velocity * DeltaTime);

	Window.draw(Shape);
}

void Planet::HandleBordersCollision(const sf::Vector2f& BorderSize)
{
	if (Shape.getPosition().x < 0.0f)
	{
		Velocity.x *= -1;

		Shape.setPosition(sf::Vector2f(0.0f, Shape.getPosition().y));

		HitSound.play();
	}
	else if (Shape.getPosition().x > BorderSize.x - Shape.getRadius() * 2)
	{
		Velocity.x *= -1;

		Shape.setPosition(sf::Vector2f(BorderSize.x - Shape.getRadius() * 2, Shape.getPosition().y));

		HitSound.play();
	}

	if (Shape.getPosition().y < 0)
	{
		Velocity.y *= -1;

		Shape.setPosition(sf::Vector2f(Shape.getPosition().x, 0.0f));

		HitSound.play();
	}
	else if (Shape.getPosition().y > BorderSize.y - Shape.getRadius() * 2)
	{
		Velocity.y *= -1;

		Shape.setPosition(sf::Vector2f(Shape.getPosition().x, BorderSize.y - Shape.getRadius() * 2));

		HitSound.play();
	}
}

void Planet::HandlePlanetsCollision(std::vector<Planet*> Planets)
{
	if (Planets.size() <= 1) return;

	for (auto OtherPlanet = Planets.begin(); OtherPlanet != Planets.end(); ++OtherPlanet)
	{
		if (*OtherPlanet == this) continue;

		if (GetIntersectionWithAnotherPlanet(*OtherPlanet) >= 0)
		{
			KickOutOfAnotherPlanet(*OtherPlanet);

			sf::Vector2f CollisionVelocity = GetVelocityOnPlanetCollision(*OtherPlanet);
			sf::Vector2f OtherPlanetCollisionVelocity = (*OtherPlanet)->GetVelocityOnPlanetCollision(this);

			SetVelocity(CollisionVelocity);
			(*OtherPlanet)->SetVelocity(OtherPlanetCollisionVelocity);

			HitSound.play();
		}
	}
}

void Planet::HandleGravityForces(std::vector<Planet*> Planets)
{
	if (Planets.size() <= 1) return;

	for (auto OtherPlanet = Planets.begin(); OtherPlanet != Planets.end(); ++OtherPlanet)
	{
		if (*OtherPlanet == this) continue;

		sf::CircleShape OtherPlanerShape = (*OtherPlanet)->GetShape();

		float X = OtherPlanerShape.getPosition().x + OtherPlanerShape.getRadius() - Shape.getPosition().x - Shape.getRadius();
		float Y = OtherPlanerShape.getPosition().y + OtherPlanerShape.getRadius() - Shape.getPosition().y - Shape.getRadius();

		float Distance = GetDistanceToPlanet(*OtherPlanet);

		sf::Vector2f Force(X / Distance, Y / Distance);
		Force *= GravitationalConstant * (float)((*OtherPlanet)->GetMass() * Mass / pow(Distance, 2));

		Velocity += Force;
	}
}

void Planet::KickOutOfAnotherPlanet(Planet* OtherPlanet)
{
	float Intersection = GetIntersectionWithAnotherPlanet(OtherPlanet);

	sf::Vector2f IntersectionVector = (GetCenterPosition() - OtherPlanet->GetCenterPosition()) / GetDistanceToPlanet(OtherPlanet) * Intersection;

	Shape.move(IntersectionVector); // "Kick" this planet out of another
}

const sf::Vector2f& Planet::GetVelocityOnPlanetCollision(Planet* OtherPlanet)
{
	sf::Vector2f DeltaVelocity = Velocity - OtherPlanet->GetVelocity();
	sf::Vector2f DeltaPosition = GetCenterPosition() - OtherPlanet->GetCenterPosition();

	float DistanceDeltaPosition = sqrt(DeltaPosition.x * DeltaPosition.x + DeltaPosition.y * DeltaPosition.y);
	float VelocityAndPositionProduct = DeltaVelocity.x * DeltaPosition.x + DeltaVelocity.y * DeltaPosition.y;

	return Velocity - ((2 * OtherPlanet->GetMass() * VelocityAndPositionProduct * DeltaPosition) /
		((Mass + OtherPlanet->GetMass()) * DistanceDeltaPosition * DistanceDeltaPosition));
}

void Planet::SetVelocity(const sf::Vector2f& Velocity)
{
	this->Velocity = Velocity;
}

float Planet::GetDistanceToPlanet(const Planet* OtherPlanet)
{
	sf::CircleShape OtherPlanetShape = OtherPlanet->GetShape();

	float DirectionX = OtherPlanetShape.getPosition().x + OtherPlanetShape.getRadius() - Shape.getPosition().x - Shape.getRadius();
	float DirectionY = OtherPlanetShape.getPosition().y + OtherPlanetShape.getRadius() - Shape.getPosition().y - Shape.getRadius();

	float Distance = sqrt(pow(DirectionX, 2) + pow(DirectionY, 2));

	return Distance;
}

const sf::Vector2f& Planet::GetCenterPosition()
{
	sf::Vector2f RadiusVector = sf::Vector2f(1.0f, 1.0f) * Shape.getRadius();

	return Shape.getPosition() + RadiusVector;
}

float Planet::GetIntersectionWithAnotherPlanet(Planet* OtherPlanet)
{
	return Shape.getRadius() + OtherPlanet->GetShape().getRadius() - GetDistanceToPlanet(OtherPlanet);
}
