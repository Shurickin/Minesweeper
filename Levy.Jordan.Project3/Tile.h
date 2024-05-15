#pragma once
#include <SFML/Graphics.hpp>
#include "TextureManager.h"
#include <iostream>
#include <vector>

class Tile
{
	

public:
	sf::Sprite sprite; // texture
	Tile* neighboringTiles[8];
	std::vector<Tile*> neighbors;
	int mineCount;
	bool flag = false;
	bool mine = false;
	bool revealed = false;

	Tile();
	void SetTileTexture(sf::Sprite sprite);
	void SetPosition(int x, int y);
	sf::Sprite& GetSprite();
};

