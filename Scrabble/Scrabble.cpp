#include <string>
#include "Scrabble.h"
using namespace std;

Scrabble::Scrabble()
{
	BoardRep = new Board();
	ToVerify = new Board();
	root = new Node();
	root->GenerateGADDAG("GADDAG.txt");
	root->verifyGADDAG("GADDAG.txt");
	TileBag = new Bag("Bag.txt");
	for (size_t i = 0; i < 2; i++)
	{
		Player[i] = new Agent;
		Player[i]->AddtoRack(TileBag->Pull(7));
	}
	cross_set = new CrossSet();
}

Scrabble::Scrabble(bool playerOneComp, bool playerTwoComp, string bagFilepath)
{
	BoardRep = new Board();
	ToVerify = new Board();
	root = new Node();
	root->GenerateGADDAG("GADDAG.txt");
	TileBag = new Bag(bagFilepath);
	Player[0] = new Agent(playerOneComp);
	Player[1] = new Agent(playerTwoComp);
	//Player[0]->AddtoRack(TileBag->Pull(7));
	//Player[1]->AddtoRack(TileBag->Pull(7));
	Player[0]->AddtoRack({ 'C', 'A', 'T', 'B', 'A', 'G', 'S' }); //for testing
	Player[1]->AddtoRack({ 'C', 'A', 'T', 'B', 'A', 'G', 'S' }); //for testing
	cross_set = new CrossSet();
} 

Board* Scrabble::getFullBoard()
{
	Board *Store = BoardRep;
	for (size_t i = 0; i < toVerify_Vector.size(); i++)
	{
		Store->PlaceTile(get<0>(toVerify_Vector[i]), get<1>(toVerify_Vector[i]), false);
	}
	return Store;
}

Board* Scrabble::getBoardRep()
{
	return BoardRep;
}

Board* Scrabble::getToVerify()
{
	return ToVerify;
}

Agent* Scrabble::getAgent(int index)
{
	return Player[index];
}

vector<tuple<unsigned char, char>> Scrabble::getToVerify_Vector()
{
	return toVerify_Vector;
}

void Scrabble::RacktoBoard(Agent* player, int rackIndex, unsigned char boardIndex)
{
	
	char letter = player->RemoveFromRack(rackIndex);
	//cout << "Placing letter: " << letter << " at position " << int(boardIndex) << endl;
	ToVerify->PlaceTile(boardIndex, letter, false);
	toVerify_Vector.push_back(make_tuple(boardIndex, letter));
}

void Scrabble::RacktoBoard(Agent* player, char rackLetter, unsigned char boardIndex)
{
	if (player->RemoveFromRack(rackLetter))
	{
		ToVerify->PlaceTile(boardIndex, rackLetter, false);
		toVerify_Vector.push_back(make_tuple(boardIndex, rackLetter));
	}
	else
	{
		cout << "ERROR: Tried to remove letter from rack that wasn't there" << endl;
	}
	
}

void Scrabble::BoardtoRack(int index, char* rack)
{
	if (ToVerify->getLetter(index) != '0')
	{
		for (size_t i = 0; i < toVerify_Vector.size(); i++)
		{
			if (get<0>(toVerify_Vector[i]) == index)
			{
				toVerify_Vector.erase(toVerify_Vector.begin() + i);
			}
		}
		ToVerify->ReplaceTile(index, rack);
	}
}

char* Scrabble::getPlayerRack(int index)
{
	return Player[index]->getRack();
}

bool Scrabble::isEmpty(unsigned char x, unsigned char y)
{
	if (BoardRep->getLetter(x,y) == '0' && ToVerify->getLetter(x,y) == '0')
	{
		return true;
	}
	return false;
}




string Scrabble::getWord(Board* Store, int direction, unsigned char x, unsigned char y)
{
	string word = "";
	int targetX = x, targetY = y;
	while (Store->getLetter(targetX, targetY) != '0') //gets all letters to the left and top of the target tile.
	{
		word = Store->getLetter(targetX, targetY) + word;
		targetX = targetX - 1 + direction;
		targetY = targetY - direction;
	}
	targetX = x - direction + 1;
	targetY = y + direction;
	while (Store->getLetter(targetX, targetY) != '0') //gets all letters to the right and below the target tile.
	{
		word = Store->getLetter(targetX, targetY);
		targetX = targetX + 1 - direction;
		targetY = targetY + direction;
	}
	cout << "'" << word << "'" << endl;
	return word;
}

