#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#pragma once
using namespace std;
using namespace sf;

//How do I even set up the boundary?
//How do I then check its collision with the balls?
//How do I make the boundary move and rotate?
//How do I have cavities inside it? How do I set textures on it?

struct pair_custom{
    float x=0;
    float y=0;
    void set(float a,float b)
    {
        x=a;y=b;
    }
};

class N_Sided_Polygon_Boundary{

    vector<pair_custom> vertices_original;
    vector<pair_custom> vertices_transformed_and_mapped;    
    float centerX = screenWidth/2; // The coordinates
    float centerY = screenHeight/2-200;    
    float radius = 200;
    float rotation_angle = 0;
    float rotation_speed = 0.02;
    float total_vertices = 0;    
    float delta_t = 0.01;        
    
    ConvexShape polygon;
    public:
    N_Sided_Polygon_Boundary(int N)
    {
        polygon.setPointCount(N);
        polygon.setFillColor(Color::Transparent);
        polygon.setOutlineColor(Color::Red);
        polygon.setOutlineThickness(6);

        total_vertices = N;    
        defineVertices_psuedotype(N);
    }

    private:
    void defineVertices_psuedotype(int N)
    {
        //get the corner points for the N-sided-polygon
        float val1 = 6.28319/N;
        for(int i=0; i<N; i++)
        {
            float val2 = val1*i;
            pair_custom temp_pair;
            temp_pair.set(radius*cos(val2),radius*sin(val2));
            vertices_original.push_back(temp_pair);
            vertices_transformed_and_mapped.push_back(temp_pair);
        }
    }

    void rotate_polygon_and_map_to_SFML_Window()
    {
        //rotation_speed>0 -> clockwise
        //rotation_speed<0 -> counter-clockwise
        rotation_angle +=(rotation_speed*delta_t);
        rotation_angle>2*PI?0:rotation_angle;
        for(int i=0; i<total_vertices; i++)
        {
            vertices_transformed_and_mapped[i].set(
            vertices_original[i].x*cos(rotation_angle)-vertices_original[i].y*sin(rotation_angle)+centerX,
            vertices_original[i].x*sin(rotation_angle)+vertices_original[i].y*cos(rotation_angle)+centerY);
            polygon.setPoint(i, Vector2f(vertices_transformed_and_mapped[i].x,vertices_transformed_and_mapped[i].y));            
        }
    }

    public:
    void drawPolygon(RenderWindow& window)
    {
        rotate_polygon_and_map_to_SFML_Window();
        window.draw(polygon);
    }

};