#include "Board.h"

Board::Board() {
	row = 0;
	col = 0;
	mines = 0;
	flags = 0;
	revealedTiles = 0;
}

void Board::ReadConfig(string filePath) {
	ifstream inFile(filePath);
	if (inFile.is_open()) {

			//Get columns
			string tempCol;
			getline(inFile, tempCol);
			col = stoi(tempCol);
			
			// Get rows
			string tempRow;
			getline(inFile, tempRow);
			row = stoi(tempRow);

			// Get mines
			string tempMine;
			getline(inFile, tempMine);
			mines = stoi(tempMine);

			// Make the board
			board.resize(row);
			for (int i = 0; i < board.size(); i++) {
				board[i].resize(col);
			}
	}
	inFile.close();
}

void Board::ReadTest(string filePath, sf::Sprite hiddenTile) {
	mineBoard.clear();
	mines = 0;
	mineBoard.resize(board.size());
	for (int i = 0; i < board.size(); i++) {
		mineBoard[i].resize(board[i].size());
	}

	ifstream inFile(filePath);
	if (inFile.is_open()) {
		
		string lineFromFile;
		int counter = 0;

		while (getline(inFile, lineFromFile)) {

			//Get values
			for (int i = 0; i < board[counter].size(); i++) {
				mineBoard[counter][i] = stoi(std::string(1,lineFromFile[i]));
			}
			counter++;
		}
	}
	
	inFile.close();

	for (int i = 0; i < mineBoard.size(); i++) {
		for (int j = 0; j < mineBoard[i].size(); j++) {

			board[i][j].SetTileTexture(hiddenTile);

			if (mineBoard[i][j] == 1) {
				board[i][j].mine = true;
				mines++;
			}
			else {
				board[i][j].mine = false;
			}
		}
	}


}

void Board::FlagTile(sf::Event event, sf::RenderWindow& window, sf::Sprite flag) {
	if (event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == sf::Mouse::Right) {
			sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

			if (mousePosition.y / 32 < board.size() && mousePosition.x / 32 < board[0].size()) {
				board[mousePosition.y / 32][mousePosition.x / 32].flag = !board[mousePosition.y / 32][mousePosition.x / 32].flag;
				if (board[mousePosition.y / 32][mousePosition.x / 32].flag == true) {
					flags++;
				}
				else {
					flags--;
				}
				
			}
		}
	}
}

void Board::RevealTile(sf::Event event, sf::RenderWindow& window, sf::Sprite revealedTile, sf::Sprite mine) {
	if (event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

			if (mousePosition.y / 32 < board.size() && mousePosition.x / 32 < board[0].size() && board[mousePosition.y / 32][mousePosition.x / 32].flag == false) {

				board[mousePosition.y / 32][mousePosition.x / 32].SetTileTexture(revealedTile);
				board[mousePosition.y / 32][mousePosition.x / 32].revealed = true;

				int mineCount = 0;
				for (int i = 0; i < 8; i++) {
					if (board[mousePosition.y / 32][mousePosition.x / 32].neighboringTiles[i] != nullptr) {
						if (board[mousePosition.y / 32][mousePosition.x / 32].neighboringTiles[i]->mine == true) {
							mineCount++;
						}
					}
				}

				board[mousePosition.y / 32][mousePosition.x / 32].mineCount = mineCount;

				if (board[mousePosition.y / 32][mousePosition.x / 32].mineCount == 0) {
					RecurseReveal(board[mousePosition.y / 32][mousePosition.x / 32].neighboringTiles, revealedTile);
				}

				// If the tile holds a mine
				if (board[mousePosition.y / 32][mousePosition.x / 32].mine == true) {
					Continue = false; // Game has been lost

					// loop through the board, if mine, reveal it
					for (int i = 0; i < board.size(); i++) {
						for (int j = 0; j < board[i].size(); j++) {
							if (board[i][j].mine == true && board[i][j].revealed == false) {
								board[i][j].revealed = true;
								board[i][j].SetTileTexture(revealedTile);
								mine.setPosition(board[i][j].sprite.getPosition().x, board[i][j].sprite.getPosition().y);
								window.draw(mine);
							}
						}
					}
				}
			}
		}
	}
}

