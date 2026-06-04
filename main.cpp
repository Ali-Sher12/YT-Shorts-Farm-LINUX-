#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
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
    Texture batmanBallTexture, spiderBallTexture;    
    batmanBallTexture.loadFromFile("Data/bat.png");
    spiderBallTexture.loadFromFile("Data/spider.png");    
///////////////////////////    
    N_Sided_Polygon_Boundary mainBoundary(7);
    BallBatman* batmanBallObj = new BallBatman(batmanBallTexture);
    BallSpiderman* spiderBallObj = new BallSpiderman(spiderBallTexture);    

    batmanBallObj->setCOORD_initial(mainBoundary.getCenterX(),mainBoundary.getCenterY());
    spiderBallObj->setCOORD_initial(mainBoundary.getCenterX()+200,mainBoundary.getCenterY());    

    while (window.isOpen())
    {
        while (const optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
                window.close();
        }

        window.clear();
        batmanBallObj->drawBall(window);
        spiderBallObj->drawBall(window);        
        mainBoundary.drawPolygon(window);
        window.display();
    }
    delete batmanBallObj;
    delete spiderBallObj;    
}
