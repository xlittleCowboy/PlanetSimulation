#include "PlanetHandle.h"

PlanetHandle::PlanetHandle()
{
    RadiusToMassMultiplier = 5.0f;

    MinRadius = 25.0f;
    MinMass = MinRadius * RadiusToMassMultiplier;

    StartRadius = 25.0f;
    StartMass = StartRadius * RadiusToMassMultiplier;

    RadiusIncreaser = 1.0f;
    MassIncreaser = RadiusIncreaser * RadiusToMassMultiplier;

    MaxRadius = 50.0f;
    MaxMass = MaxRadius * RadiusToMassMultiplier;

    srand(time(0));

    StartColor = sf::Color(std::rand() % 255, std::rand() % 255, std::rand() % 255);

    IsLMBPressed = false;
}

PlanetHandle::~PlanetHandle()
{
    for (auto OtherPlanet = Planets.begin(); OtherPlanet != Planets.end(); ++OtherPlanet)
    {
        delete *OtherPlanet;
    }
}

void PlanetHandle::EventHandle(sf::RenderWindow& Window, const sf::Event& Event)
{
    if (Event.type == sf::Event::MouseWheelMoved)
    {
        ChangeRadius(Event.mouseWheel.delta);
    }

    if (Event.type == sf::Event::MouseButtonReleased)
    {
        if (Event.mouseButton.button == sf::Mouse::Left)
        {
            SpawnPlanet(sf::Mouse::getPosition(Window));
        }
    }
}

void PlanetHandle::Update(sf::RenderWindow& Window, const sf::Vector2f& BorderSize, const sf::Event& Event, float DeltaTime)
{
    DrawSampleShape(Window);
    DrawDirectionShape(Window);

    for (auto OtherPlanet = Planets.begin(); OtherPlanet != Planets.end(); ++OtherPlanet)
    {
        (*OtherPlanet)->Update(Window, BorderSize, Planets, DeltaTime);
    }
}

void PlanetHandle::ChangeRadius(float Delta)
{
    if (Delta > 0)
    {
        StartRadius = Clamp(StartRadius + RadiusIncreaser, MinRadius, MaxRadius);
        StartMass = Clamp(StartMass + MassIncreaser, MinMass, MaxMass);
    }
    else if (Delta < 0)
    {
        StartRadius = Clamp(StartRadius - RadiusIncreaser, MinRadius, MaxRadius);
        StartMass = Clamp(StartMass - MassIncreaser, MinMass, MaxMass);
    }
}

void PlanetHandle::SpawnPlanet(const sf::Vector2i& MousePosition)
{
    sf::Vector2f StartVelocity;
    StartVelocity.x = MousePosition.x - StartPosition.x;
    StartVelocity.y = MousePosition.y - StartPosition.y;

    Planet* NewPlanet = new Planet(StartMass, StartRadius, StartPosition, StartVelocity, StartColor);

    Planets.push_back(NewPlanet);

    StartColor = sf::Color(std::rand() % 255, std::rand() % 255, std::rand() % 255);
}

void PlanetHandle::DrawSampleShape(sf::RenderWindow& Window)
{
    sf::CircleShape SampleShape;
    SampleShape.setFillColor(StartColor);
    SampleShape.setRadius(StartRadius);

    Window.draw(SampleShape);
}

void PlanetHandle::DrawDirectionShape(sf::RenderWindow& Window)
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        if (!IsLMBPressed)
        {
            IsLMBPressed = true;

            StartPosition.x = sf::Mouse::getPosition(Window).x;
            StartPosition.y = sf::Mouse::getPosition(Window).y;
        }

        sf::Vector2i MousePosition = sf::Mouse::getPosition(Window);
        StartVelocity.x = MousePosition.x - StartPosition.x;
        StartVelocity.y = MousePosition.y - StartPosition.y;

        Window.draw(GetDirectionShape(StartPosition, StartVelocity));
    }
    else
    {
        IsLMBPressed = false;
    }
}

sf::RectangleShape PlanetHandle::GetDirectionShape(const sf::Vector2f& StartPosition, const sf::Vector2f& Direction)
{
    float X = Direction.x;
    float Y = Direction.y;

    float LineThickness = 5.0f;

    float DirectionLength = sqrt(pow(X, 2) + pow(Y, 2));
    X > 0 ? DirectionLength *= 1 : DirectionLength *= -1;

    sf::RectangleShape Line(sf::Vector2f(DirectionLength, LineThickness));

    Line.setFillColor(sf::Color::White);
    Line.setPosition(sf::Vector2f(StartPosition.x, StartPosition.y));
    Line.rotate(asin(Y / DirectionLength) * 180.0f / 3.14f);

    return Line;
}

float PlanetHandle::Clamp(float Value, float Min, float Max)
{
    if (Value < Min) return Min;
    else if (Value > Max) return Max;

    return Value;
}