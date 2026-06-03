#pragma once
using namespace std;
using namespace sf;

// 'V' is invisible blocking wall
vector<char> catchLevelIdentifiers() {
	return { '@', 'h', '#', 'w', 'd', 'g', 'z' };
}

class Ray
{
public:

	//start points

	float start_x = 0;
	float start_y = 0;

	float startX_usage = 0;
	float startY_usage = 0;

	//the angle
	float angle_ray = 315;

	//size of the ray end point
	float singular_size = 2;

	//how far the ray can see
	float usage_distance = 60;

	//6 pixels (horizontal) per ray
	float distance = 0;

	float col_size = 1;

	vector<bool> coll;
	vector<char> identifier;

	char check_usage;

	float y_sc = 0;
	float y_dis = 0;

	float color_val = 0;

	float ray_separation = 0;//keep this small to prevent abnormal latching
	float perpWallDist = 0;
	//VertexArray line;
	//Vector2f p1;
	//Vector2f p2;

	float texture_map_mask_x = 0;
	float texture_map_mask_y = 0;
	float texture_height = cell_size;

	float y_up = 300;
	float pitch = 0;

	char check_a;

	Ray() {}

	void RayPsuedo(float dir_v)
	{
		//VertexArray l(Lines, 2);
		//line = l;
		coll.resize(total_wall_textures_per_level);
		identifier = catchLevelIdentifiers();
		pitch = tan((90 - dir_v) * PI / (180 * 3)) * 1000;
	}

	void ray_calculation(RenderWindow& window, float startX, float startY, float angle, char** level_grid, int& x_fac, int& max, vector<Sprite*>& colS, float* zBuff)
	{
		start_x = startX;
		start_y = startY;

		angle_ray = angle + (x_fac - max / 2) * (FOV * 180 / PI / max);

		for (int i = 0; i < total_wall_textures_per_level; i++)
		{
			coll[i] = false;
		}

		float cos_precomp = cos(angle_ray * PI / 180);
		float sin_precomp = -1 * sin(angle_ray * PI / 180);

		//DDA setup
		int mapX = (int)(start_x / cell_size);
		int mapY = (int)(start_y / cell_size);

		float deltaDistX = (cos_precomp == 0) ? numeric_limits<float>::max() : fabs(cell_size / cos_precomp);
		float deltaDistY = (sin_precomp == 0) ? numeric_limits<float>::max() : fabs(cell_size / sin_precomp);

		int stepX, stepY;
		float sideDistX, sideDistY;

		if (cos_precomp < 0)
		{
			stepX = -1;
			sideDistX = (start_x - mapX * cell_size) * deltaDistX / cell_size;
		}
		else
		{
			stepX = 1;
			sideDistX = ((mapX + 1) * cell_size - start_x) * deltaDistX / cell_size;
		}

		if (sin_precomp < 0)
		{
			stepY = -1;
			sideDistY = (start_y - mapY * cell_size) * deltaDistY / cell_size;
		}
		else
		{
			stepY = 1;
			sideDistY = ((mapY + 1) * cell_size - start_y) * deltaDistY / cell_size;
		}

		//DDA loop
		int hitSide = -1; // 0 = vertical, 1 = horizontal
		bool hit = false;
		while (!hit)
		{
			if (sideDistX < sideDistY)
			{
				sideDistX += deltaDistX;
				mapX += stepX;
				hitSide = 0;
			}
			else
			{
				sideDistY += deltaDistY;
				mapY += stepY;
				hitSide = 1;
			}

			if (hitSide == 0)
				perpWallDist = (mapX - startX / cell_size + (1 - stepX) / 2.0f) * cell_size / cos_precomp;
			else
				perpWallDist = (mapY - startY / cell_size + (1 - stepY) / 2.0f) * cell_size / sin_precomp;


			if (fabs(perpWallDist) >= render_distance_wall)
			{
				zBuff[x_fac] = render_distance_wall;
				return;
			}

			check_a = level_grid[mapX][mapY];

			for (int i = 0; i < total_wall_textures_per_level; i++)
			{
				if (check_a == identifier[i])
				{
					coll[i] = true;
					hit = true;
					break;
				}
				else if (check_a == 'V')
				{
					hit = true;
					break;
				}
			}
		}

		//Compute exact wall hit coordinates

		start_x = startX + cos_precomp * perpWallDist;
		start_y = startY + sin_precomp * perpWallDist;

		distance = perpWallDist * cos((angle_ray - angle) * PI / 180.0f);

		// --- Compute fractional wall hit for texture mapping ---
		float wallX = 0;
		if (hitSide == 0)
			wallX = start_y / cell_size;
		else
			wallX = start_x / cell_size;

		wallX -= floor(wallX); // fractional part
		texture_map_mask_x = int(wallX * 64);
		texture_map_mask_y = texture_map_mask_x;

		int color_smthng = min(255, static_cast<int>(255 - (distance / render_distance) * 255));
		color_val = (0 > color_smthng) ? 0 : color_smthng;

		y_sc = 1500 / distance;
		y_dis = 500 - (25 * y_sc) + pitch; // original formula

		for (int i = 0; i < total_wall_textures_per_level; i++)
		{
			if (coll[i])
			{
				colS[i]->setTextureRect(IntRect({ (int)texture_map_mask_y, 0 }, { 1, (int)texture_height }));
				colS[i]->setPosition({ (x_fac * col_size), y_dis });
				colS[i]->setScale({ 1, y_sc });
				colS[i]->setColor(sf::Color(color_val, color_val, color_val, 255));
				window.draw(*colS[i]);
				zBuff[x_fac] = distance;
				break;
			}
		}
	}

};


