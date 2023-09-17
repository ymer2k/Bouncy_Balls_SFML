#pragma once
#include <SFML/Graphics.hpp>

class Balls
{
public:

	//Balls(sf::Vector2f pos, float radius, sf::Color color);
	Balls(sf::Vector2f pos = { 0.0f,0.0f }, sf::Vector2f vel = { 0,0 }, float radius = 0.0f, sf::Color color = { 0,0,0 });

	void setPos(sf::Vector2f pos);
	sf::Vector2f getPos();
	void setVel(sf::Vector2f vel);
	sf::Vector2f getVel();
	sf::Vector2f getAcc();
	void setRadius(float r);
	float getRadius();
	float getMass();

	sf::Color getColor();
	sf::CircleShape getShape();



	



private:
	sf::Vector2f m_pos;
	sf::Vector2f m_vel;
	sf::Vector2f m_acc;
	sf::Color m_color;
	float m_mass;
	float m_radius;
	sf::CircleShape m_shape;




};

