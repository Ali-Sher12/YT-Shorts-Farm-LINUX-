#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "globals.h"
#pragma once
using namespace std;
using namespace sf;

class BallClass {
    //The main class which is responsible for all the physics and 
    //standard stuff, such as collisions and sizes.
    protected:
    float coordX = 100; // The coordinates
    float coordY = 200;
    
    float dimention = 100; //height and width, need to be same
    float centerX = dimention/2; 
    float centerY = dimention/2;

    float radius = 0;
    float friction_coefficient = 0.3;
    float delta_t = 0.03;
    float velocityX = 0;
    float velocityY = 5;    
    float terminal_velocity = 20;
    float rotation_angle = 0;
    float mass = 0;

    Sprite* ballSprite = NULL;
    vector<pair_custom> tempBoundaryVertices;
    int index_in_array = -1;
    bool activated = true;
    int health = 0;
    float e = 2;
    Texture* myTex;
    bool takingDamageFromSupesLaser = true;

    public:
    virtual void callHeroFunctions(N_Sided_Polygon_Boundary* boundary,vector<BallClass*>& ballObjects,RenderWindow&window) = 0;
    BallClass(Texture& ballTexture,float dimention_,float _VelocityX,float _VelocityY)
    {
        myTex = &ballTexture;
        // Can pass the same texture if multiple balls need to have the same texture                
        e = e_global;
        delta_t = global_delta_t;
        terminal_velocity = ball_terminal_velocity;        
        rotation_const = rotationSpeed_ball;
        velocityX = _VelocityX;
        velocityY = _VelocityY; 
        dimention = dimention_;       
        radius = dimention/2;
        mass = radius;
        ballSprite = new Sprite(ballTexture);
        ballSprite->setScale
        ({(dimention/ballTexture.getSize().x), (dimention/ballTexture.getSize().y)});

        ballSprite->setOrigin({ballTexture.getSize().x/2, ballTexture.getSize().x/2});
        ballSprite->setPosition({coordX,coordY});
    }

    private:
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
    float rotation_const = 0;

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
                wall_collide_sound->play();
                coordX = new_coord_temp.x;
                coordY = new_coord_temp.y;
                //implement step 7, not 6
                ball_velocity.set(velocityX,velocityY);
                r_vector.set(-1*(Q.y-boundary->getcenterPair().x),Q.x-boundary->getcenterPair().y);
                wall_velocity = mulVecPair(boundary->getRotationSpeed(),r_vector);
              
