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
    
    float dimention = 90; //height and width, need to be same
    float centerX = dimention/2; 
    float centerY = dimention/2;

    float radius = 0;
    float velocityX = 0;
    float velocityY = 2;    
    float terminal_velocity = 50;
    float rotation_angle = 0;
    float rotation_speed = 0; // radian per sec. 
    float rotation_direction = 1; // 1 for counterclockwise, -1 for clockwise     

    Sprite* ballSprite = NULL;

    public:
    BallClass(Texture& ballTexture)
    {
        // Can pass the same texture if multiple balls need to have the same texture                
        radius = dimention/2;
        ballSprite = new Sprite(ballTexture);
        ballSprite->setScale
        ({(dimention/ballTexture.getSize().x), (dimention/ballTexture.getSize().y)});

        ballSprite->setOrigin({ballTexture.getSize().x/2, ballTexture.getSize().x/2});
        ballSprite->setPosition({coordX,coordY});
    }

    private:
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

    public:
    void setCOORD_initial(int x,int y)
    {
        coordX = x; coordY = y;
    }
    void drawBall(RenderWindow& window)
    {
        ballSprite->setPosition({coordX,coordY});//line here temporarily
        window.draw(*ballSprite);
    }

    ~BallClass()
    {
        delete ballSprite;
    }
};

class BallBatman:public BallClass
{
    public:
    BallBatman(Texture& batmanBallTexture):BallClass(batmanBallTexture){}
};

class BallSpiderman:public BallClass
{
    public:
    BallSpiderman(Texture& spiderBallTexture):BallClass(spiderBallTexture){}
};
