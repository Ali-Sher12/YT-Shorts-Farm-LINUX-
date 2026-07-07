#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "globals.h"
#pragma once
using namespace std;
using namespace sf;

class HealthBar{
    Texture* profileTexture;
    Sprite* profileSprite;
    int health = 0;
    float division_factor = 6;
    float factorX = 150;
    float factorY = 30;    
    float profileHeight = 110;
    float profileWidth = 110;
    float spacing  = 20;  
    RectangleShape* rect;  
    int index_self = 0;
    public:
    HealthBar(string hero){
        rect = new RectangleShape({10, 40});
        rect->setFillColor(Color::Green);
        profileTexture = new Texture("Data/Images/"+hero+".png");
        profileSprite = new Sprite(*profileTexture);
        profileSprite->setScale({profileWidth/profileTexture->getSize().x,profileHeight/profileTexture->getSize().y});        
    }
    void setHealth(int health_){
        health = health_;        
    }
    void setHeight(float H){
        profileHeight = H;
        profileSprite->setScale({profileWidth/profileTexture->getSize().x,H/profileTexture->getSize().y});
    }
    void setWidth(float W){
        profileWidth = W;
        profileSprite->setScale({W/profileTexture->getSize().x,profileHeight/profileTexture->getSize().y});
    }
    void setPosition(int healthBar_index_self){
        index_self = healthBar_index_self;
        if(screenTypeHoriz){
            profileSprite->setPosition({(2 * boundary_radius_for_healthBar + 20),healthBar_index_self*(profileWidth + factorY + 50)});
        }        
        else{
            profileSprite->setPosition({factorX,(2 * boundary_radius_for_healthBar + 130) + healthBar_index_self*(profileHeight +factorY)});
        }
    }
    void drawAll(RenderWindow& window){
        window.draw(*profileSprite);
        for(int i=0;i<health;i++){
            if(screenTypeHoriz){
                rect->setPosition({(2 * boundary_radius_for_healthBar + 20) + (20 * i) + (profileWidth + 10),index_self*(profileWidth + factorY + 20)});
            }        
            else{
                rect->setPosition({factorX  + (20 * i) + (profileWidth + 10),(2 * boundary_radius_for_healthBar + 130) + index_self*(profileHeight +factorY)+ 20});
            }
            window.draw(*rect);
        }
    }
    ~HealthBar(){
        delete rect;
        delete profileSprite;
        delete profileTexture;
    }

};