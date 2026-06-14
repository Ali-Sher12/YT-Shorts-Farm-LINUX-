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
    float division_factor = 3;
    float factorX = 60;
    float factorY = -50;    
    float profileHeight = 180;
    float profileWidth = 180;
    float spacing  = 30;  
    RectangleShape* rect;  
    int index_self = 0;
    public:
    HealthBar(string hero){
        rect = new RectangleShape({20, 80});
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
        healthBar_index_self++;
        index_self = healthBar_index_self;
        if(screenTypeHoriz){
            profileSprite->setPosition({2*boundary_radius_for_healthBar+factorX,healthBar_index_self*screenHeight/division_factor +factorY});
        }        
        else{
            profileSprite->setPosition({healthBar_index_self*screenWidth/division_factor +factorX,2*boundary_radius_for_healthBar + factorY});
        }
    }
    void drawAll(RenderWindow& window){
        window.draw(*profileSprite);
        for(int i=0;i<health;i++){
            if(screenTypeHoriz){
                rect->setPosition({(2*boundary_radius_for_healthBar+factorX)+profileWidth+(i+1)*spacing,index_self*screenHeight/division_factor +factorY + 30});
            }        
            else{
                rect->setPosition({(index_self*screenWidth/division_factor +factorX)+profileWidth+(i+1)*spacing,2*boundary_radius_for_healthBar + factorY + 30});
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