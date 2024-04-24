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