void Board::RecurseReveal(Tile* (&neighbors)[8], sf::Sprite revealedTile) {
	// I want to check the mine count for all the neighbors, if it is 0, run recurse on that tile
	int mineCount = 0;
	for (int j = 0; j < 8; j++) { // j is the index of the neighbors array
		// Reveal original neighbors
		if (neighbors[j] != nullptr && neighbors[j]->revealed == false && neighbors[j]->flag == false) {

			neighbors[j]->revealed = true;
			neighbors[j]->SetTileTexture(revealedTile);
			revealedTiles++;

			for (int i = 0; i < 8; i++) { // i is the index of the neighbors' neighbors
				if (neighbors[j]->neighboringTiles[i] != nullptr) {
					if (neighbors[j]->neighboringTiles[i]->mine == true) {
						mineCount++;
					}
				}
				
			}
			neighbors[j]->mineCount = mineCount;
			mineCount = 0;
			if (neighbors[j]->mineCount == 0) {
				/*for (int i = 0; i < 8; i++) {
					neighbors[j]->neighboringTiles[i]->revealed == true;
				}*/
				RecurseReveal(neighbors[j]->neighboringTiles, revealedTile);
			}
		}
		
	}
	
}

void Board::Debug(sf::Event event, sf::RenderWindow& window, sf::Sprite debug) {
	if (event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			sf::Vector2f mousePosition(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);

			if (debug.getGlobalBounds().contains(mousePosition)) {
				this->debug = !this->debug;
			}
		}
	}
}

void Board::SetMines() {
	mineBoard.resize(board.size());
	for (int i = 0; i < board.size(); i++) {
		mineBoard[i].resize(board[i].size());
	}

	int mineCount = 0;
	for (int i = 0; i < mineBoard.size(); i++) {
		for (int j = 0; j < mineBoard[i].size(); j++) {
			mineBoard[Random::Int(0,board.size() - 1)][Random::Int(0, board[i].size() - 1)] = Random::Int(0, 1);
			
			if (mineCount < mines) {
				if (mineBoard[i][j] == 1) {
					board[i][j].mine = true;
					mineCount++;
				}
			}
			
		}
	}
}

