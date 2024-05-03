#include "Board.h"
#include <iostream>
using namespace std;

Board::Board()
{ //tried converting to a function rather than repeating code, but the need for a custom array of varying sizes makes it too much effort to bother implementing
	//when array is initialised it will use the default constructor, effectively Tile tile(1,1)
	{ //sets tiles that award triple word points
		const int tileNum = 8;
		Tile tripleWordTile(1, 3);
		unsigned char Tiles[tileNum] = { 0, 7, 14, 105, 119, 210, 217, 224 };
		for (size_t i = 0; i < tileNum; i++)
		{
			tiles[Tiles[i]] = tripleWordTile;
		}
	}
	{ //sets tiles that award triple the points of the letter placed on them.
		const int tileNum = 12;
		Tile tripleLetterTile(3, 1);
		unsigned char Tiles[tileNum] = { 20, 24, 76, 80, 84, 88, 136, 140, 144, 148, 200, 204 };
		for (size_t i = 0; i < tileNum; i++)
		{
			tiles[Tiles[i]] = tripleLetterTile;
		}
	}
	{ //tiles that award double word
		const int tileNum = 17;
		Tile doubleWordTile(1, 2);
		unsigned char Tiles[tileNum] = { 16, 28, 32, 42, 48, 56, 64, 70, 112, 154, 160, 168, 176, 182, 192, 196, 208 };
		for (size_t i = 0; i < tileNum; i++)
		{
			tiles[Tiles[i]] = doubleWordTile;
		}
	}
	{//tiles that award double letter
		const int tileNum = 24;
		Tile doubleLetterTile(2, 1);
		unsigned char Tiles[tileNum] = { 3, 11, 36, 38, 45, 52, 59, 92, 96, 98, 102, 108, 116, 122, 126, 128, 132, 165, 172, 179, 186, 188, 213, 221 };
		for (size_t i = 0; i < tileNum; i++)
		{
			tiles[Tiles[i]] = doubleLetterTile;
		}
	}
	for (size_t i = 0; i < 225; i++)
	{
		for (size_t j = 0; j < 27; j++)
		{
			crossSet[i][j] = false;
		}
	}
	anchors.insert(112);
}

Board::Board(const Board& other)
{
	for (size_t i = 0; i < 225; i++)
	{
		this->tiles[i] = other.tiles[i];
	}
}

Board& Board::operator=(const Board& other)
{
	if (&other != this)
	{
		for (size_t i = 0; i < 225; i++)
		{
			this->tiles[i] = other.tiles[i];
		}
	}

	return *this;
}

Board::~Board()
{
	//delete tiles;
}

pair<unsigned char, unsigned char> Board::getCoords(unsigned char index)
{
	unsigned char x = index % 15;
	unsigned char y = index / 15;
	//e.g the tile with index 4 is at (4,0). The tile with index 121 is at (7,7).
	return pair<unsigned char, unsigned char>(x, y);
}

unsigned char Board::getIndex(unsigned char x, unsigned char y)
{
	return (y * 15 + x);
}

Tile* Board::getTile(unsigned char index)
{
	return &tiles[index];
}

Tile* Board::getTile(unsigned char x, unsigned char y)
{
	return &tiles[getIndex(x,y)];
}

char Board::getLetter(unsigned char index)
{
	if (index > 225 || index < 0)
	{
		//cout << "ERROR: TILE INDEX OUTSIDE BOUNDS OF ARRAY AT: " << int(index) << endl;
	}
	return tiles[index].getLetter();
}

char Board::getLetter(unsigned char x, unsigned char y)
{
	return tiles[getIndex(x,y)].getLetter();
}

unsigned char Board::getLetterBonus(unsigned char index)
{
	return tiles[index].getLetterBonus();
}

unsigned char Board::getLetterBonus(unsigned char x, unsigned char y)
{
	return tiles[getIndex(x, y)].getLetterBonus();
}

unsigned char Board::getWordBonus(unsigned char index)
{
	return tiles[index].getWordBonus();
}

unsigned char Board::getWordBonus(unsigned char x, unsigned char y)
{
	return tiles[getIndex(x, y)].getWordBonus();
}

void Board::PlaceTile(unsigned char index, char letter, bool blank)
{
	this->getTile(index)->setLetter(letter);
	this->getTile(index)->setBlank(blank);
}

