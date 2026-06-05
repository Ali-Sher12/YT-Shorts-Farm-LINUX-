#pragma once
using namespace sf;
using namespace std;
float screenHeight = 1080;
float screenWidth = 608;
float PI = 3.14159;
float e = 1;
struct pair_custom{
    float x=0;
    float y=0;
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

