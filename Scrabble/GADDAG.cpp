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
	if (children[reference] == nullptr) //if no child node currently exists
	{
		children[reference] = new Node();
	}
	word = word.substr(1);
	if (word.length() > 0)
	{
		children[reference]->insert(word);
	}
	else
	{
		//this->terminal = true;
		children[reference]->terminal = true;
	}
}

vector<char> Node::getCrossSetChildren(string word)
{
	vector<char> output;

	int reference;
	if (word[0] == '+')
	{
		reference = 26;
	}
	else
	{
		reference = word[0] - 'A';
	}
	if (reference < 0 || reference > 26)
	{
		return output;
	}
	//cout << reference << endl;
	if (children[reference] == nullptr)
	{
		return output;
	}
	word = word.substr(1);
	if (word.length() > 0)
	{
		return children[reference]->getCrossSetChildren(word);
	}
	for (size_t i = 0; i < 26; i++)
	{
		if (children[i] != nullptr)
		{
			output.push_back(i);
		}
	}
	return output;
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
	if (reference < 0 || reference > 26)
	{
		//cout << endl;
		//cout << "ERROR: REFERENCE PROVIDED FOR NODE->FIND OUTSIDE BOUNDS OF ARRAY" << endl;
		//cout << "REFERENCE: " << reference << endl;
		//cout << endl;
		return false;
	}
	//cout << reference << endl;
	if (children[reference] == nullptr)
	{
		return false;
	}
	word = word.substr(1);
	if (word.length() > 0)
	{
		return children[reference]->find(word);
	}
	return children[reference]->terminal;

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
	if ((L-'A' > 26 || L-'A' < 0) && L != '+')
	{
		//cout << "ERROR: CHAR L OUTSIDE BOUNDS OF ARRAY AT: " << L <<"/"<< int(L)<< "/" << int(L-'A') << endl;
		return nullptr;
	}
	if (L == '+')
	{
		return children[26];
	}
	else if (L - 'A' < 26)
	{
		return children[L - 'A'];
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

Node* Node::getChild(unsigned char index) //accepts values 0-26
{
	return children[index];
}

bool Node::getTerminal()
{
	return terminal;
}





//reference a specific node from the children list tree[children[i]]