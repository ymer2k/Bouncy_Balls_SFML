#pragma once
#include <SFML/Graphics.hpp>
#include "Balls.h"
#include <array>


class Game
{
public:
	Game();

	void update(sf::Time dt, sf::RenderWindow& window);
	void drawGame(sf::RenderWindow& window);
	void interact(sf::RenderWindow& window);


private:
	sf::Event m_event;
	sf::CircleShape m_shape;
	sf::Vector2f m_shapeP;
	sf::Vector2f m_shapeV;
	sf::Vector2f m_shapeA;
	float m_shapeR;
	float m_g;
	int nrOfBalls;
	sf::Vector2f m_worldMousePos;

	std::array<Balls, 200> ballArray;


	// methods
	void edgeOfScreenDetection(Balls & ball,sf::Vector2u worldSize);
	void updateBallVelAndPos(Balls& ball, sf::Time dt);
	bool colliding(Balls ball1, Balls ball2);
	void handleCollision(Balls& ball1, Balls &ball2);
	float dot(sf::Vector2f vector1, sf::Vector2f vector2); // dotproduct of two 2d vectors


};
