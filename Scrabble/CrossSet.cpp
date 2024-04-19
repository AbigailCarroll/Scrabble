#include "CrossSet.h"
#include <iostream>

using namespace std;

CrossSet::CrossSet()
{
	for (size_t i = 0; i < 225; i++)
	{
		for (size_t j = 0; j < 26; j++)
		{
			set[i][j] = '0';
		}
	}
}

CrossSet::CrossSet(Board* board, char* rack, Node* root)
{
	for (size_t i = 0; i < 225; i++)
	{
		for (size_t j = 0; j < 26; j++)
		{    
			set[i][j] = '0';
		}
	}
	fillFromBoard(board, rack, root);
}

CrossSet::~CrossSet()
{
	for (size_t i = 0; i < 225; i++)
	{
		delete set[i];
	}
	delete set;
}

void CrossSet::fillFromBoard(Board* board, char* rack, Node* root)
{
	cout << "Starting move generation" << endl;
	for (size_t i = 0; i < 225; i++)
	{
		unsigned char x = i % 15;
		unsigned char y = i / 15;
		if (board->getLetter(i) != '0')
		{
			vector<char> rack_vector;
			for (size_t j = 0; j < 7; j++)
			{
				if (rack[i] != '0')
				{
					rack_vector.push_back(rack[j]);
				}
			}
			string word = "" + board->getLetter(i);
			if (root->findChild(board->getLetter(i)) == nullptr)
			{
				cout << "child was nullptr" << endl;
			}
			else
			{
				cout << "beggining traversal from anchor: " << board->getLetter(i) << endl;
				GetValidWordsVertical(board, rack_vector, root->findChild(board->getLetter(i)), i, i, word, 1);
			}
		}
	}
}

bool CrossSet::GetValidWordsVertical(Board* board, vector<char> rack, Node* node, unsigned char currentSpace, unsigned char anchor, string word, unsigned char postJoin) //1 for no, -1 for yes
{
	cout << "traversing node" << endl;
	unsigned char x = currentSpace % 15;
	unsigned char y = currentSpace / 15;
	if (node->getTerminal())
	{
		Valid_Vertical_Words.push_back(word);
		cout << word << endl;
	}
	for (size_t i = 0; i < rack.size(); i++)
	{
		char store = rack[i];
		vector<char> new_rack = rack;
		new_rack.erase(new_rack.begin() + i);
		if (node->findChild(store) != nullptr)
		{
			GetValidWordsVertical(board, new_rack, node->findChild(store), currentSpace, anchor, word + store, postJoin);
		}
	}
}

