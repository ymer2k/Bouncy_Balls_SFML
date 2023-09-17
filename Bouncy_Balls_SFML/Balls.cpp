#include "Balls.h"

//Balls::Balls(sf::Vector2f pos, float radius, sf::Color color) :

Balls::Balls(sf::Vector2f pos, sf::Vector2f vel,float radius,sf::Color color) :
	m_pos(pos)
	, m_vel(vel)
	, m_radius(radius)
	, m_color(color)
	
{
	m_shape.setPosition(m_pos);
	m_shape.setRadius(m_radius);
	m_shape.setOrigin(m_radius, m_radius);
	m_shape.setFillColor(color);
	m_mass = m_radius;


}

void Balls::setPos(sf::Vector2f pos)
{
	m_pos = pos;
	m_shape.setPosition(m_pos);
}

sf::Vector2f Balls::getPos()
{
	return m_pos;
}

void Balls::setVel(sf::Vector2f vel)
{
	m_vel = vel;
}

sf::Vector2f Balls::getVel()
{
	return m_vel;
}

void Balls::setRadius(float r)
{
	m_radius = r;
}

float Balls::getRadius()
{
	return m_radius;
}

float Balls::getMass()
{
	return m_mass;
}

sf::CircleShape Balls::getShape()
{
	return m_shape;
}
