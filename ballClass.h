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
    float delta_t = 0.03;
    float velocityX = 0;
    float velocityY = 5;    
    float terminal_velocity = 15;
    float rotation_angle = 0;
    float mass = 0;

    Sprite* ballSprite = NULL;
    vector<pair_custom> tempBoundaryVertices;

    protected:
    float e = 1;
    public:
    BallClass(Texture& ballTexture,float dimention)
    {
        // Can pass the same texture if multiple balls need to have the same texture                
        radius = dimention/2;
        mass = radius;
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
    float rotation_const = 0.015;

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
                // wall_collide_sound->play();
                coordX = new_coord_temp.x;
                coordY = new_coord_temp.y;
                //implement step 7, not 6
                ball_velocity.set(velocityX,velocityY);
                r_vector.set(-1*(Q.y-boundary->getCenterY()),Q.x-boundary->getCenterX());
                wall_velocity = mulVecPair(boundary->getRotationSpeed(),r_vector);
              
                relative_velocity = subPair(ball_velocity,wall_velocity);
                if(computeDotProduct(relative_velocity,normal_vector)<0)
                {
                    relative_velocity = subPair(relative_velocity,mulVecPair((1+e)*computeDotProduct(relative_velocity,normal_vector),normal_vector));
                    implementFriction(AB,relative_velocity);
                    ball_velocity = addPair(relative_velocity,wall_velocity);
                    velocityX = ball_velocity.x;
                    velocityY = ball_velocity.y;                
                }
            }
        }
    }    

    void implementGravity()
    {
//        velocityY = velocityY + gravity_strength*delta_t;
    }
    void finalCOORDUpdate()
    {
        coordX+=velocityX*delta_t;
        coordY+=velocityY*delta_t;        
    }
    
    pair_custom tangent;
    pair_custom implementFriction(pair_custom AB,pair_custom relative_velocity)
    {//to be called in detectCollisionWithBoundary()
        tangent = mulVecPair(1/computeMagnitude(AB),AB);
        float tanget_component = computeDotProduct(relative_velocity,tangent);
        relative_velocity.x -= friction_coefficient*tanget_component*tangent.x;
        relative_velocity.y -= friction_coefficient*tanget_component*tangent.y;        
        return relative_velocity;
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

    Angle rotation_angle_SFML;
    void changeRotation()
    {
        rotation_angle+=velocityX*rotation_const*delta_t;
        rotation_angle_SFML = radians(rotation_angle);
        ballSprite->setRotation(rotation_angle_SFML);
    }

    float penetration_const;
    float relative_velocity_along_normal;
    float impulse_scalar;

    float radius2,mass2,beta = 0.5;
    pair_custom v1,v2,centerPair2;
    void detectBallToBallCollision(vector<BallClass*> ballObjects,int own_index)
    {
        float total_balls = ballObjects.size();
        for(int i = own_index+1 ; i<total_balls; i++){
            centerPair.set(coordX-ballObjects[i]->coordX,coordY-ballObjects[i]->coordY);
            float dist = computeMagnitude(centerPair);
            if(dist>(radius+ballObjects[i]->radius) || dist<0.0001f)
                continue;
            ball_collide_sound->play();
            mass2 = ballObjects[i]->mass;
            radius2 = ballObjects[i]->radius;
            v1.set(velocityX,velocityY);
            v2.set(ballObjects[i]->velocityX,ballObjects[i]->velocityY);
            centerPair.set(coordX,coordY);
            centerPair2.set(ballObjects[i]->coordX,ballObjects[i]->coordY);
            
            normal_vector.set((coordX-centerPair2.x)/dist,(coordY-centerPair2.y)/dist);
            //penetrationDepth of ball to wall collision will be repurposed
            penetrationDepth = (radius + radius2)-dist;
            penetration_const = penetrationDepth*beta*(mass/(mass+mass2));
            centerPair = addPair(centerPair,mulVecPair(penetration_const,normal_vector));
            coordX = centerPair.x;
            coordY = centerPair.y;
            penetration_const = penetrationDepth*beta*((mass2)/(mass+mass2));
            centerPair2 = addPair(centerPair2,mulVecPair(penetration_const,normal_vector));            
            ballObjects[i]->coordX = centerPair2.x;
            ballObjects[i]->coordY = centerPair2.y;

            relative_velocity_along_normal = computeDotProduct(subPair(v1,v2),normal_vector);
            if (relative_velocity_along_normal > 0) 
                continue;
            impulse_scalar = ((-1-e)*relative_velocity_along_normal)/((1/mass)+(1/mass2));
            v1 = addPair(v1,mulVecPair(impulse_scalar/mass,normal_vector));
            v2 = subPair(v2,mulVecPair(impulse_scalar/mass2,normal_vector));
            velocityX = v1.x;
            velocityY = v1.y;            
            ballObjects[i]->velocityX = v2.x;
            ballObjects[i]->velocityY = v2.y; 
        }

    }


    public:
    void callBallPhysicsFunctions(N_Sided_Polygon_Boundary* boundary,vector<BallClass*> ballObjects,int own_index)
    {
        implementGravity();
        detectCollisionWithBoundary(boundary);
        changeRotation();
        detectBallToBallCollision(ballObjects,own_index);
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
    BallBatman(Texture& batmanBallTexture,float dimention):BallClass(batmanBallTexture, dimention){}
};
class BallSpider:public BallClass
{
    public:
    BallSpider(Texture& batmanBallTexture,float dimention):BallClass(batmanBallTexture, dimention){}
};
class BallSuper:public BallClass
{
    public:
    BallSuper(Texture& batmanBallTexture,float dimention):BallClass(batmanBallTexture, dimention){}
};
class BallHulk:public BallClass
{
    public:
    BallHulk(Texture& batmanBallTexture,float dimention):BallClass(batmanBallTexture, dimention){
        BallClass::e = 1.2;
    }
};
class BallInvincible:public BallClass
{
    public:
    BallInvincible(Texture& batmanBallTexture,float dimention):BallClass(batmanBallTexture, dimention){}
};

