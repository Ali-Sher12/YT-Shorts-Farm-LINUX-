#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include "globals.h"
#include "boundaries.h"
#include "ballClass.h"

using namespace std;
using namespace sf;

int main()
{
    RenderWindow window(VideoMode({screenWidth, screenHeight}), "Ball Farm");

    ball_collide_buffer.loadFromFile("Data/Audio/ball_collision.ogg");
    wall_collide_buffer.loadFromFile("Data/Audio/wall_collision.ogg");    
    ball_collide_sound = new Sound(ball_collide_buffer);
    wall_collide_sound = new Sound(wall_collide_buffer);
    ball_collide_sound->setVolume(100);
    wall_collide_sound->setVolume(100);    
/////////////////////////// Texture setup
    Texture batmanBallTexture,spiderBallTexture,superBallTexture,hulkBallTexture,invincibleBallTexture;
    batmanBallTexture.loadFromFile("Data/Images/bat.png");
    superBallTexture.loadFromFile("Data/Images/sup.png");
    spiderBallTexture.loadFromFile("Data/Images/spider.png");
    hulkBallTexture.loadFromFile("Data/Images/hulk.png");    
    invincibleBallTexture.loadFromFile("Data/Images/inv.png");
    ///////////////////////////    
    N_Sided_Polygon_Boundary mainBoundary(5);
    vector<BallClass*> ballObjects;
    ballObjects.resize(5);
    ballObjects[0] = new BallBatman(batmanBallTexture,50);
    ballObjects[1] = new BallSuper(superBallTexture,60);
    ballObjects[2] = new BallSpider(spiderBallTexture,50);
    ballObjects[3] = new BallHulk(hulkBallTexture,60);
    ballObjects[4] = new BallInvincible(invincibleBallTexture,40);

    ballObjects[0]->setCOORD_initial(mainBoundary.getCenterX()-40,mainBoundary.getCenterY()-40);
    ballObjects[1]->setCOORD_initial(mainBoundary.getCenterX()+40,mainBoundary.getCenterY()-40);
    ballObjects[2]->setCOORD_initial(mainBoundary.getCenterX()-40,mainBoundary.getCenterY()+40);
    ballObjects[3]->setCOORD_initial(mainBoundary.getCenterX()+40,mainBoundary.getCenterY()+40);            
    ballObjects[4]->setCOORD_initial(mainBoundary.getCenterX(),mainBoundary.getCenterY()+80);    

    ////////////    
    while (window.isOpen())
    {
        while (const optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
                window.close();
        }
        //This must be called before any collision checks

        if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
        {
            window.close();
        }
        mainBoundary.rotate_polygon_and_map_to_SFML_Window();
        for(int i=0;i<ballObjects.size();i++){
            ballObjects[i]->callBallPhysicsFunctions(&mainBoundary,ballObjects,i);
        }

        window.clear();

        for(int i=0;i<ballObjects.size();i++){
            ballObjects[i]->drawBall(window);
        }        
        mainBoundary.drawPolygon(window);
        window.display();
//        sleep(milliseconds(5));
    }
    for(int i=0;i<ballObjects.size();i++){
        delete ballObjects[i];
    }

    return 0;
}
