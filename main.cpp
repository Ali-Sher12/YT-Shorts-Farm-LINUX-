#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include "globals.h"
#include "backGround.h"
#include "healthBar.h"
#include "boundaries.h"
#include "ballClass.h"

using namespace std;
using namespace sf;

int main()
{
    RenderWindow window(VideoMode({screenWidth, screenHeight}), "BallFarm");
    window.setFramerateLimit(FPS);

    thwokSoundBuffer.loadFromFile("Data/Audio/web.ogg");
    ball_collide_buffer.loadFromFile("Data/Audio/ball_collision.ogg");
    wall_collide_buffer.loadFromFile("Data/Audio/wall_collision.ogg");    
    swish_buffer.loadFromFile("Data/Audio/swish.ogg");
    hurt_buffer.loadFromFile("Data/Audio/hurt.ogg");        
    laserSoundBuffer.loadFromFile("Data/Audio/laser.wav");    
    laserSoundBuffer_home.loadFromFile("Data/Audio/laser_home.ogg");        
    ball_collide_sound = new Sound(ball_collide_buffer);
    wall_collide_sound = new Sound(wall_collide_buffer);
    hurt_sound = new Sound(hurt_buffer);
    swish_sound = new Sound(swish_buffer);    
    laserSound = new Sound(laserSoundBuffer);        
    laserSound_home = new Sound(laserSoundBuffer_home);            
    thwok_sound = new Sound(thwokSoundBuffer);            
    ball_collide_sound->setVolume(100);
    swish_sound->setVolume(100);
    hurt_sound->setVolume(100);        
    wall_collide_sound->setVolume(60); 
    laserSound->setVolume(160);       
    laserSound_home->setVolume(160);       

    thwok_sound->setVolume(70);           
/////////////////////////// Texture setup
    Texture homelanderBallTexture,batmanBallTexture,spiderBallTexture,superBallTexture,hulkBallTexture,invincibleBallTexture;
    batmanBallTexture.loadFromFile("Data/Images/bat.png");
    superBallTexture.loadFromFile("Data/Images/sup.png");
    spiderBallTexture.loadFromFile("Data/Images/spider.png");
    hulkBallTexture.loadFromFile("Data/Images/hulk.png");    
    invincibleBallTexture.loadFromFile("Data/Images/inv.png");
    homelanderBallTexture.loadFromFile("Data/Images/homelander.png");
///////////////////////////    
    N_Sided_Polygon_Boundary mainBoundary(PolygonSides);
    vector<BallClass*> ballObjects;
    total_balls = 2;
    ballObjects.resize(total_balls);

    ballObjects[0] = new BallSuper(superBallTexture,140,0,100);    
    ballObjects[1] = new BallHomelander(homelanderBallTexture,140,-100,-100);

    ballObjects[0]->setCOORD_initial(mainBoundary.getcenterPair().x,mainBoundary.getcenterPair().y);
    ballObjects[1]->setCOORD_initial(mainBoundary.getcenterPair().x-100,mainBoundary.getcenterPair().y-100);


    BackGround background;
    Music music("Data/Audio/song3.ogg");
    music.setVolume(40);
    music.play();
    music.isLooping();
///////////////////////////
    while (window.isOpen())
    {
        while (const optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
                window.close();
        }
        //This must be called before any collision checks

        if (Keyboard::isKeyPressed(Keyboard::Key::N))
            while(1)
                if (Keyboard::isKeyPressed(Keyboard::Key::M))  
                    break;

        if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
        {
            window.close();
        }
        mainBoundary.rotate_polygon_and_map_to_SFML_Window();
        for(int i=0;i<total_balls;i++){
            ballObjects[i]->callBallPhysicsFunctions(&mainBoundary,ballObjects,i);
        }

        window.clear();

        background.drawBackground(window);
        for(int i=0;i<total_balls;i++){
            ballObjects[i]->drawBall(window);
        }        
        mainBoundary.drawPolygon(window);
        for(int i=0;i<total_balls;i++){
            ballObjects[i]->callHeroFunctions(&mainBoundary, ballObjects,window);
        }        
        window.display();
        if(total_balls_remaining<=1)window.close();
    }
    music.stop();
    for(int i=0;i<total_balls;i++){
        delete ballObjects[i];
    }
    delete ball_collide_sound;
    delete wall_collide_sound;
    delete swish_sound;
    delete thwok_sound;    
    delete hurt_sound;
    delete laserSound;
    delete laserSound_home;    
    delete barSprite;
    delete barTexture;

    return 0;
}
