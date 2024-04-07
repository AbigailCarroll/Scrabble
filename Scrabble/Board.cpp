#include "Board.h"
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
			rack[i] = this->getLetter(index);
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

/*void Board::BonusTiles(unsigned char* arr, int length, unsigned char letterBonus, unsigned char wordBonus)
{
	Tile tile(letterBonus, wordBonus);
	for (size_t i = 0; i < length; i++)
	{
		tiles[arr[i]] = tile;
	}
}*/