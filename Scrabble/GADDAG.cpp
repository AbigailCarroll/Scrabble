//#include "GADDAG.h"
#include "Consts.h"
#include <fstream>
#include <iostream>
#include <chrono>
using namespace std;
using namespace std::chrono;

struct Node
{
	struct Node* children[MAX_CHILDREN]; //child nodes are stored as an index to the letter they represent, i.e children[0] is the node for 'A', if it is nullptr the node has no children 'A'
	bool terminal;
};

struct Node* newNode(void) //creates and return new empty node
{
	struct Node *node = new Node;
	for (size_t i = 0; i < MAX_CHILDREN; i++)
	{
		node->children[i] = nullptr;
	}
	node->terminal = false;
	return node;
};

void insert(struct Node* root, string word)
{
	struct Node *CurrentNode = root;
	for (size_t i = 0; i < word.length(); i++)
	{
		int reference;
		if (word[i] == '+')
		{
			reference = 26;
		}
		else
		{
			reference = word[i] - 'A';
		}
		if (!CurrentNode->children[reference]) //checks if a child for the right letter exists
		{
			CurrentNode->children[reference] = newNode(); //if not, makes it
		}
		CurrentNode = CurrentNode->children[reference]; //then sets it as current node
	}

	CurrentNode->terminal = true;
}

bool find(struct Node* root, string word)
{
	struct Node* CurrentNode = root;

	for (size_t i = 0; i < word.length(); i++)
	{
		int reference;
		if (word[i] == '+')
		{
			reference = 26;
		}
		else
		{
			reference = word[i] - 'A';
		}
		if (!CurrentNode->children[reference])
		{
			return false;
		}
		CurrentNode = CurrentNode->children[reference];
	}
	return CurrentNode->terminal;
}

Node* GenerateGADDAG(string filepath) //runs in ~1.5 seconds with GADDAG.txt
{
	auto start = high_resolution_clock::now();
	string line;
	struct Node* root = newNode();
	ifstream Dictionary(filepath);
	while (getline(Dictionary, line))
	{
		insert(root, line);
	}
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);
	cout << "loading " << filepath << " took: " << duration.count() << " milliseconds" << endl;

	return root;
}

Node* findChild(Node* node, char L)
{
	L = toupper(L);
	if (node->children[L - 41] != nullptr)
	{
		return node->children[L - 41];
	}
	return nullptr;
}

bool verifyGADDAG(struct Node* root, string filepath)
{
	string line;
	ifstream Dictionary(filepath);
	while (getline(Dictionary, line))
	{
		if (!find(root, line))
		{
			return false;
		}
	}
	return true;
}

void Gen(int pos, string word, char* rack, Node arc, char(*BoardRep)[BOARD_SIZE])
{

}

void GoOn(int pos, char L, string word, char* rack, Node NewArc, Node OldArc)
{

}


//reference a specific node from the children list tree[children[i]]