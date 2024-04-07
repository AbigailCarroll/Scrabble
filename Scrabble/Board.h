#include "Tile.h"
#include <utility>;
using namespace std;

//class stores the tiles in a 1d array as an index between 0 and 224 as it is much easier to work with than a 2d array
//different parameters for functions ensure coordinates can still be used effectively, y begins at 0 in the top left and increases to 15 as you move down the screen.
//rather than beggining at the bottom left

class Board
{
public:
	Board();

	~Board();

	Tile* getTile(unsigned char index);

	Tile* getTile(unsigned char x, unsigned char y);

	Board(const Board& other);

	Board& operator=(const Board& other);

	pair<unsigned char, unsigned char> getCoords(unsigned char index);

	unsigned char getIndex(unsigned char x, unsigned char y);

	char getLetter(unsigned char index);

	char getLetter(unsigned char x, unsigned char y);

	unsigned char getLetterBonus(unsigned char index);

	unsigned char getLetterBonus(unsigned char x, unsigned char y);

	unsigned char getWordBonus(unsigned char index);

	unsigned char getWordBonus(unsigned char x, unsigned char y);

	void PlaceTile(unsigned char index, char letter, bool blank);

	void PlaceTile(unsigned char x, unsigned char y, char letter, bool blank);

	void ReplaceTile(unsigned char index, char* rack); //takes a tile off the board.

	void ReplaceTile(unsigned char x, unsigned char y, char* rack);

	void RemoveTile(unsigned char index);

	void RemoveTile(unsigned char x, unsigned char y);

	void Clear();


private:
	Tile tiles[225];

};