                relative_velocity = subPair(ball_velocity,wall_velocity);
                if(computeDotProduct(relative_velocity,normal_vector)<0)
                {
                    relative_velocity = subPair(relative_velocity,mulVecPair((1+e)*computeDotProduct(relative_velocity,normal_vector),normal_vector));
                    if(friction_enabled)
                        relative_velocity = implementFriction(AB,relative_velocity);
                    ball_velocity = addPair(relative_velocity,wall_velocity);
                    velocityX = ball_velocity.x;
                    velocityY = ball_velocity.y;                
                }
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
        rotation_angle>2*PI?0:rotation_angle;
        rotation_angle_SFML = radians(rotation_angle);
        ballSprite->setRotation(rotation_angle_SFML);
    }

    float penetration_const;
    float relative_velocity_along_normal;
    float impulse_scalar;

    float radius2,mass2,beta = 0.5;
    pair_custom v1,v2,centerPair2;
    void detectBallToBallCollision(vector<BallClass*>& ballObjects,int own_index)
    {
        index_in_array = own_index;
        float total_balls = ballObjects.size();
        for(int i = own_index+1 ; i<total_balls; i++){

            if(!ballObjects[i]->activated || ballObjects[i]->ImDyinChief)continue;

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

    void checkFunctions(){
        if (health <=0){
            ballSprite->setColor(Color(255, 0, 0, 255));
            ImDyinChief = true;
        }
    }

    int hurt_frame_index = 0;
    bool hurt_ouch = false;
    void hurt_animation(){
        ballSprite->setColor(Color(255, 0, 0, 255));
        hurt_frame_index = 0;
    }

    bool ImDyinChief = false;
    int dyin_frame_index = 0;    
    void death_animation(){
        dimention = dimention - dimention_reduction_death_anim;
        radius = dimention/2;
        ballSprite->setScale
        ({(dimention/myTex->getSize().x), (dimention/myTex->getSize().y)});
        if(dyin_frame_index>dyin_frames)
        {
            activated = false;
            ImDyinChief = false;
        }
        dyin_frame_index++;
    }

    public:
    void setRotationConst(float rotation_const_new){
        rotation_const = rotation_const_new;
    }
    bool getDeathStat(){return ImDyinChief;}
    float getRadius(){return radius;}
    float getCoordX(){return coordX;}
    float getCoordY(){return coordY;}
    bool getActivationStat(){return activated && !ImDyinChief;}    
    void depleteHealth(float factor){
        health-=factor;
        hurt_ouch = true;
        hurt_sound->play();
        hurt_animation();
    }        
    void callBallPhysicsFunctions(N_Sided_Polygon_Boundary* boundary,vector<BallClass*> ballObjects,int own_index)
    {
        if(activated && !ImDyinChief){
            if(hurt_ouch && !ImDyinChief)
            {
                if(hurt_frame_index<hurt_frames){
                    hurt_frame_index++;
                }
                else{
                    hurt_ouch = false;
                    ballSprite->setColor(Color(255, 255, 255, 255));                
                }
            }
            checkFunctions();
            if(gravity_true)
                implementGravity();
            detectCollisionWithBoundary(boundary);
            changeRotation();
            if(ball_to_ball_collision)
                detectBallToBallCollision(ballObjects,own_index);
            checkTerminalVelocity();
            finalCOORDUpdate();
        }
        else if(ImDyinChief){
            death_animation();
        }
    }
    
    void setCOORD_initial(int x,int y)
    {
        coordX = x; coordY = y;
    }
    void drawBall(RenderWindow& window)
    {
        if(activated){
            ballSprite->setPosition({coordX,coordY});//line here temporarily
            window.draw(*ballSprite);
        }
    }

    ~BallClass()
    {
        delete ballSprite;
    }
};

class BallBatman:public BallClass
{
    public:
    Sprite* BatarangSprite;
    Texture* BatarangTexture;
    float batarangX,batarangY,batarangRadius;
    bool batarangActivate = false;
    int frame_index_appearance = 0;
    int frame_index_gap = 0;    
    BallBatman(Texture& batmanBallTexture,float dimention,float _VelocityX,float _VelocityY):BallClass(batmanBallTexture, dimention,_VelocityX,_VelocityY){
        BallClass::health = 10;
        BatarangTexture = new Texture("Data/Images/batrang.png");
        BatarangSprite = new Sprite(*BatarangTexture);        
        batarangRadius = BallClass::radius*2;

        BatarangSprite->setScale
        ({((batarangRadius*2)/BatarangTexture->getSize().x), ((batarangRadius*2)/BatarangTexture->getSize().y)});
        BatarangSprite->setOrigin({BatarangTexture->getSize().x/2, BatarangTexture->getSize().x/2});

    }
    void deployBatarang(){
        if(frame_index_gap>gapFrames_bat)
        {
            swish_sound->play();
            swish_sound->setLooping(true);
            batarangActivate = true;
            frame_index_gap = 0;
            frame_index_appearance = 0;            
            batarangX = BallClass::coordX;
            batarangY = BallClass::coordY;        
        }
    }
    void collideDeactivationBatarang(vector<BallClass*>& ballObjects){
        if(batarangActivate){
            for(int i=0;i<total_balls;i++){
                if(i!=index_in_array && ballObjects[i]->getActivationStat() && !ballObjects[i]->getDeathStat()){
                    if((ballObjects[i]->getRadius()+batarangRadius)>=computeMagnitude(pair_custom(batarangX-ballObjects[i]->getCoordX(),batarangY-ballObjects[i]->getCoordY())))
                    {
                        // cout<< i <<" Here\n";
                        ballObjects[i]->depleteHealth(1);
                        batarangActivate = false;
                        swish_sound->stop();
                        frame_index_appearance = 0;
                        frame_index_gap = 0;
                        return;
                    }
                }
            }
        }
    }
    void idleDeactivateBatarang(){
        if(frame_index_appearance>appearanceFrames_bat){
            batarangActivate = false;
            swish_sound->stop();            
            frame_index_appearance = 0;
            frame_index_gap = 0;                    
        }
    }
    void drawBatarang(RenderWindow& window){
        if(batarangActivate){
            frame_index_appearance++;
            BatarangSprite->setPosition({batarangX,batarangY});
            window.draw(*BatarangSprite);        
        }
        else{
            frame_index_gap++;
        }
    }
    void batarang_animate(){
        if(batarangActivate){
            BatarangSprite->setRotation(radians(frame_index_appearance*batarang_rotate_factor));
        }        
    }
    public:
    void callHeroFunctions(N_Sided_Polygon_Boundary* boundary,vector<BallClass*>& ballObjects,RenderWindow&window) override{
        if(!getDeathStat() && activated)
        {
            deployBatarang();
            batarang_animate();        
            collideDeactivationBatarang(ballObjects);
            idleDeactivateBatarang();
            drawBatarang(window);
        }
    }
    ~BallBatman(){// claude said base destructor would be automatically called
        delete BatarangSprite;
        delete BatarangTexture;   
    }
    
};
class BallSuper:public BallClass
{
    Vertex laserEyes[2];

    bool laserActivate = false;
    int frame_index_appearance = 0;
    int frame_index_gap = 0;
    int total_fire_Sprites = 3;
    Texture* fireTexture;
    vector<Sprite*> fireSprites;
    public:
    BallSuper(Texture& batmanBallTexture,float dimention,float _VelocityX,float _VelocityY):BallClass(batmanBallTexture, dimention,_VelocityX,_VelocityY){
        BallClass::health = 9;
        setRotationConst(0.01);
        laserEyes[0].color = Color::Red;
        laserEyes[1].position = Vector2f(0, 0);
        laserEyes[1].color = Color::Yellow;

        fireTexture = new Texture("Data/Images/fire.png");
        fireSprites.resize(total_fire_Sprites);
        for(int i=0;i<total_fire_Sprites;i++)
        {
            fireSprites[i] = new Sprite(*fireTexture);
            fireSprites[i]->setTextureRect(IntRect({(int)(fireTexture->getSize().x/total_fire_Sprites)*i,0}, {fireTexture->getSize().x/total_fire_Sprites, fireTexture->getSize().y}));
            fireSprites[i]->setOrigin({fireTexture->getSize().x/(total_fire_Sprites*2),fireTexture->getSize().y});
        }

    }
     
    void deployLaser(){
        if(frame_index_gap>gapFrames_super)
        {
            laserActivate = true;
            frame_index_gap = 0;
            frame_index_appearance = 0;            
            laserSound->play();
            laserSound->setLooping(true);            
        }
        laserEyes[0].position = Vector2f(coordX, coordY);
    }
    float t_val = 0,u_val;
    float t_val_smallest = 100000000000;
    vector<pair_custom> vec_pairs;
    pair_custom known_point,pairB_m_A,pairA_m_P,anglePair,foundPair;
    void find_second_point(N_Sided_Polygon_Boundary* boundary){
        if(laserActivate){
            t_val_smallest = 100000000000;
            known_point.set(coordX,coordY);
            vec_pairs = boundary->getVerticeList();
            anglePair.set(cos(rotation_angle),sin(rotation_angle));
            for(int i=0;i<PolygonSides;i++){
                pairA_m_P = subPair(vec_pairs[i],known_point);
                pairB_m_A = subPair(vec_pairs[(i+1)%PolygonSides],vec_pairs[i]);
                t_val = crossProduct(pairA_m_P,pairB_m_A)
                        /crossProduct(anglePair,pairB_m_A);
                u_val = crossProduct(pairA_m_P,anglePair)/crossProduct(anglePair,pairB_m_A);
                if(t_val>0 && u_val>=0 && u_val<=1)
                    t_val<t_val_smallest?t_val_smallest=t_val:t_val=t_val;
            }
            foundPair = addPair(known_point,mulVecPair(t_val_smallest,anglePair));
            laserEyes[1].position = Vector2f(foundPair.x,foundPair.y);
        }
    }
    
    float fire_animation_index = 0;
    void drawFire(RenderWindow& window){
        if(laserActivate){
            fire_animation_index+=0.02;
            fireSprites[(int)(fire_animation_index)%total_fire_Sprites]->setPosition({laserEyes[1].position.x,laserEyes[1].position.y});
            window.draw(*fireSprites[(int)(fire_animation_index)%total_fire_Sprites]);
            frame_index_appearance++;
        }

    }

    float lasterLength = 0,D_sq = 0;
    pair_custom P_closest;
    void laserDamageFunction(vector<BallClass*>& ballObjects){
        if(laserActivate){
            lasterLength = computeMagnitude(pair_custom(coordX-foundPair.x,coordY-foundPair.y));
            for(int i=0;i<total_balls;i++){
                if(i != index_in_array && ballObjects[i]->getActivationStat() && !ballObjects[i]->getDeathStat())
                {
                   t_val = (ballObjects[i]->getCoordX()-coordX)*anglePair.x+(ballObjects[i]->getCoordY()-coordY)*anglePair.y;
                   t_val<0?t_val=0:t_val=t_val;
                   t_val>lasterLength?t_val=lasterLength:t_val=t_val;
                   P_closest.set(known_point.x+t_val*anglePair.x,known_point.y+t_val*anglePair.y);
                   D_sq = (ballObjects[i]->getCoordX()-P_closest.x)*(ballObjects[i]->getCoordX()-P_closest.x)+(ballObjects[i]->getCoordY()-P_closest.y)*(ballObjects[i]->getCoordY()-P_closest.y);
                   if(D_sq<=ballObjects[i]->getRadius()*ballObjects[i]->getRadius()){
                        laserEyes[1].position = Vector2f({P_closest.x,P_closest.y});
                        if(takingDamageFromSupesLaser)ballObjects[i]->depleteHealth(1);
                        takingDamageFromSupesLaser = false;
                   }
                   else takingDamageFromSupesLaser=true;
                }
            }
        }
    }

    void drawLaser(RenderWindow& window){
        if(laserActivate){
            frame_index_appearance++;
            window.draw(laserEyes,2,PrimitiveType::Lines);  
            drawFire(window);      
        }
        else{
            fire_animation_index = 0;
            frame_index_gap++;
        }
    }
    void idleDeactivateLaser(){
        if(frame_index_appearance>appearanceFrames_super){
            laserActivate = false;
            frame_index_appearance = 0;
            frame_index_gap = 0;       
            laserSound->stop();             
        }
    }
    void callHeroFunctions(N_Sided_Polygon_Boundary* boundary,vector<BallClass*>& ballObjects,RenderWindow&window) override{
        if(!getDeathStat() && activated){
            deployLaser();
            idleDeactivateLaser();
            find_second_point(boundary);
            laserDamageFunction(ballObjects);            
            drawLaser(window);
        }
    }
    ~BallSuper(){
        delete fireTexture;
        for(int i=0;i<total_fire_Sprites;i++)
        delete fireSprites[i];
    }    
};

