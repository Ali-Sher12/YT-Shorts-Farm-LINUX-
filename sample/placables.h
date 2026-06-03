#pragma once
using namespace std;
using namespace sf;
class placables_
{
public:
	vector<Texture> Textures;
	int total_frames = 1;
	float x_coord = 900;
	float y_coord = 900;

	float objRadius = 20;
	float angle_rad = 0;
	float distanceToObj = 0;
	float relativeAngle = 0;
	float angleToObj = 0;
	float spriteHeight = 1;
	float spriteWidth = 1;
	float TexHeight = 1;
	float TexWidth = 1;
	float distanceToProjPlane = 5;
	float pitch = 0;

	float x_pos = 0;
	float y_pos = 0;
	float y_factor = -5;

	float perpDist = 0;
	float facing_angle = PI/3;// in radians
	placables_() {}
	virtual void placables_psudeo(float dir_v, string folder) = 0;
	virtual void placables_psudeo(float dir_v,string folder,int total_sprites,float f_angle)
	{
		facing_angle = f_angle;
		total_frames = total_sprites;

		pitch = tan((90 - dir_v) * PI / (180 * 3)) * 1000;
		Textures.resize(total_frames);
		for (int i = 0; i < total_frames; i += 1)
		{
			Textures[i].loadFromFile("Data/placables/"+folder+"/" + to_string(i+1) + ".png");
		}
		distanceToProjPlane =
			(SCREEN_WIDTH * 0.5f) / tan(FOV * 0.5f);

	}

	void setCOORD(float X, float Y)
	{
		x_coord = X;
		y_coord = Y;
	}


	bool isPresentInFOV(float playerX, float playerY, float player_angle)
	{
		float dx = x_coord - playerX;
		float dy = y_coord - playerY;
		distanceToObj = sqrt(dx * dx + dy * dy);
		angleToObj = atan2(-dy, dx);  // Negative dy
		relativeAngle = normalizeAngle(angleToObj - player_angle);
		if (fabs(relativeAngle) <= (FOV / 2.0) + PI / 12)
			return true;
		return false;
	}

	void draw_placable(RenderWindow& window, float* zBuff, int size_of_buffer, float playerX, float playerY, float player_angle)
	{//call after draw_vision

		perpDist = distanceToObj * cos(relativeAngle);
		if (perpDist <= 0.001f) return;

		spriteHeight = Textures[0].getSize().y / perpDist * distanceToProjPlane;
		spriteWidth = spriteHeight/Textures[0].getSize().y * Textures[0].getSize().x;

		// Fixed calculation:
		float screenX = tan(relativeAngle) * distanceToProjPlane + SCREEN_WIDTH / 2;
		x_pos = screenX - spriteWidth / 2;
		y_pos = y_factor + (SCREEN_HEIGHT / 2) - (spriteHeight / 1000) + pitch / 3;
		if (perpDist >= render_distance_wall)
			return;
//		draw_zBuffMethod(window,getSpriteIndex(playerX,playerY,player_angle),zBuff,size_of_buffer);
		draw_zBuffMethod(window, getSpriteIndex(), zBuff, size_of_buffer);
	}

	virtual bool collideWithPlayer(float playerX, float playerY, float playerRadius)
	{
		float dx = x_coord - playerX;
		float dy = y_coord - playerY;
		float distance = sqrt(dx * dx + dy * dy);

		return distance < (playerRadius + objRadius);
	}

	virtual int getSpriteIndex()// fix this function
	{
		float relativeViewAngle = normalizeAngle(angleToObj - facing_angle);
		float segmentSize = (2 * PI) / total_frames;
		int spriteIndex = (int)((relativeViewAngle + PI + segmentSize / 2) / segmentSize) % total_frames;
		if (spriteIndex < 0) spriteIndex += total_frames;
		if (spriteIndex >= total_frames) spriteIndex = total_frames - 1;

		return spriteIndex;
	}

	void draw_zBuffMethod(RenderWindow& window, int spriteInd, float* zBuff, int size_of_buffer)
	{
		int drawStartX = (int)x_pos;
		int drawEndX = (int)(x_pos + spriteWidth);

		// Clamp to screen bounds
		if (drawStartX < 0) drawStartX = 0;
		if (drawEndX >= size_of_buffer) drawEndX = size_of_buffer;

		int color_smthng = min(255, static_cast<int>(300 - (perpDist / render_distance) * 255));
		int color_val = (0 > color_smthng) ? 0 : color_smthng;

		for (int stripe = drawStartX; stripe < drawEndX; stripe++)
		{
			if (perpDist < zBuff[stripe])
			{
				zBuff[stripe] = perpDist;
				int texX = (int)((stripe - (int)x_pos) * Textures[spriteInd].getSize().x / spriteWidth);
				// Clamp texture coordinate
				if (texX < 0) texX = 0;
				if (texX >= (int)Textures[spriteInd].getSize().x)
					texX = Textures[spriteInd].getSize().x - 1;

				Sprite column(Textures[spriteInd]);
				column.setTextureRect(IntRect({ texX, 0 }, { 1, (int)Textures[spriteInd].getSize().y }));
				column.setPosition({ (float)stripe, (float)y_pos });
				column.setScale({ 1, spriteHeight / Textures[spriteInd].getSize().y });
				column.setColor(sf::Color(color_val, color_val, color_val, 255));
				window.draw(column);
			}
		}
	}


	float normalizeAngle(float angle)
	{
		float TWO_PI = 2.0f * PI;
		angle -= TWO_PI * floor((angle + PI) / TWO_PI);
		return angle;
	}

};

class Decorators : public placables_
{// static placables that have only one angle (animated or not) and are non-colliding
public:
	void Decorators_psudeo(float dir_v, string folder) 
	{
		placables_::placables_psudeo(dir_v,folder,1,0);
	}

	int getSpriteIndex() 
	{
		return 0;
	}

	bool collideWithPlayer(float playerX, float playerY, float playerRadius) 
	{
		return false;
	}

};

class Decorators_Physical : public placables_
{// static placables that have only one angle (animated or not) and are colliding
public:
	void Decorators_Physical_psudeo(float dir_v, string folder)
	{
		placables_::placables_psudeo(dir_v, folder, 1, 0);
	}

	int getSpriteIndex()
	{
		return 0;
	}

};

class three_candle : public Decorators
{// static placables that have only one angle (animated or not) and are non-colliding
public:
	void placables_psudeo(float dir_v, string folder)
	{
		Decorators::Decorators_psudeo(dir_v, folder);
	}

};

class dead_guy : public placables_
{// static placables that have only one angle (animated or not) and are non-colliding
public:
	void placables_psudeo(float dir_v, string folder)
	{
		y_factor = -10;
		objRadius = 50;
		placables_::placables_psudeo(dir_v, folder,5,PI/2);
	}

};

class Barrel : public Decorators_Physical
{// static placables that have only one angle (animated or not) and are non-colliding
public:
	void placables_psudeo(float dir_v, string folder)
	{
		objRadius = 40;
		Decorators_Physical::Decorators_Physical_psudeo(dir_v, folder);
	}

};

class BloodyHeart : public Decorators_Physical
{// static placables that have only one angle (animated or not) and are non-colliding
public:
	void placables_psudeo(float dir_v, string folder)
	{
		objRadius = 40;
		Decorators_Physical::Decorators_Physical_psudeo(dir_v, folder);
	}

};