bool Scrabble::VerifyBoard()
{
	int start = 0;
	Board Store = *BoardRep;
	for (size_t i = 0; i < toVerify_Vector.size(); i++)
	{
		Store.PlaceTile(get<0>(toVerify_Vector[i]), get<1>(toVerify_Vector[i]), false);
	}
	bool xLine = true;
	bool yLine = true;
	for (size_t i = 0; i < toVerify_Vector.size(); i++) //this loop ensures that all the tiles fall along the same vertical or horizontal line
	{//need to check that either all X values are the same, or all Y values are the same, maximum O(2n) time, where N is the number of tiles
		if (get<0>(toVerify_Vector[i]) % 15 != get<0>(toVerify_Vector[0]) % 15)//check xs are the same
		{
			xLine = false;
		}
		if (get<0>(toVerify_Vector[i]) / 15 != get<0>(toVerify_Vector[0]) / 15)//check ys are the same
		{
			yLine = false;
		}
	}
	if (xLine == false && yLine == false) { cout << "failed due to line rule" << endl; return false; }

	cout << "Vector size: " << toVerify_Vector.size() << endl;
	for (size_t i = 0; i < toVerify_Vector.size(); i++)
	{
		vector<string> words;
		words.push_back(getWord(&Store, 0, get<0>(toVerify_Vector[i]) % 15, get<0>(toVerify_Vector[i]) / 15 )); //checks left/right
		words.push_back(getWord(&Store, 1, get<0>(toVerify_Vector[i]) % 15, get<0>(toVerify_Vector[i]) / 15));  //checks up/down
		cout << "Words Vector size: " << toVerify_Vector.size() << endl;
		for (size_t j = 0; j < words.size(); j++)
		{
			cout << "Words[" << j << "]: " << words[j] << endl;
			if (words[j].length() > 1)
			{
				cout << "word to verify is: " << words[j] << endl;
				reverse(words[j].begin(), words[j].end());
				cout << "reversed word is: " << words[j] << endl;
				if (!root->find(words[j]))
				{
					return false;
				}
			}
		}
	}// board has been verified, cleanup step
	*BoardRep = Store;
	cout << "TEST 1" << endl;

	ToVerify->Clear();
	toVerify_Vector.clear();
	return true;
}

void Scrabble::Gen(int x, int y, int pos, string word, Node* arc, char* rack)
{
	char letter = BoardRep->getLetter(x, y);
	if (letter != '0')
	{
		Node* next = arc->findChild(letter);
		GoOn(x, y, pos, word, next, arc, rack);
	}
	else if(!isRackEmpty(rack))
	{
		//get valid squares
	}
}

void Scrabble::GoOn(int x, int y, int pos, string word, Node* NewArc, Node* OldArc, char* rack)
{

}

bool Scrabble::isRackEmpty(char* rack)
{
	for (size_t i = 0; i < 7; i++)
	{
		if (rack[i] != '0')
		{
			return false;
		}
	}
	return true;
}

vector<char> Scrabble::getValidLetters(char* rack, int x, int y)
{
	vector<char> output;

}

void Scrabble::GenerateMoves(int playernum)
{
	cout << "Starting move generation" << endl;
	vector<char> rack_vector;
	for (size_t j = 0; j < 7; j++)
	{
		if (Player[playernum]->getRack()[j] != '0')
		{
			rack_vector.push_back(Player[playernum]->getRack()[j]);
		}
	}
	for (size_t i = 0; i < 225; i++)
	{
		unsigned char x = i % 15;
		unsigned char y = i / 15;
		if (BoardRep->getLetter(i) != '0')
		{
			cout << "target letter is: " << BoardRep->getLetter(i) << endl;
			string word;
			word.push_back(BoardRep->getLetter(i));
			if (root->findChild(BoardRep->getLetter(i)) == nullptr)
			{
				cout << "child was nullptr" << endl;
			}
			else
			{
				cout << "beggining traversal from anchors: " << BoardRep->getLetter(i) << endl;
				GetWordsVertical(rack_vector, root->findChild(BoardRep->getLetter(i)), i, i, word, 1);
			}
		}
	}
	for (size_t i = 0; i < Valid_Vertical_Words.size(); i++)
	{
		cout << "Word " << i << ": " << Valid_Vertical_Words[i] << endl;
	}
}

bool Scrabble::GetWordsVertical(vector<char> rack, Node* node, unsigned char currentSpace, unsigned char anchor, string word, unsigned char postJoin)
{
	unsigned char x = currentSpace % 15;
	unsigned char y = currentSpace / 15;
	
	for (size_t i = 0; i < rack.size(); i++)
	{
		char store = rack[i];
		vector<char> new_rack = rack;
		new_rack.erase(new_rack.begin() + i);
		if (node->findChild(store) != nullptr)
		{
			GetWordsVertical(new_rack, node->findChild(store), currentSpace, anchor, word + store, postJoin);
		}
		
	}
	if (node->findChild('+') != nullptr)
	{
		GetWordsVertical(rack, node->findChild('+'), currentSpace, anchor, word + '+', postJoin);
	}
	if (node->getTerminal())
	{
		Valid_Vertical_Words.push_back(word);
	}
}