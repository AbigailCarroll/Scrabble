using namespace std;

class Tile
{
public:

	Tile();

	Tile(unsigned char letterBonus_, unsigned char wordBonus_);

	Tile(const Tile& other); //copy constructor
	
	Tile& operator=(const Tile& other); //copy assignment

	char getLetter();

	bool isBlank();

	unsigned char getLetterBonus();

	unsigned char getWordBonus();

	void setLetter(char L);

	void setBlank(bool B);


private:

	char letter;
	bool blank;
	unsigned char letterBonus;
	unsigned char wordBonus;
};
