#pragma once
using namespace sf;
using namespace std;

///////////////////
int screenWidth = 1000;
int screenHeight = 650;
int PolygonSides = 6;
int total_balls = 2;
float rotationSpeed_Polygon = 0.055;
float rotationSpeed_ball = 0.04;//Superman rotation is changed from this
float global_delta_t = 0.03;
float e_global = 1.2;
float ball_terminal_velocity = 15;
float gravity_strength = 0.4;
bool gravity_true = true;
bool friction_enabled = false;
bool ball_to_ball_collision = true;
bool backgroundScroll = true;
///////////////////

float PI = 3.14159;
Sound* ball_collide_sound, *wall_collide_sound,*swish_sound, *hurt_sound,*laserSound;
SoundBuffer ball_collide_buffer,wall_collide_buffer,hurt_buffer,swish_buffer,laserSoundBuffer;

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

