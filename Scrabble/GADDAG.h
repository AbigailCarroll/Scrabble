#include <string>
#include <vector>
#include "Node.h"
using namespace std;

class GADDAG {
public:

	GADDAG(); //default constructor

	GADDAG(string filepath); //constructs the whole GADDAG from a preprepared text file. Might be better to have the ability to save the GADDAG rather than construct it new
	//each time the program is run.

	bool exportGADDAG(string filename); //writes tree to a data file 

	bool verifyGADDAG(string filename);

	bool findWord(string word);

private:

	vector<Node> tree_;

};




//each node represents a letter as part of a word in the GADDAG trie
//nodes should be able to have children inserted dynamically to save on memory usage, otherwise each node (even terminal) would store space 
// for 27 (alphabet + joiner) child nodes. better to add them as trie is constructed.
//tree is directed, so there will be nodes of duplicate letters. nodes should not be identified by the letter they represent.