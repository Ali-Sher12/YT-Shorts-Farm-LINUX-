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

/////////////////////////// sample
    Texture t;
    t.loadFromFile("pak.png");
    BallPakistan* b = new BallPakistan(t);
/////////////////////////// sample
    N_Sided_Polygon_Boundary mainBoundary(5);

    while (window.isOpen())
    {
        while (const optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
                window.close();
        }

        window.clear();
        b->drawBall(window);
        mainBoundary.drawPolygon(window);
        window.display();
    }
}