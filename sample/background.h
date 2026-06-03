#pragma once
using namespace std;
using namespace sf;

class skyBox 
{
public:
	Sprite* skySpr;
	Texture skyTex;
//	float fov;
	skyBox() {}
	void skyBox_Psuedo(string name,float player_fov) 
	{	
		skyTex.loadFromFile("Data/skyBoxes/" + name + ".png");
		skySpr = new Sprite(skyTex);
		skySpr->setScale({2.5,2.5});
//		fov = player_fov;
	}
	void draw_skyBox(RenderWindow& window, float angle)
	{

		window.draw(*skySpr);
		//int texWidth = skyTex.getSize().x;
		//int texMask = (int)((angle / 360) * texWidth);

		//if (texMask + SCREEN_WIDTH > texWidth)
		//{
		//	int part1Width = texWidth - texMask;
		//	int part2Width = SCREEN_WIDTH - part1Width;

		//	Sprite skySpr1(skyTex);
		//	skySpr1.setTextureRect(IntRect({ texMask, 0 }, { part1Width, (int)(SCREEN_HEIGHT / 2) }));
		//	skySpr1.setPosition({ 0, 0 });
		//	window.draw(skySpr1);

		//	Sprite skySpr2(skyTex);
		//	skySpr2.setTextureRect(IntRect({ 0, 0 }, { part2Width, (int)(SCREEN_HEIGHT / 2) }));
		//	skySpr2.setPosition({ (float)part1Width, 0});
		//	window.draw(skySpr2);
		//}
		//else
		//{
		//	Sprite skySpr(skyTex);
		//	skySpr.setTextureRect(IntRect({ texMask, 0 }, { (int)SCREEN_WIDTH, (int)(SCREEN_HEIGHT / 2) }));
		//	skySpr.setPosition({ 0, 0 });
		//	window.draw(skySpr);
		//}
	}
	~skyBox() 
	{
		delete skySpr;
	}

};
