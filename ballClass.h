#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "globals.h"
#pragma once
using namespace std;
using namespace sf;

float gravity_strength = 0.2;

class BallClass {
    //The main class which is responsible for all the physics and 
    //standard stuff, such as collisions and sizes.
    float coordX = 100; // The coordinates
    float coordY = 200;
    
    float dimention = 70; //height and width, need to be same
    float centerX = dimention/2; 
    float centerY = dimention/2;

    float radius = 0;
    float friction_coefficient = 0.3;
    float delta_t = 0.02;
    float velocityX = 0;
    float velocityY = 5;    
    float terminal_velocity = 20;
    float rotation_angle = 0;
    float rotation_speed = 0; // radian per sec. 
    float rotation_direction = 1; // 1 for counterclockwise, -1 for clockwise     
    Sprite* ballSprite = NULL;
    vector<pair_custom> tempBoundaryVertices;
    public:
    BallClass(Texture& ballTexture, N_Sided_Polygon_Boundary* boundary)
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
    //better to handle all the collisions and such in ball class
    //rather than call each ball to the boundary
    pair_custom A;pair_custom B;
    pair_custom AB;
    float t = 0;
    float distanceToWall = 0;
    float penetrationDepth = 0;
    pair_custom Q;
    pair_custom wall_velocity;
    pair_custom ball_velocity;
    pair_custom relative_velocity;                
    pair_custom r_vector;                
    pair_custom centerPair;  
    pair_custom normal_vector;  
    pair_custom new_coord_temp;          
    pair_custom P;
    pair_custom perp;
    void detectCollisionWithBoundary(N_Sided_Polygon_Boundary* boundary)
    {
        int total_vertices = (int)(boundary->gettotalVertices());
        tempBoundaryVertices = boundary->getVerticeList();

        for(int i=0;i<total_vertices;i++)
        {
            float collisionOccured = false;
            centerPair.set(coordX,coordY);
            A.set(tempBoundaryVertices[i].x,tempBoundaryVertices[i].y);
            B.set(tempBoundaryVertices[(i+1)%total_vertices].x,tempBoundaryVertices[(i+1)%total_vertices].y);
            AB.set(B.x-A.x,B.y-A.y);
            P.set(coordX-A.x,coordY-A.y);
            t = computeDotProduct(P,AB)/computeDotProduct(AB,AB);
            t = clamp(t,0.0f,1.0f);//f is important here
            Q.set(A.x+t*AB.x , A.y+t*AB.y);
            distanceToWall = computeMagnitude(subPair(centerPair,Q));
            penetrationDepth = radius - distanceToWall;
            collisionOccured = penetrationDepth>=0?true:false;

            if (distanceToWall < 0.0001f) 
            { 
                perp.set(-1*AB.y, AB.x);
                normal_vector = normalize(perp);
            }
            else
               normal_vector = mulVecPair(1/distanceToWall , subPair(centerPair,Q));
            new_coord_temp = addPair(centerPair,mulVecPair(penetrationDepth , normal_vector));
            if(collisionOccured == true)
            {
                coordX = new_coord_temp.x;
                coordY = new_coord_temp.y;
                //implement step 7, not 6
                ball_velocity.set(velocityX,velocityY);
                r_vector.set(-1*(Q.y-boundary->getCenterY()),Q.x-boundary->getCenterX());
                wall_velocity = mulVecPair(boundary->getRotationSpeed(),r_vector);
                relative_velocity = subPair(ball_velocity,wall_velocity);
                if(computeDotProduct(relative_velocity,normal_vector)<0)
                {
                    ball_velocity = addPair(subPair(relative_velocity , 
                    mulVecPair((1+e)*computeDotProduct(relative_velocity,normal_vector),normal_vector)),wall_velocity);
                    velocityX = ball_velocity.x;
                    velocityY = ball_velocity.y;                
                }
                //implementFriction(AB,relative_velocity);
            }
        }
    }    

    void implementGravity()
    {
        velocityY = velocityY + gravity_strength*delta_t;
    }
    void finalCOORDUpdate()
    {
        coordX+=velocityX*delta_t;
        coordY+=velocityY*delta_t;        
    }
    
    pair_custom tangent;
    void implementFriction(pair_custom AB,pair_custom relative_velocity)
    {//to be called in detectCollisionWithBoundary()
        tangent = mulVecPair(1/computeMagnitude(AB),AB);
        float tanget_component = computeDotProduct(relative_velocity,tangent);
        velocityX = velocityX - friction_coefficient*tanget_component*tangent.x;
        velocityY = velocityY - friction_coefficient*tanget_component*tangent.y;        
    }
    
    pair_custom temp_terminal;
    void checkTerminalVelocity()
    {
        temp_terminal.set(velocityX,velocityY);
        float speed = computeMagnitude(temp_terminal);
        if(speed>terminal_velocity){
            temp_terminal = mulVecPair(terminal_velocity/speed,temp_terminal);
            velocityX = temp_terminal.x;
            velocityY = temp_terminal.y;
        }
    }    
    void detectBallToBallCollision()
    {}

    void changeRotation()
    {}

    public:
    void callBallPhysicsFunctions(N_Sided_Polygon_Boundary* boundary)
    {
        implementGravity();
        detectCollisionWithBoundary(boundary);


        checkTerminalVelocity();
        finalCOORDUpdate();
    }
    
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
    BallBatman(Texture& batmanBallTexture,N_Sided_Polygon_Boundary* boundary):BallClass(batmanBallTexture, boundary){}
};
