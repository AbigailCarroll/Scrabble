#include <string>
#include <iostream>
#include <set>
#include "Bag.h"
#include "Agent.h"
#include "Board.h"
#include "GADDAG.h"

class Scrabble
{
public:

	Scrabble();

	Scrabble(bool playerOneComp, bool playerTwoComp, string bagFilepath); //(false, true, "Bag.txt")

	Board* getFullBoard();

	Board* getBoardRep();

	Board* getToVerify();

	vector<tuple<unsigned char, char>> getToVerify_Vector();

	Agent* getAgent(int index);

	char* getPlayerRack(int index);

	string getWord(Board* Store, int direction, unsigned char x, unsigned char y);

	bool VerifyBoard(int playernum);

	void RacktoBoard(Agent* player, int rackIndex, unsigned char boardIndex);
	
	void RacktoBoard(Agent* player, int rackIndex, unsigned char boardIndex, bool blank);

	void RacktoBoard(Agent* player, char rackLetter, unsigned char boardIndex);

	void BoardtoRack(int index, char* rack);

	void ALLBoardtoRack(char* rack);

	void Gen(int x, int y, int pos, string word, Node* arc, char* rack);

	void GoOn(int x, int y, int pos, string word, Node* NewArc, Node* OldArc, char* rack);

	bool isEmpty(unsigned char x, unsigned char y);

	bool isRackEmpty(char* rack);

	vector<char> getValidLetters(char* rack, int x, int y);

	void GenerateMoves(int playernum);

	bool GetWordsVertical(vector<char> rack, Node* node, unsigned char index, unsigned char anchor, string word, int postJoin, int points, float multiplier, int rack_size);

	bool GetWordsHorizontal(vector<char> rack, Node* node, unsigned char index, unsigned char anchor, string word, int postJoin, int points, float multiplier, int rack_size);

	int getPointValue(char L, unsigned char board_index);

	void PlayBestMove(int playernum);

	void FillRack(int playernum);

	void DisplayBoardData();

	int getPoints(int playernum);

	bool isOver();

	int getPoints(Board* Store);

private:

	Board* BoardRep;
	Board* ToVerify;
	vector<tuple<unsigned char, char>> toVerify_Vector;
	Node* root;
	Bag* TileBag;
	Agent* Player[2];
	set<tuple<unsigned char, string, int>> Valid_Vertical_Words; //stores the anchor square of the word, the word and it's point value.
	set<tuple<unsigned char, string, int>> Valid_Horizontal_Words; //anchor square, word, point value
	int point_value[26] = { 1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10 };
	bool firstTurn;
	bool isOver_;
};


