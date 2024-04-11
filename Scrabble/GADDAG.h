#include <string>
#include <vector>
using namespace std;

class Node
{
public:

	Node();

	bool exportGADDAG(string filename); //writes tree to a data file 

	void insert(string word);

	bool find(string word);

	void GenerateGADDAG(string filepath); //returns the root node/intitial state of the GADDAG

	bool verifyGADDAG(string filepath);

	Node* findChild(char L);

private:

	Node* children[27];
	bool terminal;
};



//each node represents a letter as part of a word in the GADDAG trie
//nodes should be able to have children inserted dynamically to save on memory usage, otherwise each node (even terminal) would store space 
// for 27 (alphabet + joiner) child nodes. better to add them as trie is constructed.