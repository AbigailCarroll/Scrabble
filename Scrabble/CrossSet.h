#include "Board.h"
#include "GADDAG.h"
using namespace std;

class CrossSet
{
public:

	CrossSet();

	CrossSet(Board* board, char* rack, Node* root);

	~CrossSet();

	void fillFromBoard(Board* board, char* rack, Node* root); //computes the cross set of a board or, for each empty tile finds each letter that could be valid in that tile

	bool GetValidWordsVertical(Board* board, vector<char> rack, Node* node, unsigned char index, unsigned char anchor, string word, unsigned char postJoin);

private:

	char set[225][26];
	vector<string> Valid_Vertical_Words;
	vector<string> Valid_Horizontal_Words;
};
