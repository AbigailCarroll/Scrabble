#include "GADDAG.h"
#include "Consts.h"
#include <fstream>
#include <iostream>
#include <chrono>
using namespace std;
using namespace std::chrono;

Node::Node()
{
	for (size_t i = 0; i < 27; i++)
	{
		children[i] = nullptr;
	}
	terminal = false;
}

void Node::insert(string word)
{
	//cout << word << endl;
	int reference;
	if (word[0] == '+')
	{
		reference = 26;
	}
	else
	{
		reference = word[0] - 'A';
	}
	if (children[reference] == nullptr)
	{
		//cout << "adding node with reference " << reference << endl;
		children[reference] = new Node();
	}
	word = word.substr(1);
	if (word.length() > 0)
	{
		children[reference]->insert(word);
	}
	else
	{
		this->terminal = true;
	}
}

bool Node::find(string word)
{
	int reference;
	//cout << "Word[i] is: " << word[i] << endl;
	if (word[0] == '+')
	{
		reference = 26;
	}
	else
	{
		reference = word[0] - 'A';
	}
	//cout << reference << endl;
	if (!children[reference])
	{
		return false;
	}
	word = word.substr(1);
	if (word.length() > 0)
	{
		return children[reference]->find(word);
	}
	return terminal;

}


void Node::GenerateGADDAG(string filepath)
{
	cout << "Starting GADDAG generation..." << endl;
	auto start = high_resolution_clock::now();
	string line;
	ifstream Dictionary(filepath);
	while (getline(Dictionary, line))
	{
		insert(line);
	}
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);
	cout << "Finished GADDAG generation" << endl;
	cout << "loading " << filepath << " took: " << duration.count() << " milliseconds" << endl;
}

Node* Node::findChild(char L)
{
	L = toupper(L);
	if (children[L - 41] != nullptr)
	{
		return children[L - 41];
	}
	return nullptr;
}

bool Node::verifyGADDAG(string filepath)
{
	cout << "Starting verification" << endl;
	string line;
	ifstream Dictionary(filepath);
	while (getline(Dictionary, line))
	{
		if (!find(line))
		{
			cout << "GADDAG Invalid" << endl;
			return false;
		}
	}
	cout << "GADDAG Verified" << endl;
	return true;
}




//reference a specific node from the children list tree[children[i]]