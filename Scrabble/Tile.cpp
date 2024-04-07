#include "Tile.h"
using namespace std;

Tile::Tile()
{
	letter = '0';
	blank = false;
	wordBonus = 1;
	letterBonus = 1;
}

Tile::Tile(unsigned char letterBonus_, unsigned char wordBonus_)
{
	letter = '0';
	blank = false;
	wordBonus = wordBonus_;
	letterBonus = letterBonus_;
}

Tile::Tile(const Tile& other)
{
	this->letter = other.letter;
	this->wordBonus = other.wordBonus;
	this->letterBonus = other.letterBonus;
}


Tile& Tile::operator=(const Tile& other)
{
	if (&other != this)
	{
		this->letter = other.letter;
		this->wordBonus = other.wordBonus;
		this->letterBonus = other.letterBonus;
	}
	return *this;
}

char Tile::getLetter()
{
	return letter;
}

bool Tile::isBlank()
{
	return blank;
}

unsigned char Tile::getLetterBonus()
{
	return letterBonus;
}

unsigned char Tile::getWordBonus()
{
	return wordBonus;
}

void Tile::setLetter(char L)
{
	this->letter = L;
}

void Tile::setBlank(bool B)
{
	this->blank = B;
}

