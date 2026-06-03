#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include "globals.h"
#include "boundaries.h"
#include "ballClass.h"

using namespace std;
using namespace sf;

int main()
{
    RenderWindow window(VideoMode({608, 1080}), "Ball Farm");

/////////////////////////// sample
    Texture t;
    t.loadFromFile("pak.png");
    BallPakistan* b = new BallPakistan(t);
/////////////////////////// sample

    while (window.isOpen())
    {
        while (const optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
                window.close();
        }

        window.clear();
        b->drawBall(window);
        window.display();
    }
}