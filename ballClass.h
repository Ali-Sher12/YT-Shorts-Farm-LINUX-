#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "globals.h"
#pragma once
using namespace std;
using namespace sf;

float gravity_strength = 9;

class BallClass {
    //The main class which is responsible for all the physics and 
    //standard stuff, such as collisions and sizes.
    float coordX = 100; // The coordinates
    float coordY = 200;

    float centerX = dimention/2; 
    float centerY = dimention/2;

    float radius = 20;
    float velocityX = 0;
    float velocityY = 2;    
    float terminal_velocity = 50;
    float rotation_angle = 0;
    float rotation_speed = 0; // radian per sec. 
    float rotation_direction = 1; // 1 for counterclockwise, -1 for clockwise     

    float dimention = 20; //height and width, need to be same
    Sprite* ballSprite = NULL;

    public:
    BallClass(Texture& ballTexture)
    {
        // Can pass the same texture if multiple balls need to have the same texture                
        ballSprite = new Sprite(ballTexture);
        ballSprite->setOrigin({centerX, centerY});
        ballSprite->setPosition({coordX,coordY});
    }

    //To be defined parameters unknown
    bool detectCollisionWithOtherBalls()
    {
        return false;
    }
    bool detectCollisionWithBoundary()
    {
        return false;
    }    
    void changeVelocities()
    {}
    void changeRotationDirection()
    {}
    void changeRotationSpeed()
    {}
    void updateBallPosition()
    {}
    void drawBall(RenderWindow& window)
    {
        window.draw(*ballSprite);
    }

    ~BallClass()
    {
        delete ballSprite;
    }
};

class BallPakistan:public BallClass
{
    public:
    BallPakistan(Texture& ballTexture_Pakistan):BallClass(ballTexture_Pakistan)
    {}

};
