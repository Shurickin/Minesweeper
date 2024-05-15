#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "Tile.h"
#include "Random.h"
using namespace std;

class Board
{
    
    
public:
    vector<vector<Tile>> board;
    vector<vector<int>> mineBoard;
    int row = 0, col = 0, mines = 0, flags = 0, revealedTiles = 0;
    bool Won = false;
    bool Continue = true;
    bool debug = false;

    Board();
   void ReadConfig(string filePath);
   void ReadTest(string filePath, sf::Sprite hiddenTile);
   void SetMines();
   void Debug(sf::Event event, sf::RenderWindow& window, sf::Sprite debug);
   void DrawBoard(sf::RenderWindow& window, sf::Sprite flag, sf::Sprite mine, sf::Sprite numberArray[], sf::Sprite revealedTile);
   void FlagTile(sf::Event event, sf::RenderWindow& window, sf::Sprite flag);
   void RevealTile(sf::Event event, sf::RenderWindow& window, sf::Sprite revealedTile, sf::Sprite mine);
   void RecurseReveal(Tile* (&neighbors)[8], sf::Sprite revealedTile);
   void ResetBoard(sf::Event event, sf::RenderWindow& window, sf::Sprite happyFace, sf::Sprite hiddenTile, sf::Sprite flag, sf::Sprite mine);
   Board& operator=(const Board& rhs);
   void Test1(sf::Event event, sf::RenderWindow& window, sf::Sprite test1, sf::Sprite hiddenTile, sf::Sprite happyFace, sf::Sprite flag, sf::Sprite mine);
   void Test2(sf::Event event, sf::RenderWindow& window, sf::Sprite test2, sf::Sprite hiddenTile, sf::Sprite happyFace, sf::Sprite flag, sf::Sprite mine);
   void Test3(sf::Event event, sf::RenderWindow& window, sf::Sprite test3, sf::Sprite hiddenTile, sf::Sprite happyFace, sf::Sprite flag, sf::Sprite mine);
};

