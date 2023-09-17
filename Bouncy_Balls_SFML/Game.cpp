#include "Game.h"
#include "math.h"
#include <iostream>
#include <stdlib.h>


Game::Game():
    m_shapeP(100,100)
    , m_shapeV(0,0)
    , m_shapeA(0,0)
    , m_shapeR(50)
    , m_g(400)
    , nrOfBalls(0)
{

}

void Game::update(sf::Time dt, sf::RenderWindow& window)
{
    for (auto& ball : ballArray)
    {
        if (ball.getRadius()) // Only get balls that has a positive radius
        {
            updateBallVelAndPos(ball, dt);
            edgeOfScreenDetection(ball, window.getSize());
        }
    }

    for (int i = 0; i < 200 ; i++)
    {
        for (int j = 0; j < 200; j++)
        {
            if (i == j) // Don't check collision with itself
                continue;
            if (ballArray[i].getRadius() && ballArray[j].getRadius())
            {
                if (colliding(ballArray[i], ballArray[j]))
                {
                    //std::cout << "colliding" << std::endl;
                    handleCollision(ballArray[i], ballArray[j]);
                }
            }
        }
    }

}

void Game::drawGame(sf::RenderWindow& window)
{
    // Clear window before drawing
    window.clear();

    // Draw everything here
    for (auto ball : ballArray)
    {
        if (ball.getRadius()) // Draw if radius is not zero
        {
            window.draw(ball.getShape());
        }
    }
        

    //draw line
    sf::Vertex line[2];
    line[0].color = sf::Color::White;
    line[1].color = sf::Color::White;
    line[0].position = sf::Vector2f(0, window.getSize().y - 100.0f);
    line[1].position = sf::Vector2f(window.getSize().x, window.getSize().y - 100.0f);
    window.draw(line, 2, sf::Lines);

    // Display everything that has been drawn
    window.display();
}

void Game::interact(sf::RenderWindow& window)
{
    while (window.pollEvent(m_event))
    {
        if (m_event.type == sf::Event::Closed)
        {
            window.close();
        }

        if (m_event.type == sf::Event::MouseButtonPressed)
        {
            if (m_event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i mousePos(m_event.mouseButton.x, m_event.mouseButton.y);
                sf::Vector2i worldMousePos(window.mapPixelToCoords(mousePos));
                m_worldMousePos = window.mapPixelToCoords(mousePos);

                //std::cout << "left Mouse Button Pressed" << std::endl;
            }
        }

        if (m_event.type == sf::Event::MouseButtonReleased)
        {
            // Create the ball when I release the mouse button and give the ball some velocity
            //std::cout << "left Mouse Button released" << std::endl;
            sf::Vector2i mousePosRelease(m_event.mouseButton.x, m_event.mouseButton.y);
            sf::Vector2f worldMousePosRelease(window.mapPixelToCoords(mousePosRelease));
            sf::Vector2f velocity(worldMousePosRelease - m_worldMousePos); // Get the distance between the two points
            Balls ball(sf::Vector2f(m_worldMousePos),velocity, 25, sf::Color(rand() % 255, rand() % 255, rand() % 255)); // rand() % 50 + 25
            ballArray[nrOfBalls] = ball;
            nrOfBalls++;
            std::cout << "NrOfBalls: " << nrOfBalls << std::endl;
            if (nrOfBalls > 199)
            {
                nrOfBalls = 0;
            }
        }
    }
}

void Game::updateBallVelAndPos(Balls& ball, sf::Time dt)
{
    float vX = ball.getVel().x;
    float vY = ball.getVel().y;
    float pX = ball.getPos().x;
    float pY = ball.getPos().y;

    // Update position of balls here.
    sf::Vector2f newVel(vX + 0 * dt.asSeconds(), vY + m_g * dt.asSeconds());
    ball.setVel(newVel);
    sf::Vector2f newPos(pX + vX * dt.asSeconds() + dt.asSeconds() * dt.asSeconds() * 0 / 2.0f,
        pY + vY * dt.asSeconds() + dt.asSeconds() * dt.asSeconds() * m_g / 2.0f);
    ball.setPos(newPos);
}

