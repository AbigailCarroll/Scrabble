#include <string>
#include <vector>
using namespace std;

	bool exportGADDAG(string filename); //writes tree to a data file 

	void insert(struct Node* root, string word);

	bool find(struct Node* root, string word);

	Node* GenerateGADDAG(string filepath); //returns the root node/intitial state of the GADDAG

	bool verifyGADDAG(struct Node* root, string filepath);

//each node represents a letter as part of a word in the GADDAG trie
//nodes should be able to have children inserted dynamically to save on memory usage, otherwise each node (even terminal) would store space 
// for 27 (alphabet + joiner) child nodes. better to add them as trie is constructed.