void Board::DrawBoard(sf::RenderWindow& window, sf::Sprite flag, sf::Sprite mine, sf::Sprite numberArray[], sf::Sprite revealedTile) {
	for (int i = 0; i < board.size(); i++) {
		for (int j = 0; j < board[i].size(); j++) {

			board[i][j].SetPosition(j * 32, i * 32);

			// Sets up neighbors
			if (i == 0 && j == 0) {
				board[i][j].neighboringTiles[0] = nullptr;
				board[i][j].neighboringTiles[1] = nullptr;
				board[i][j].neighboringTiles[2] = nullptr;
				board[i][j].neighboringTiles[3] = nullptr;
				board[i][j].neighboringTiles[4] = &board[i][j + 1];
				board[i][j].neighboringTiles[5] = nullptr;
				board[i][j].neighboringTiles[6] = &board[i + 1][j];
				board[i][j].neighboringTiles[7] = &board[i + 1][j + 1];
			}
			else if (i == board.size() - 1 && j == board[i].size() - 1) {
				board[i][j].neighboringTiles[0] = &board[i - 1][j - 1];
				board[i][j].neighboringTiles[1] = &board[i - 1][j];
				board[i][j].neighboringTiles[2] = nullptr;
				board[i][j].neighboringTiles[3] = &board[i][j - 1];
				board[i][j].neighboringTiles[4] = nullptr;
				board[i][j].neighboringTiles[5] = nullptr;
				board[i][j].neighboringTiles[6] = nullptr;
				board[i][j].neighboringTiles[7] = nullptr;
			}
			else if (i == 0 && j == board[i].size() - 1) {
				board[i][j].neighboringTiles[0] = nullptr;
				board[i][j].neighboringTiles[1] = nullptr;
				board[i][j].neighboringTiles[2] = nullptr;
				board[i][j].neighboringTiles[3] = &board[i][j - 1];
				board[i][j].neighboringTiles[4] = nullptr;
				board[i][j].neighboringTiles[5] = &board[i + 1][j - 1];
				board[i][j].neighboringTiles[6] = &board[i + 1][j];
				board[i][j].neighboringTiles[7] = nullptr;
			}
			else if (i == board.size() - 1 && j == 0) {
				board[i][j].neighboringTiles[0] = nullptr;
				board[i][j].neighboringTiles[1] = &board[i - 1][j];
				board[i][j].neighboringTiles[2] = &board[i - 1][j + 1];
				board[i][j].neighboringTiles[3] = nullptr;
				board[i][j].neighboringTiles[4] = &board[i][j + 1];
				board[i][j].neighboringTiles[5] = nullptr;
				board[i][j].neighboringTiles[6] = nullptr;
				board[i][j].neighboringTiles[7] = nullptr;
			}
			else if (i == 0) {
				board[i][j].neighboringTiles[0] = nullptr;
				board[i][j].neighboringTiles[1] = nullptr;
				board[i][j].neighboringTiles[2] = nullptr;
				board[i][j].neighboringTiles[3] = &board[i][j - 1];
				board[i][j].neighboringTiles[4] = &board[i][j + 1];
				board[i][j].neighboringTiles[5] = &board[i + 1][j - 1];
				board[i][j].neighboringTiles[6] = &board[i + 1][j];
				board[i][j].neighboringTiles[7] = &board[i + 1][j + 1];
			}
			else if (j == 0) {
				board[i][j].neighboringTiles[0] = nullptr;
				board[i][j].neighboringTiles[1] = &board[i - 1][j];
				board[i][j].neighboringTiles[2] = &board[i - 1][j + 1];
				board[i][j].neighboringTiles[3] = nullptr;
				board[i][j].neighboringTiles[4] = &board[i][j + 1];
				board[i][j].neighboringTiles[5] = nullptr;
				board[i][j].neighboringTiles[6] = &board[i + 1][j];
				board[i][j].neighboringTiles[7] = &board[i + 1][j + 1];
			}
			else if (i == board.size() - 1) {
				board[i][j].neighboringTiles[0] = &board[i - 1][j - 1];
				board[i][j].neighboringTiles[1] = &board[i - 1][j];
				board[i][j].neighboringTiles[2] = &board[i - 1][j + 1];
				board[i][j].neighboringTiles[3] = &board[i][j - 1];
				board[i][j].neighboringTiles[4] = &board[i][j + 1];
				board[i][j].neighboringTiles[5] = nullptr;
				board[i][j].neighboringTiles[6] = nullptr;
				board[i][j].neighboringTiles[7] = nullptr;
			}
			else if (j == board[i].size() - 1) {
				board[i][j].neighboringTiles[0] = &board[i - 1][j - 1];
				board[i][j].neighboringTiles[1] = &board[i - 1][j];
				board[i][j].neighboringTiles[2] = nullptr;
				board[i][j].neighboringTiles[3] = &board[i][j - 1];
				board[i][j].neighboringTiles[4] = nullptr;
				board[i][j].neighboringTiles[5] = &board[i + 1][j - 1];
				board[i][j].neighboringTiles[6] = &board[i + 1][j];
				board[i][j].neighboringTiles[7] = nullptr;
			}
			else {
				board[i][j].neighboringTiles[0] = &board[i - 1][j - 1];
				board[i][j].neighboringTiles[1] = &board[i - 1][j];
				board[i][j].neighboringTiles[2] = &board[i - 1][j + 1];
				board[i][j].neighboringTiles[3] = &board[i][j - 1];
				board[i][j].neighboringTiles[4] = &board[i][j + 1];
				board[i][j].neighboringTiles[5] = &board[i + 1][j - 1];
				board[i][j].neighboringTiles[6] = &board[i + 1][j];
				board[i][j].neighboringTiles[7] = &board[i + 1][j + 1];
			}
			

			window.draw(board[i][j].sprite);

			if (board[i][j].flag == true) {
				flag.setPosition(board[i][j].sprite.getPosition().x, board[i][j].sprite.getPosition().y);
				window.draw(flag);
			}
			if (board[i][j].mine == true && board[i][j].revealed == true) {
				mine.setPosition(board[i][j].sprite.getPosition().x, board[i][j].sprite.getPosition().y);
				window.draw(mine);
			}
			if (board[i][j].revealed == true && board[i][j].mine == false) {
				//board[i][j].SetTileTexture(revealedTile);
				numberArray[board[i][j].mineCount].setPosition(board[i][j].sprite.getPosition().x, board[i][j].sprite.getPosition().y);
				window.draw(numberArray[board[i][j].mineCount]);
			}
		}
	}
}

