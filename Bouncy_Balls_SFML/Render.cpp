#include "Render.h"

Render::Render()
{
	m_window.create(sf::VideoMode(800, 600), "Bouncy Balls"); // The create  function is used when we create the window AFTER initilization.
	m_view = m_window.getView();
	m_view.setSize(800, 600);
	m_window.setTitle("Bouncy Balls");
	m_window.setView(m_view);
}

sf::RenderWindow& Render::getWindow()
{
	return m_window;
}
