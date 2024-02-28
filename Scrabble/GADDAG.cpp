#include "GADDAG.h"
#include "Consts.h"
#include <fstream>
#include <iostream>
#include <chrono>
using namespace std;
using namespace std::chrono;

struct State
{
	struct State* children[MAX_CHILDREN];
	bool terminal;
};


GADDAG::GADDAG()
{
}

GADDAG::GADDAG(string filepath)
{
	auto start = high_resolution_clock::now();
	string line;
	int currentNode;
	if (filepath == "GADDAG.txt")
	{
		tree_.push_back(Node('*', 0));
		ifstream Dictionary(filepath);
		while (getline(Dictionary, line))
		{
			currentNode = 0;
			for (size_t i = 0; i < line.length(); i++)
			{
				if (tree_[currentNode].containsChild(line[i]) == -1 && !tree_[tree_[currentNode].containsChild(line[i])].getTerminal() ) //check if current node has a child node with the same letter as its content.
				{
					tree_.push_back(Node(line[i], tree_.size()));//create a new child node with the contents of the letter we want a node for
					cout << line[i] << endl;
					tree_[currentNode].addChild(tree_[tree_.size() - 1]); //then add it as a child node
					if (i == line.length()-1)
					{
						tree_[tree_.size() - 1].setTerminal();
						cout << "marked as terminal" << endl;
					}
					currentNode++;
				}
				else
				{
					currentNode = tree_[currentNode].containsChild(line[i]); //if child node with the same content as the letter looking to be added exists
				} //set that as the current node then move onto the next letter

			}
		}
	}
	else if (filepath == "GADDAG_export.txt")
	{
		cout << "loading from file" << endl;
		
		string reference;
		ifstream Dictionary(filepath);
		string line;
		cout << "declared variables" << endl;
		getline(Dictionary, line);
		int nodeCount = stoi(line);
		cout << nodeCount << endl;
		cout << "beggining loading" << endl;
		for (size_t i = 0; i < nodeCount; i++)
		{
			getline(Dictionary, line);
			tree_.push_back(Node(line[0], tree_.size()));
		}
		cout << "finished loading nodes" << endl;
		for (size_t i = 0; i < nodeCount; i++)
		{
			getline(Dictionary, line);
			stringstream stream(line);
			while (!stream.eof() && line != "")
			{
				getline(stream, reference, ',');
				if (reference == "")
				{
					break;
				}
				tree_[i].addChild(tree_[stoi(reference)]);
			}
		}
	}
	
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);
	cout << "loading " << filepath << " took: " << duration.count() << " milliseconds" << endl;
	cout << "Nodes generated: " << tree_.size() << endl;
}

bool GADDAG::exportGADDAG(string filename)
{
	ofstream writefile(filename);
	writefile << tree_.size() << endl;
	for (size_t i = 0; i < tree_.size(); i++)
	{
		writefile << tree_[i].getContent() << endl;
	}
	for (size_t i = 0; i < tree_.size(); i++)
	{
		for (Node i : tree_[i].getChildren())
		{
			writefile << i.getReference() << ",";
		}
		writefile << endl;
	}

	writefile.close();
}

bool GADDAG::verifyGADDAG(string filename)
{
	string line;
	ifstream Dictionary(filename);
	while (getline(Dictionary, line))
	{

	}
}

bool GADDAG::findWord(string word)
{
	Node currentNode = tree_[0];
	for (size_t i = 0; i < word.length()-1; i++)
	{
		if (currentNode.containsChild(word[i]) == -1) //if there is no child node with the right content
		{
			cout << "failed to find word: " << word << endl;
			return false;
		}
		else
		{
			currentNode = tree_[currentNode.containsChild(word[i])];
		}
	}
	if (currentNode.getTerminal()) //if current node is terminal
	{
		return true;
	}
	return false;
}


//reference a specific node from the children list tree[children[i]]