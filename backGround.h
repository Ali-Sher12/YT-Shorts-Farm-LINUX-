#pragma once
using namespace sf;
using namespace std;

class BackGround{
    Sprite* backGroundSprite;
    Texture* backGroundTexture;
    float backgroundX_alpha,backgroundX_beta;
    float backgroundY;    

    public:    
    BackGround(){
        backGroundTexture = new Texture("Data/Images/sky.png");
        backGroundSprite = new Sprite(*backGroundTexture);
        backGroundSprite->setScale({((float)screenWidth/backGroundTexture->getSize().x),((float)screenHeight/backGroundTexture->getSize().y)});
        backgroundX_alpha = 0; backgroundY = 0;
        backgroundX_beta = -screenWidth;        
    }
    private:
    void scroll(){
        if(backgroundScroll){
            backgroundX_alpha+=BGscrollVelocity;
            backgroundX_beta+=BGscrollVelocity;
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
