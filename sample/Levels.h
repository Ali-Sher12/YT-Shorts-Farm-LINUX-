#pragma once

using namespace sf;
using namespace std;


class VHSFilter
{

	Texture filter[86];
	Sprite* effect[86];

	string initial = "Data/VHS/";
	string final = ".png";
	string num;
	Clock animation_clock;
	int current = 0;
	int total = 86;
	Music noise;

public:

	VHSFilter()
	{
		string helper;

		for(int i=0;i<total;i+=1)
		{
			helper = initial + to_string(i + 1) + final;
			filter[i].loadFromFile(helper);
			effect[i] = new Sprite(filter[i]);
			effect[i]->setPosition({ 0,0 });
			effect[i]->setScale({1.9, 2.7});
		}
		noise.openFromFile("Data/Sound/noise.ogg");
		noise.setLooping(true);
		noise.setVolume({ 20 });
		noise.play();
	}

	void apply_filter(RenderWindow& window)
	{
		if (animation_clock.getElapsedTime().asMilliseconds() > 20)
		{
			animation_clock.restart();
			current += 1;
		}
	
		if (current == total)
			current = 0;

		window.draw(*effect[current]);

	}
	~VHSFilter()
	{
		noise.stop();
		for (int i = 0; i < 86; i++)
			delete[]effect[i];
	}
};

class level
{

	Texture barT;
	Sprite* bar;
	Texture meT;
	Sprite* me;
	Texture mouseTex;
	Sprite* mouseSpr;
	skyBox sky;
	vector<Texture> colT;
	vector<Sprite*> colS;

	vector<placables_*> objS;

	Room level_rooms[2];

//	VHSFilter vhs;

	int current_room = 0;
	player ali;
	float* zBuffer;
	Music lvlMus;
public:


	level()
	{
		sky.skyBox_Psuedo("sky",FOV);
		lvlMus.openFromFile("Data/Music/sample.ogg");
		lvlMus.setVolume({ 50 });
		lvlMus.setLooping(true);
		lvlMus.play();
		level_rooms[0].Psuedo_Room(1 ,0);
		level_rooms[1].Psuedo_Room(1, 0);
		ali.position_x = level_rooms[0].px * cell_size;
		ali.position_y = level_rooms[0].py * cell_size;

		zBuffer = new float[ali.ray_no] {-1};
		objS.resize(16);
		/////////////////////////////////////////////////

		objS[0] = new Barrel; objS[0]->placables_psudeo(ali.dir_v, "barrel");
		objS[0]->setCOORD(2*cell_size,6 * cell_size);
		objS[1] = new Barrel; objS[1]->placables_psudeo(ali.dir_v, "barrel");
		objS[1]->setCOORD(2 * cell_size, 17 * cell_size);
		objS[2] = new Barrel; objS[2]->placables_psudeo(ali.dir_v, "barrel");
		objS[2]->setCOORD(9 * cell_size, 20 * cell_size);
		objS[3] = new Barrel; objS[3]->placables_psudeo(ali.dir_v, "barrel");
		objS[3]->setCOORD(15 * cell_size, 8 * cell_size);
		objS[4] = new Barrel; objS[4]->placables_psudeo(ali.dir_v, "barrel");
		objS[4]->setCOORD(28 * cell_size, 17 * cell_size);

		objS[5] = new three_candle; objS[5]->placables_psudeo(ali.dir_v, "three_candle");
		objS[5]->setCOORD(2 * cell_size, 8 * cell_size);
		objS[6] = new three_candle; objS[6]->placables_psudeo(ali.dir_v, "three_candle");
		objS[6]->setCOORD(11 * cell_size, 20 * cell_size);
		objS[7] = new three_candle; objS[7]->placables_psudeo(ali.dir_v, "three_candle");
		objS[7]->setCOORD(15 * cell_size, 13 * cell_size);
		objS[8] = new three_candle; objS[8]->placables_psudeo(ali.dir_v, "three_candle");
		objS[8]->setCOORD(27 * cell_size, 6 * cell_size);
		objS[9] = new three_candle; objS[9]->placables_psudeo(ali.dir_v, "three_candle");
		objS[9]->setCOORD(28 * cell_size, 17 * cell_size);

		objS[10] = new BloodyHeart; objS[10]->placables_psudeo(ali.dir_v, "bloodyHeart");
		objS[10]->setCOORD(4 * cell_size, 26 * cell_size);
		objS[11] = new BloodyHeart; objS[11]->placables_psudeo(ali.dir_v, "bloodyHeart");
		objS[11]->setCOORD(11 * cell_size, 16 * cell_size);
		objS[12] = new BloodyHeart; objS[12]->placables_psudeo(ali.dir_v, "bloodyHeart");
		objS[12]->setCOORD(22 * cell_size, 25 * cell_size);
		objS[13] = new BloodyHeart; objS[13]->placables_psudeo(ali.dir_v, "bloodyHeart");
		objS[13]->setCOORD(27 * cell_size, 28 * cell_size);
		objS[14] = new BloodyHeart; objS[14]->placables_psudeo(ali.dir_v, "bloodyHeart");
		objS[14]->setCOORD(22 * cell_size, 8 * cell_size);

		objS[15] = new dead_guy; objS[15]->placables_psudeo(ali.dir_v, "dead_guy");
		objS[15]->setCOORD(20 * cell_size, 12 * cell_size);

		/////////////////////////////////////////////////
		barT.loadFromFile("Data/Acc/bar.png");
		bar = new Sprite(barT);
		bar->setScale({ 2.5, 2.5 });
		bar->setPosition({ 0, 520 });

		meT.loadFromFile("Data/Acc/me.png");
		me = new Sprite(meT);
		me->setScale({ 0.45, 0.45 });
		me->setPosition({ 400-42, 520.7 });

		mouseTex.loadFromFile("Data/Acc/mouse.png");
		mouseSpr = new Sprite(mouseTex);

		colT.resize(total_wall_textures_per_level);
		colS.resize(total_wall_textures_per_level);

		for(int i=0;i<colT.size();i+=1)
		{
			colT[i].loadFromFile("Data/Walls/"+to_string(i+1)+".png");
			colS[i] = new Sprite(colT[i]);
		}

	}

	void draw_level(RenderWindow& window)
	{
		sky.draw_skyBox(window,ali.angle);
		ali.player_collision(level_rooms,current_room);
		ali.draw_player_vision(window,level_rooms,current_room,colS, zBuffer);

		sort(objS.begin(), objS.end(),
			[](placables_* a, placables_* b) {
				return a->perpDist > b->perpDist;
			});

		for (int i = 0; i < objS.size(); i += 1)
		{
			if (objS[i]->isPresentInFOV(ali.position_x, ali.position_y, ali.angle * PI / 180))
			{
				objS[i]->draw_placable(window, zBuffer,ali.ray_no,ali.position_x,ali.position_y,ali.angle*PI/180);
			}

		}
		ali.player_collision_staticObj(objS);
		ali.move_player();

//		vhs.apply_filter(window);
		window.draw(*bar);
		mouseSpr->setPosition({ (float)Mouse::getPosition().x, (float)Mouse::getPosition().y });
		window.draw(*me);
		window.draw(*mouseSpr);
	}
	~level() 
	{
		lvlMus.stop();
		delete[]bar;
		delete[]me;
		delete[]mouseSpr;
		for (int i = 0; i < colS.size(); i++)
			delete[]colS[i];
		for (int i = 0; i < objS.size(); i++)
			delete[]objS[i];

	}

};