void Game::edgeOfScreenDetection(Balls& ball, sf::Vector2u worldSize)
{
    float radius = ball.getRadius();
    // Screen edge collision detection
    if (ball.getPos().y + radius > worldSize.y - 100.0f || ball.getPos().y + radius < 0) // Bottom and Top detection
    {
        if (abs(ball.getVel().y) < 48)
        {
            ball.setVel(sf::Vector2f(ball.getVel().x, 0));
        }
        sf::Vector2f newPos(ball.getPos().x, worldSize.y - 100.0f - radius); // To prevent going through the wall GROUND
        ball.setPos(newPos);
        sf::Vector2f newVel(ball.getVel().x, -ball.getVel().y * 0.9); // 10% damping
        ball.setVel(newVel);
    }
    if (ball.getPos().x + radius > worldSize.x) // Right
    {
        sf::Vector2f newPos(worldSize.x - radius, ball.getPos().y);
        ball.setPos(newPos);

        sf::Vector2f newVel(-ball.getVel().x * 0.9f, ball.getVel().y);
        ball.setVel(newVel);
    }
    else if (ball.getPos().x - radius < 0) //Left
    {
        sf::Vector2f newPos(0 + radius, ball.getPos().y);
        ball.setPos(newPos);

        sf::Vector2f newVel(-ball.getVel().x * 0.9f, ball.getVel().y);
        ball.setVel(newVel);
    }
}


bool Game::colliding(Balls ball1, Balls ball2)
{
    sf::Vector2f distance = ball1.getPos() - ball2.getPos(); // get the distance between two balls as a vector (x,y)
    // square both sides of the equation to avoid using square root.
    float sqrDistanceScalar = distance.x * distance.x + distance.y * distance.y;
    float sumRadius = ball1.getRadius() + ball2.getRadius();
    float sqrSumRadius = sumRadius * sumRadius;

    if (sqrDistanceScalar <= sqrSumRadius) 
    {
        return true;
    }
    else
        return false;
}

void Game::handleCollision(Balls& ball1, Balls& ball2)
{
    //Step 1 find the normal vector i.e. the vector that points from the middle of ball1 to the middle of ball2
    sf::Vector2f normalVector = ball1.getPos() - ball2.getPos(); 
    //step 1.a check if balls are moving towards one anoter.
    float dotProduct = dot(normalVector, ball2.getVel() - ball1.getVel());
    if (dotProduct > 0) //then moving towards each other
    {
        // Step 2 Find the UNIT normal vector. i.e. the noromal vector when it has a magnitude of 1 or in other words length 1.
        // Divide the normalvector by the magnitude of the normalvector
        // unit normal vector = normalvector/ magnitude(unit normal vector) i.e. UNV = normalVector/sqrt(normalVector.x^2 + normalVector.y^2)
        sf::Vector2f unitNormalVector = normalVector / sqrt(normalVector.x * normalVector.x + normalVector.y * normalVector.y);
        //Step 3 get the tangent vector. Just flip from (UNV.x,UNV.y) to (-UNV.y, UNV.x)
        sf::Vector2f unitTangentVector(-unitNormalVector.y, unitNormalVector.x);

        // Step 4 the velocity after impact will be divide up into a velocity in the normal direction and in the tangent direction.
        // to get these scalar velocitties. do the dot product of the velocity and the unit tangent vector/ unit normal vector
        float v1n = dot(unitNormalVector, ball1.getVel());
        float v1t = dot(unitTangentVector, ball1.getVel());
        float v2n = dot(unitNormalVector, ball2.getVel());
        float v2t = dot(unitTangentVector, ball2.getVel());
        // Step 5 get the tangent velocities AFTER colission (they are same because there is not force between the balls in the tangental direction during colission)
        float v1tAfter = v1t;
        float v2tAfter = v2t;
        // Step 6 find the normal velcoities AFTER colission. USe the One dimentional collission formula.
        float v1nAfter = (v1n * (ball1.getMass() - ball2.getMass()) + 2 * ball2.getMass() * v2n) / (ball1.getMass() + ball2.getMass());
        float v2nAfter = (v2n * (ball2.getMass() - ball1.getMass()) + 2 * ball1.getMass() * v1n) / (ball1.getMass() + ball2.getMass());
        // Step 7. convert scalar normal and tangenttial velocities into vectors
        sf::Vector2f v1nAfterV = v1nAfter * unitNormalVector;
        sf::Vector2f v1tAfterV = v1tAfter * unitTangentVector;
        sf::Vector2f v2nAfterV = v2nAfter * unitNormalVector;
        sf::Vector2f v2tAfterV = v2tAfter * unitTangentVector;
        // Step 8 add the normal and tangential compoinents for  each ball.
        sf::Vector2f v1After = v1nAfterV + v1tAfterV;
        sf::Vector2f v2After = v2nAfterV + v2tAfterV;
        // Step 9 set the new velocities
        ball1.setVel(v1After);
        ball2.setVel(v2After);
    }
}

float Game::dot(sf::Vector2f vector1, sf::Vector2f vector2)
{
    float retVal = 0.0f;
    retVal = (vector1.x * vector2.x + vector1.y * vector2.y);
    return retVal;
}

