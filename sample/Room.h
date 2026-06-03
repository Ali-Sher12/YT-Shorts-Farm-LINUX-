#pragma once

using namespace sf;
using namespace std;

/*

	# is for walls
	@ is for grime walls
	* is for blood walls
	a is for toilet walls
	b is for toilet grime walls
	d is for doors
	e is for elevator
	f is for frames

	//adding transition for doors

*/


class MusicClass
{
public:
	Music bg;

	float x = 1920;
	float y = 1080;

	MusicClass()
	{
		bg.openFromFile("Data/Sound/test.ogg");
	}

};


class Room
{

public:

	char** level_grid;
	int rows = 0;
	int columns = 0;
	int level_id = 0;
	int room_no = 0;
	int px, py;
	void Psuedo_Room(int level_id_, int room_no_)
	{
		level_id = level_id_;
		room_no = room_no_;

		ifstream file1("Data/level_data/level" + to_string(level_id) + "/room"+to_string(room_no)+"_met.txt");
		file1>>rows;
		file1>>columns;
		file1.close();

		level_grid = new char* [rows];
		for (int i = 0; i < rows; i += 1)
		{
			level_grid[i] = new char[columns];
		}

		char ch;
		ifstream file2("Data/level_data/level" + to_string(level_id) + "/room" + to_string(room_no) + "_dat.txt");
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < columns; j++)
			{
				file2.get(ch);
				if(ch == '\n')
					file2.get(ch);
				else if (ch == 'P')
				{
					px = i; py = j; ch = ' ';
				}
				level_grid[i][j] = ch;
			}
		}
		file2.close();
	
	}

	~Room()
	{
		for (int i = 0; i < rows; ++i)
		{
			delete[] level_grid[i];
		}
		delete[] level_grid;
	}

};