void Board::ResetBoard(sf::Event event, sf::RenderWindow& window, sf::Sprite happyFace, sf::Sprite hiddenTile, sf::Sprite flag, sf::Sprite mine) {
	if (event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			sf::Vector2f mousePosition(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);

			if (happyFace.getGlobalBounds().contains(mousePosition)){
				Board game;
				game.ReadConfig("boards/config.cfg");

				for (int i = 0; i < game.board.size(); i++) {
					for (int j = 0; j < game.board[i].size(); j++) {
						game.board[i][j].SetTileTexture(hiddenTile);
					}
				}

				game.SetMines();

				*this = game;
			}
		}
	}
}

Board& Board::operator=(const Board& rhs) {
	row = rhs.row;
	col = rhs.col;
	mines = rhs.mines;
	flags = rhs.flags;
	revealedTiles = rhs.revealedTiles;
	Won = rhs.Won;
	Continue = rhs.Continue;

	for (int i = 0; i < rhs.board.size(); i++) {
		for (int j = 0; j < rhs.board[i].size(); j++) {
			board[i][j] = rhs.board[i][j];
			board[i][j].mine = rhs.board[i][j].mine;
		}
	}
	return *this;
}

void Board::Test1(sf::Event event, sf::RenderWindow& window, sf::Sprite test1, sf::Sprite hiddenTile, sf::Sprite happyFace, sf::Sprite flag, sf::Sprite mine) {
	if (event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			sf::Vector2f mousePosition(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);

			if (test1.getGlobalBounds().contains(mousePosition)) {
				ResetBoard(event, window, test1, hiddenTile, flag, mine);
				ReadTest("boards/testBoard1.brd", hiddenTile);
			}
		}
	}
}

void Board::Test2(sf::Event event, sf::RenderWindow& window, sf::Sprite test2, sf::Sprite hiddenTile, sf::Sprite happyFace, sf::Sprite flag, sf::Sprite mine) {
	if (event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			sf::Vector2f mousePosition(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);

			if (test2.getGlobalBounds().contains(mousePosition)) {
				ResetBoard(event, window, test2, hiddenTile, flag, mine);
				ReadTest("boards/testBoard2.brd", hiddenTile);
			}
		}
	}
}

void Board::Test3(sf::Event event, sf::RenderWindow& window, sf::Sprite test3, sf::Sprite hiddenTile, sf::Sprite happyFace, sf::Sprite flag, sf::Sprite mine) {
	if (event.type == sf::Event::MouseButtonPressed) {
		if (event.mouseButton.button == sf::Mouse::Left) {
			sf::Vector2f mousePosition(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);

			if (test3.getGlobalBounds().contains(mousePosition)) {
				ResetBoard(event, window, test3, hiddenTile, flag, mine);
				ReadTest("boards/testBoard3.brd", hiddenTile);
			}
		}
	}
}
