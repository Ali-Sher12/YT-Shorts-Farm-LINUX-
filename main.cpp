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

/////////////////////////// Texture setup
    Texture batmanBallTexture,spiderBallTexture,superBallTexture,hulkBallTexture;
    batmanBallTexture.loadFromFile("Data/bat.png");
    superBallTexture.loadFromFile("Data/sup.png");
    spiderBallTexture.loadFromFile("Data/spider.png");
    hulkBallTexture.loadFromFile("Data/hulk.png");    
    ///////////////////////////    
    N_Sided_Polygon_Boundary mainBoundary(4);
    vector<BallClass*> ballObjects;
    ballObjects.resize(4);
    ballObjects[0] = new BallBatman(batmanBallTexture,50);
    ballObjects[1] = new BallSuper(superBallTexture,60);
    ballObjects[2] = new BallSpider(spiderBallTexture,50);
    ballObjects[3] = new BallHulk(hulkBallTexture,60);

    ballObjects[0]->setCOORD_initial(mainBoundary.getCenterX()-40,mainBoundary.getCenterY()-40);
    ballObjects[1]->setCOORD_initial(mainBoundary.getCenterX()+40,mainBoundary.getCenterY()-40);
    ballObjects[2]->setCOORD_initial(mainBoundary.getCenterX()-40,mainBoundary.getCenterY()+40);
    ballObjects[3]->setCOORD_initial(mainBoundary.getCenterX()+40,mainBoundary.getCenterY()+40);            

    while (window.isOpen())
    {
        while (const optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
                window.close();
        }
        //This must be called before any collision checks
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
//        sleep(milliseconds(500));
    }
    
}
