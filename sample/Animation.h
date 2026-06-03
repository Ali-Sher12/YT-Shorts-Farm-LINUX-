#pragma once

using namespace std;
using namespace sf;

class Animation
{

    vector<Texture> Tex;
    vector<Sprite*> Sprites;
    int total_sprites = 0;
    int cycles = 0;
    Clock myclk;

public:
    bool reverse = false;
    int time = 60;

    Animation() {}

    void Psudeo_Constructor(int totalSpr, string tex_path,bool rvr)
    {// This must be called in constructor of a constructor
        reverse = rvr;
        reverse ? cycles = total_sprites-1 : cycles = 0;
        total_sprites = totalSpr;

        int n = 0;
        for (int i = 0; i < total_sprites; i++)
        {
            Tex[i].loadFromFile(tex_path);
            Sprites[i] = new Sprite(Tex[i]);
        }

    }

    void Draw(RenderWindow& window, float x, float y,float scaleX,float scaleY)
    {
        if (myclk.getElapsedTime().asMilliseconds() > time)
        {
            myclk.restart();

            reverse?cycles-=1:cycles += 1;
        }


        if (!reverse && cycles >= total_sprites)
            cycles = 0;
        else if (reverse && cycles < 0)
            cycles = total_sprites-1;


        Sprites[cycles]->setPosition({ x, y });
        Sprites[cycles]->setScale({ scaleX, scaleY });
        window.draw(*Sprites[cycles]);

    }

    ~Animation() 
    {
        for(int i=0;i<total_sprites;i++)
            delete[]Sprites[i];
    }

};

