compile:
	g++ main.cpp -o app -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network

run:
	./app

all:
	g++ main.cpp -o app -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network && ./app