void Board::PlaceTile(unsigned char x, unsigned char y, char letter, bool blank)
{
	PlaceTile(getIndex(x, y), letter, blank);
}

void Board::RemoveTile(unsigned char index)
{
	this->getTile(index)->setLetter('0');
	this->getTile(index)->setBlank(false);
}

void Board::RemoveTile(unsigned char x, unsigned char y)
{
	RemoveTile(getIndex(x, y));
}

void Board::ReplaceTile(unsigned char index, char* rack)
{
	for (size_t i = 0; i < 7; i++)
	{	
		if (rack[i] == '0')
		{
			if (getBlank(index))
			{
				rack[i] = '[';
			}
			else
			{
				rack[i] = this->getLetter(index);
			}
			break;
		}
	}
	this->getTile(index)->setLetter('0');
	this->getTile(index)->setBlank(false);
}

void Board::ReplaceTile(unsigned char x, unsigned char y, char* rack)
{
	ReplaceTile(getIndex(x, y), rack);
}

void Board::Clear()
{
	for (size_t i = 0; i < 225; i++)
	{
		tiles[i].setLetter('0');
		tiles[i].setBlank(false);
	}
}

bool Board::getBlank(int index)
{
	return tiles[index].isBlank();
}

set<int> Board::getAnchors()
{
	return anchors;
}

bool* Board::getValidLetters(int index)
{
	return crossSet[index];
}

void Board::getWord(int direction, int x, int y, Node* root)
{
	int start, end;
	string word = "";
	int targetX = x, targetY = y;
	while (getLetter(targetX, targetY) != '0') //gets all letters to the left and top of the target tile.
	{
		word = getLetter(targetX, targetY) + word;
		targetX = targetX - 1 + direction;
		targetY = targetY - direction;
	}
	start = getIndex(targetX, targetY);
	targetX = x - direction + 1;
	targetY = y + direction;
	while (getLetter(targetX, targetY) != '0') //gets all letters to the right and below the target tile.
	{
		word = word + getLetter(targetX, targetY);
		targetX = targetX + 1 - direction;
		targetY = targetY + direction;
	}
	end = getIndex(targetX, targetY);
	reverse(word.begin(), word.end());
	vector<char> startCrossSet = root->getCrossSetChildren(word);
	for (size_t i = 0; i < startCrossSet.size(); i++)
	{
		crossSet[start][startCrossSet[i]] = true;
	}
	vector<char> endCrossSet = root->getCrossSetChildren(word + '+');
	for (size_t i = 0; i < endCrossSet.size(); i++)
	{
		crossSet[end][startCrossSet[i]] = true;
	}
	crossSet[start][26] = true;
	crossSet[end][26] = true;
	
}



void Board::UpdateCrossSets(Node* root)
{
	for (size_t i = 0; i < 15; i++)
	{
		for (size_t j = 0; j < 15; j++)
		{
			if (getLetter(i, j) != '0')
			{
				getWord(0, i, j, root);
				getWord(1, i, j, root);
			}
		}
	}
}

bool Board::getCrossSet(int index, char L)
{
	return crossSet[index][L - 'A'];
}

bool Board::hasAdjacent(int x, int y)
{
	if (getLetter(x+1,y) != '0' || getLetter(x - 1, y) != '0' || getLetter(x, y + 1) != '0' || getLetter(x, y - 1) != '0')
	{
		return true;
	}
	return false;
}

void Board::UpdateAnchors()
{
	anchors.clear();
	anchors.insert(112);
	for (size_t i = 1; i < 14; i++)
	{
		for (size_t j = 1; j < 14; j++)
		{
			if (getLetter(i, j) != '0')
			{
				anchors.insert(getIndex(i, j));
				if (i < 14)
				{
					anchors.insert(getIndex(i + 1, j));
				}
				if (i > 0)
				{
					anchors.insert(getIndex(i - 1, j));
				}
				if (j < 14)
				{
					anchors.insert(getIndex(i, j + 1));
				}
				if (j > 0)
				{
					anchors.insert(getIndex(i, j - 1));
				}
			}
		}
	}
	
}

/*void Board::BonusTiles(unsigned char* arr, int length, unsigned char letterBonus, unsigned char wordBonus)
{
	Tile tile(letterBonus, wordBonus);
	for (size_t i = 0; i < length; i++)
	{
		tiles[arr[i]] = tile;
	}
}*/