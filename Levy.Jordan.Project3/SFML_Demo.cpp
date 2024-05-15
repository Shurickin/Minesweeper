#include <SFML/Graphics.hpp>
#include <vector>
#include "TextureManager.h"
#include "Board.h"
#include "Tile.h"
using namespace std;

int main()
{
    sf::RenderWindow window(sf::VideoMode(25*32, (16*32) + 88), "Minesweeper");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    // Load all of my Sprites w/textures
    sf::Sprite mine(TextureManager::GetTexture("mine"));
    sf::Sprite hiddenTile(TextureManager::GetTexture("tile_hidden"));
    sf::Sprite revealedTile(TextureManager::GetTexture("tile_revealed"));
    sf::Sprite one(TextureManager::GetTexture("number_1"));
    sf::Sprite flag(TextureManager::GetTexture("flag"));
    sf::Sprite happyFace(TextureManager::GetTexture("face_happy"));
    sf::Sprite winFace(TextureManager::GetTexture("face_win"));
    sf::Sprite loseFace(TextureManager::GetTexture("face_lose"));
    sf::Sprite digits(TextureManager::GetTexture("digits"));
    sf::Sprite debug(TextureManager::GetTexture("debug"));
    sf::Sprite test1(TextureManager::GetTexture("test_1"));
    sf::Sprite test2(TextureManager::GetTexture("test_2"));
    sf::Sprite test3(TextureManager::GetTexture("test_3"));
    sf::Sprite number1(TextureManager::GetTexture("number_1"));
    sf::Sprite number2(TextureManager::GetTexture("number_2"));
    sf::Sprite number3(TextureManager::GetTexture("number_3"));
    sf::Sprite number4(TextureManager::GetTexture("number_4"));
    sf::Sprite number5(TextureManager::GetTexture("number_5"));
    sf::Sprite number6(TextureManager::GetTexture("number_6"));
    sf::Sprite number7(TextureManager::GetTexture("number_7"));
    sf::Sprite number8(TextureManager::GetTexture("number_8"));
    
    Board game;
    game.ReadConfig("boards/config.cfg");

    for (int i = 0; i < game.board.size(); i++) {
        for (int j = 0; j < game.board[i].size(); j++) {
            game.board[i][j].SetTileTexture(hiddenTile);
            
        }
    }

    // create digit array and set up individual digits
    sf::Sprite digitArray[11];
    for (int i = 0; i < 11; i++) {
        digits.setTextureRect(sf::IntRect(i * 21, 0, 21, 32));
        digitArray[i] = digits;
        
    }

    // create number array
    sf::Sprite numberArray[9];
    numberArray[0] = revealedTile;
    numberArray[1] = number1;
    numberArray[2] = number2;
    numberArray[3] = number3;
    numberArray[4] = number4;
    numberArray[5] = number5;
    numberArray[6] = number6;
    numberArray[7] = number7;
    numberArray[8] = number8;
    

    game.SetMines();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            game.ResetBoard(event, window, happyFace, hiddenTile, flag, mine);
            game.Test1(event, window, test1, hiddenTile, happyFace, flag, mine);
            game.Test2(event, window, test2, hiddenTile, happyFace, flag, mine);
            game.Test3(event, window, test3, hiddenTile, happyFace, flag, mine);

            if (game.Won == false && game.Continue == true) {
                game.Debug(event, window, debug);
            }
            

            if (game.Continue == true) {
                game.FlagTile(event, window, flag);
                game.RevealTile(event, window, revealedTile, mine);
            }
            
        }

        window.clear();

        //Set positions
        happyFace.setPosition(window.getSize().x / 2 - 32, game.row * 32);
        loseFace.setPosition(window.getSize().x / 2 - 32, game.row * 32);
        winFace.setPosition(window.getSize().x / 2 - 32, game.row * 32);
        debug.setPosition(window.getSize().x - 256, game.row * 32);
        test1.setPosition(window.getSize().x - 192, game.row * 32);
        test2.setPosition(window.getSize().x - 128, game.row * 32);
        test3.setPosition(window.getSize().x - 64, game.row * 32);

        
        // Get Placements
        int hundreds = abs((game.mines - game.flags) / 100);
        int tens = abs(((game.mines - game.flags) % 100) / 10);
        int ones = abs(((game.mines - game.flags) % 100) % 10);
        
        // Set and Draw digits
        if (game.mines - game.flags < 0) {
            digitArray[10].setPosition(0, game.row * 32);
            window.draw(digitArray[10]);

            digitArray[hundreds].setPosition(21, game.row * 32);
            window.draw(digitArray[hundreds]);

            digitArray[tens].setPosition(42, game.row * 32);
            window.draw(digitArray[tens]);

            digitArray[ones].setPosition(63, game.row * 32);
            window.draw(digitArray[ones]);
        }
        else {
            digitArray[hundreds].setPosition(21, game.row * 32);
            window.draw(digitArray[hundreds]);

            digitArray[tens].setPosition(42, game.row * 32);
            window.draw(digitArray[tens]);

            digitArray[ones].setPosition(63, game.row * 32);
            window.draw(digitArray[ones]);
        }

        if (game.Continue == true) {
            window.draw(happyFace);
        }
        else if (game.Won == true) {
            window.draw(winFace);
        }
        else {
            window.draw(loseFace);
        }
        
        window.draw(debug);
        window.draw(test1); 
        window.draw(test2);
        window.draw(test3);

        // Make a loop to draw out the board
        game.DrawBoard(window, flag, mine, numberArray, revealedTile);
        
        if (game.debug == true) {
            for (int i = 0; i < game.board.size(); i++) {
                for (int j = 0; j < game.board[i].size(); j++) {
                    if (game.board[i][j].mine == true) {
                        mine.setPosition(game.board[i][j].sprite.getPosition().x, game.board[i][j].sprite.getPosition().y);
                        window.draw(mine);
                    }
                }
            }

        }

        //Check if game has been won
        int winCondition = (game.board.size() * game.board[0].size()) - game.mines;
        //cout << winCondition << endl;
        //cout << game.board.size() * game.board[0].size() << endl;
        for (int i = 0; i < game.board.size(); i++) {
            for (int j = 0; j < game.board[i].size(); j++) {
                if (game.board[i][j].revealed == true) {
                    game.revealedTiles++;
                } 
            }
        }
        
        if (game.Won == false) {
            if (game.revealedTiles == winCondition) {
                game.Won = true;
                game.Continue = false;
            }
            else {
                game.revealedTiles = 0;
            }
        }

        if (game.Won == true && game.Continue == false) {
            for (int i = 0; i < game.board.size(); i++) {
                for (int j = 0; j < game.board[i].size(); j++) {
                    if (game.board[i][j].mine == true && game.board[i][j].flag == false){
                        flag.setPosition(game.board[i][j].sprite.getPosition().x, game.board[i][j].sprite.getPosition().y);
                        window.draw(flag);
                        
                    }
                }
            }
            game.flags = game.mines;
        }
        

        window.display();
    }

    TextureManager::Clear();
    return 0;
}
