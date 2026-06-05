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
    Texture batmanBallTexture;
    batmanBallTexture.loadFromFile("Data/bat.png");
///////////////////////////    
    N_Sided_Polygon_Boundary mainBoundary(4);
    BallBatman* batmanBallObj = new BallBatman(batmanBallTexture,&mainBoundary);

    batmanBallObj->setCOORD_initial(mainBoundary.getCenterX()-50,mainBoundary.getCenterY());

    while (window.isOpen())
    {
        while (const optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
                window.close();
        }
        //This must be called before any collision checks
        mainBoundary.rotate_polygon_and_map_to_SFML_Window();
        batmanBallObj->callBallPhysicsFunctions(&mainBoundary);

        window.clear();
        batmanBallObj->drawBall(window);
        mainBoundary.drawPolygon(window);
        window.display();
    }
    delete batmanBallObj;
}
