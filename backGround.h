#pragma once
using namespace sf;
using namespace std;

class BackGround{
    Sprite* backGroundSprite;
    Texture* backGroundTexture;
    float backgroundX_alpha,backgroundX_beta;
    float backgroundY;    
    float scrollVelocity = 0.2;

    public:    
    BackGround(){
        backGroundTexture = new Texture("Data/Images/sky.png");
        backGroundSprite = new Sprite(*backGroundTexture);
        backGroundSprite->setScale({(screenWidth/backGroundTexture->getSize().x),(screenHeight/backGroundTexture->getSize().y)});
        backgroundX_alpha = 0; backgroundY = 0;
        backgroundX_beta = -screenWidth;        
    }
    private:
    void scroll(){
        if(backgroundScroll){
            backgroundX_alpha+=scrollVelocity;
            backgroundX_beta+=scrollVelocity;
            if(backgroundX_alpha>=screenWidth)backgroundX_alpha=-screenWidth;
            else if(backgroundX_beta>=screenWidth)backgroundX_beta=-screenWidth;
        }
    }
    public:
    void drawBackground(RenderWindow& window){
        scroll();
        backGroundSprite->setPosition({backgroundX_alpha,backgroundY});
        window.draw(*backGroundSprite);
        if(backgroundScroll){
            backGroundSprite->setPosition({backgroundX_beta,backgroundY});
            window.draw(*backGroundSprite);
        }
    }
    ~BackGround(){
        delete backGroundSprite;
        delete backGroundTexture;
    }
};
