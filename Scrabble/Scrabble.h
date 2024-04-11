#include <string>
#include <iostream>
#include "Bag.h"
#include "Agent.h"
#include "Board.h"
#include "GADDAG.h"
//#include "GADDAG.h"

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

	void PlayBestMove();

	string getWord(Board* Store, int direction, unsigned char x, unsigned char y);

	bool VerifyBoard();

	void RacktoBoard(Agent* player, int rackIndex, unsigned char boardIndex);

	void RacktoBoard(Agent* player, char rackLetter, unsigned char boardIndex);

	void BoardtoRack(int index, char* rack);

	void Gen(int x, int y, int pos, string word, Node* arc);

	void GoOn(int x, int y, int pos, string word, Node* NewArc, Node* OldArc);

	bool isEmpty(unsigned char x, unsigned char y);


private:

	Board* BoardRep;
	Board* ToVerify;
	vector<tuple<unsigned char, char>> toVerify_Vector;
	Node* root;
	Bag* TileBag;
	Agent* Player[2];
};


