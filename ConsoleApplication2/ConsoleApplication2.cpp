// ConsoleApplication2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <math.h>
#include<stdio.h>
#include<iostream>

const int height = 5; const int width = 8;
const int screen_w = 1200; const int screen_h = 640;
int map[height + 2][width + 2] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 1, 0, 0, 0, 0, 1, 0 },
	{ 0, 0, 0, 1, 1, 0, 0, 0, 1, 0 },
	{ 0, 0, 1, 1, 0, 1, 1, 1, 1, 0 },
	{ 0, 0, 0, 0, 1, 1, 1, 0, 1, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
int dogtotile(int dog){
	switch (dog){
	case 0:return 16;
	case 1:return 13;
	case 2:return 11;
	case 3:return 12;
	case 4:return 3;
	case 5:return 8;
	case 6:return 15;
	case 7:return 4;
	case 8:return 1;
	case 9:return 14;
	case 10:return 6;
	case 11:return 5;
	case 12:return 2;
	case 13:return 9;
	case 14:return 10;
	case 15:return 7;
	default:throw;
	}
}
float zoom = 1;
int scroll = 0;
sf::VertexArray tilemap;
void update_tiles(){
	int dog = 0;
	for (int i = 0; i < height + 1; i++){
		for (int j = 0; j < width + 1; j++){
			dog = map[i][j] + 2 * map[i][j + 1] + 4 * map[i + 1][j] + 8 * map[i + 1][j + 1];
			
				dog = dogtotile(dog) - 1;
				sf::Vertex* quad = &tilemap[(i*(width + 1) + j) * 4];
				// define its 4 corners
				quad[0].position = sf::Vector2f(j * 128, i * 128);
				quad[1].position = sf::Vector2f((j + 1) * 128, i * 128);
				quad[2].position = sf::Vector2f((j + 1) * 128, (i + 1) * 128);
				quad[3].position = sf::Vector2f(j * 128, (i + 1) * 128);

				// define its 4 texture coordinates
				quad[0].texCoords = sf::Vector2f((dog % 5) * 128, (int)(dog / 5) * 128);
				quad[1].texCoords = sf::Vector2f((dog % 5 + 1) * 128, (int)(dog / 5) * 128);
				quad[2].texCoords = sf::Vector2f((dog % 5 + 1) * 128, (int)(dog / 5 + 1) * 128);
				quad[3].texCoords = sf::Vector2f((dog % 5) * 128, (int)(dog / 5 + 1) * 128);
			
		}
	}
};
sf::Vector2i operator*(sf::Vector2i v1, float n){
	return sf::Vector2i(v1.x*n, v1.y*n);
}
sf::Vector2i operator/(sf::Vector2i v1, float n){
	return sf::Vector2i(v1.x / n, v1.y / n);
}
sf::Vector2i hold(-1, -1);
sf::Vector2i fixed(0, 0);
sf::Vector2i offset(screen_w/2, screen_h/2);
const sf::Vector2i center = offset;
sf::Vector2i of1(0, 0);
bool is_fixed = false;

int main()
{
	sf::RenderWindow window(sf::VideoMode(screen_w,screen_h), "SFML works!");
	sf::View view(sf::FloatRect(sf::Vector2f(screen_w/2,screen_h/2),sf::Vector2f(screen_w,screen_h)));
	sf::Texture dogs;
	dogs.loadFromFile("dog-tileset-angles.png");
	tilemap.setPrimitiveType(sf::Quads);
	tilemap.resize((height + 1) * (width + 1) * 4);
	sf::RenderStates states;
	states.texture = &dogs;
	update_tiles();
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
				sf::Vector2i pos = sf::Mouse::getPosition(window) + offset / zoom - center;
				//std::cout << pos.y << ' ' << pos.x << std::endl;
				pos.x = round(pos.x * zoom / 128);
				pos.y = round(pos.y * zoom / 128);
				//std::cout << pos.y << ' ' << pos.x << std::endl;
				if (pos != hold){
					hold = pos;
					if ((pos.y >= 0) && (pos.y <= height + 1) && (pos.x >= 0) && (pos.x <= width + 1)){
						map[pos.y][pos.x] = (map[pos.y][pos.x] + 1) % 2;
						update_tiles();
					}
				}
			}
			if ((!sf::Mouse::isButtonPressed(sf::Mouse::Left))&&(hold.x>=0)){
				hold=sf::Vector2i(-1,-1);
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Right)){
				if (!is_fixed){
					fixed = sf::Mouse::getPosition(window)*zoom;
					is_fixed = true;
				}
				of1 = offset + fixed - sf::Mouse::getPosition(window)*zoom;
				view.setCenter(of1.x,of1.y);
				window.setView(view);
			}
			if (!(sf::Mouse::isButtonPressed(sf::Mouse::Right)) && (is_fixed)){
				is_fixed = false;
				offset = of1;
				fixed = sf::Vector2i(0, 0);
				of1 = fixed;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract)){
				zoom *= 2;
				view.zoom(2);
				offset = offset / 2 + center / 2;
				//of1 = sf::Mouse::getPosition(window) - sf::Vector2i(screen_w / 2, screen_h / 2);
				//offset -= sf::Vector2i(of1.x*zoom/2, of1.y*zoom/2);
				view.setCenter(offset.x, offset.y);
				window.setView(view);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add)){
				zoom *= 0.5;
				view.zoom(0.5);
				offset = offset * 2 - center;
				//of1 = sf::Mouse::getPosition(window) - sf::Vector2i(screen_w / 2, screen_h / 2);
				//offset += sf::Vector2i(of1.x *zoom, of1.y *zoom);
				view.setCenter(offset.x, offset.y);
				window.setView(view);
			}
		}
		/*if (event.type == sf::Event::MouseWheelScrolled){
			float delta = event.mouseWheelScroll.delta;
			zoom += delta;
			if (zoom < 128){
				zoom = 128;
			}
			else{
				view.zoom(zoom / zoom - delta);
			}
			//zoom = zoom*delta;
			//view.zoom(delta);
		}*/
		window.clear();
		window.draw(tilemap,states);
		window.display();
	}

	return 0;
}
