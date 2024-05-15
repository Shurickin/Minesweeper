#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
using std::string;
using std::unordered_map;

class TextureManager
{
	static unordered_map<string, sf::Texture> textures;

public:
	static void LoadTexture(string textureName); // LoadTexture("space")
	static sf::Texture& GetTexture(string textureName);
	static void Clear(); // Call this once at the end of the program
};

