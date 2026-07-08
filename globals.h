#pragma once
using namespace sf;
using namespace std;

#include "recorder.h"

///////////////////
int FPS = 60;
int screenWidth = 1155;
int screenHeight = 2200;
int PolygonSides = 5;
int total_balls = 3;
float rotationSpeed_Polygon = 0.085;
float rotationSpeed_ball = 0.025;//Superman rotation is changed from this
float global_delta_t = 0.65;
float e_global = 3.5;
float batarang_rotate_factor = 0.25;
float ball_terminal_velocity = 40;
float gravity_strength = 0.7;
bool gravity_true = false;
bool friction_enabled = false;
bool ball_to_ball_collision = true;
bool backgroundScroll = true;
bool screenTypeHoriz = true;
bool drawHealth = true;

float BGscrollVelocity = 20;
int dyin_frames = 60;
float web_rotate_factor = 0.05;
float dimention_reduction_death_anim = 1.7;
int hurt_frames = 10;
int appearanceFrames_super = 180;//can be reused for flames as well
int gapFrames_super = 200;
int appearanceFrames_bat = 300;
int gapFrames_bat = 150;
int appearanceFrames_spider = 300;
int gapFrames_spider = 150;
int postDeathIdleFrames = 100;
///////////////////
int total_balls_remaining = total_balls;
float PI = 3.14159;
Sound* ball_collide_sound, *wall_collide_sound,*swish_sound, *hurt_sound,*laserSound,*thwok_sound;
SoundBuffer ball_collide_buffer,wall_collide_buffer,hurt_buffer,swish_buffer,laserSoundBuffer,thwokSoundBuffer;
Texture* barTexture;
Sprite* barSprite;
float boundary_radius_for_healthBar = 0;
///////////////////
struct pair_custom{
    float x=0;
    float y=0;
    pair_custom(){}
    pair_custom(float a,float b)
    {
        x=a;y=b;
    }    
    void set(float a,float b)
    {
        x=a;y=b;
    }
};

float computeDotProduct(pair_custom A,pair_custom B)
{
    return A.x*B.x + A.y*B.y;
}
float computeMagnitude(pair_custom A)
{
    return sqrt(A.x*A.x + A.y*A.y);
}
pair_custom addPair(pair_custom A,pair_custom B)
{
    pair_custom temp;
    temp.set(A.x+B.x ,A.y+B.y);
    return temp;
}
pair_custom subPair(pair_custom A,pair_custom B)
{
    pair_custom temp;
    temp.set(A.x-B.x ,A.y-B.y);
    return temp;
}
pair_custom mulVecPair(float k ,pair_custom A)
{
    pair_custom temp;
    temp.set(A.x*k ,A.y*k);
    return temp;
}
pair_custom normalize(pair_custom v)
{
    float magnitude = computeMagnitude(v);
    return mulVecPair(1/magnitude, v);
}
float crossProduct(pair_custom A,pair_custom B)
{
    return A.x*B.y - A.y*B.x;
}
