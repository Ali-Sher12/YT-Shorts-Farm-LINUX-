#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include "globals.h"
#include "backGround.h"
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
    wall_collide_sound->setVolume(60);    
/////////////////////////// Texture setup
    Texture batmanBallTexture,spiderBallTexture,superBallTexture,hulkBallTexture,invincibleBallTexture;
    batmanBallTexture.loadFromFile("Data/Images/bat.png");
    superBallTexture.loadFromFile("Data/Images/sup.png");
    spiderBallTexture.loadFromFile("Data/Images/spider.png");
    hulkBallTexture.loadFromFile("Data/Images/hulk.png");    
    invincibleBallTexture.loadFromFile("Data/Images/inv.png");
///////////////////////////    
    N_Sided_Polygon_Boundary mainBoundary(PolygonSides);
    vector<BallClass*> ballObjects;
    ballObjects.resize(total_balls);
    ballObjects[0] = new BallBatman(batmanBallTexture,50,-10,10);
    ballObjects[1] = new BallSuper(superBallTexture,60,10,-10);

    ballObjects[0]->setCOORD_initial(mainBoundary.getcenterPair().x-40,mainBoundary.getcenterPair().y);
    ballObjects[1]->setCOORD_initial(mainBoundary.getcenterPair().x+40,mainBoundary.getcenterPair().y);

    BackGround background;
///////////////////////////
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
        for(int i=0;i<total_balls;i++){
            ballObjects[i]->callBallPhysicsFunctions(&mainBoundary,ballObjects,i);
        }

        window.clear();

        // background.drawBackground(window);
        for(int i=0;i<total_balls;i++){
            ballObjects[i]->drawBall(window);
        }        
        ballObjects[0]->callBatarangFunctions(ballObjects,window);
        mainBoundary.drawPolygon(window);
        window.display();
        // sleep(milliseconds(1));
    }
    for(int i=0;i<total_balls;i++){
        delete ballObjects[i];
    }

    return 0;
}
