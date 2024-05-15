#include "Tile.h"

Tile::Tile() {

}

void Tile::SetTileTexture(sf::Sprite sprite) {
	this->sprite = sprite;
}

void Tile::SetPosition(int x, int y) {
	sprite.setPosition(x,y);
}

sf::Sprite& Tile::GetSprite() {
	return sprite;
}