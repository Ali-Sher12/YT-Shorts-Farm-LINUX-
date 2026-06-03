#pragma once
using namespace sf;

class player
{

	const float rotate_speed = 1.5;
	float retarding_factor = 6;
	float hand_direction = 1;

public:

	int ray_no = 800;
	vector<Ray> rays;
	Ray Interacting_ray;

	float body_size = 25;

	float position_x = 500;
	float position_y = 900;

	float offset_x = 0;
	float offset_y = 0;

	bool coll_x = false;
	bool coll_y = false;

	bool up = 0;
	bool down = 0;
	bool A = 0;
	bool D = 0;
	
	float Radius = 20;
	float dir_v = 140;

	float angle = 0;

	bool interact_bool = false;
	
	player()
	{		
		int y_factor = 16;
		int x_factor = 512 / 32;

		rays.resize(ray_no);
		for (int i = 0; i < ray_no; i++)
			rays[i].RayPsuedo(dir_v);
	}

	void move_player()
	{
			if (Keyboard::isKeyPressed(Keyboard::Key::D))
			{		
				if(!coll_x)
				position_x -= (cos((angle * PI / 180) - PI / 2) * retarding_factor);

				if (!coll_y)
				position_y -= (sin((angle * PI / 180) + PI / 2) * retarding_factor);
				
				A = 0; D = 1; up = 0; down = 0;
			}

			else if (Keyboard::isKeyPressed(Keyboard::Key::A))
			{
				if (!coll_x)
				position_x += (cos((angle * PI / 180) - PI / 2) * retarding_factor);

				if (!coll_y)
				position_y -= (sin((angle * PI / 180) - PI / 2) * retarding_factor);

				A = 1; D = 0; up = 0; down = 0;
			}

			else if (Keyboard::isKeyPressed(Keyboard::Key::Up) || Keyboard::isKeyPressed(Keyboard::Key::W))
			{

				if(!coll_x)
				position_x += (cos(angle * PI / 180) * retarding_factor);

				if (!coll_y)
				position_y -= (sin(angle * PI / 180) * retarding_factor);

				A = 0; D = 0; up = 1; down = 0;
			}

			else if (Keyboard::isKeyPressed(Keyboard::Key::Down) || Keyboard::isKeyPressed(Keyboard::Key::S))
			{

				if (!coll_x)
				position_x -= (cos(angle * PI / 180) * retarding_factor);

				if (!coll_y)
				position_y += (sin(angle * PI / 180) * retarding_factor);

				A = 0; D = 0; up = 0; down = 1;
			}
				
			if (Keyboard::isKeyPressed(Keyboard::Key::Right))
			{
				angle += rotate_speed;
			}

			else if (Keyboard::isKeyPressed(Keyboard::Key::Left))
			{
				angle -= rotate_speed;
			}
			angle = ((int)angle %360);
	}

	void player_collision(Room* rooms , int& current_room)
	{
	
		coll_x = false;
		coll_y = false;
				
		if(D)
		{
			offset_x = position_x - (cos((angle * PI / 180) - PI / 2) * retarding_factor);
			offset_y = position_y - (sin((angle * PI / 180) + PI / 2) * retarding_factor);
		}
		
		else if(A)
		{
			offset_x = position_x + (cos((angle * PI / 180) - PI / 2) * retarding_factor);
			offset_y = position_y - (sin((angle * PI / 180) - PI / 2) * retarding_factor);
		}
		
		else if(up)
		{
			offset_x = position_x + (cos(angle * PI / 180) * retarding_factor);
			offset_y = position_y - (sin(angle * PI / 180) * retarding_factor);
		}
	
		else 
		{
			offset_x = position_x - (cos(angle * PI / 180) * retarding_factor);
			offset_y = position_y + (sin(angle * PI / 180) * retarding_factor);
		}
		//collision detection
		if (
			rooms[current_room].level_grid[(int)offset_x / cell_size][(int)position_y / cell_size] != ' '
			|| rooms[current_room].level_grid[(int)(offset_x + body_size) / cell_size][(int)position_y / cell_size] != ' '
			|| rooms[current_room].level_grid[(int)offset_x / cell_size][(int)(position_y + body_size) / cell_size] != ' '
			|| rooms[current_room].level_grid[(int)(offset_x + body_size) / cell_size][(int)(position_y + body_size) / cell_size] != ' '
		)
		{
			coll_x = true; 
		}
		if (
			rooms[current_room].level_grid[(int)position_x / cell_size][(int)offset_y / cell_size] != ' '
			|| rooms[current_room].level_grid[(int)(position_x + body_size) / cell_size][(int)offset_y / cell_size] != ' '
			|| rooms[current_room].level_grid[(int)position_x / cell_size][(int)(offset_y + body_size) / cell_size] != ' '
			|| rooms[current_room].level_grid[(int)(position_x + body_size) / cell_size][(int)(offset_y + body_size) / cell_size] != ' '
		)
		{
			coll_y = true;
		}
			
	}
	void player_collision_staticObj(vector<placables_*>& objects)
	{
		float playerRadius = body_size / 2;
		float objRadius = cell_size / 2;

		float testX = offset_x;
		float testY = offset_y;

		for (int i=0;i<objects.size();i++)
		{
			// Check X-axis collision
			float dx_x = objects[i]->x_coord - testX;
			float dy_x = objects[i]->y_coord - position_y;
			float dist_x = sqrt(dx_x * dx_x + dy_x * dy_x);

			if (dist_x < (playerRadius + objRadius))
			{
				coll_x = true;
			}

			// Check Y-axis collision
			float dx_y = objects[i]->x_coord - position_x;
			float dy_y = objects[i]->y_coord - testY;
			float dist_y = sqrt(dx_y * dx_y + dy_y * dy_y);

			if (dist_y < (playerRadius + objRadius))
			{
				coll_y = true;
			}
		}
	}
		
	void draw_player_vision(RenderWindow& window, Room* rooms , int& current_room ,vector<Sprite*> colS, float* zBuff)
	{
				
		float bod_size = 16;
		for(int i = 0 ; i < ray_no ; i += 1)
		{
			rays[i].ray_calculation(window, position_x + bod_size, position_y + bod_size, angle, rooms[current_room].level_grid, i,ray_no, colS,zBuff);
		}

	}

};


