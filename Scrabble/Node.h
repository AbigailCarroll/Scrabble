#include <string>
using namespace std;

class Node {
public:

	Node(); //default constructor

	Node(char content, int reference); //parametised constructor

	~Node(); //destructor

	Node(const Node& other); //copy constructor

	Node& operator=(const Node& other); //copy assignment

	vector<Node> getChildren();

	void printChildren();

	int containsChild(char childContent); //performs binary search to check if a node contains a child with the content [childContent], then returns its reference

	bool sortChildren();

	bool addChild(Node node);

	char getContent();

	int getReference();

	void setTerminal();

	bool getTerminal();

private:

	char content_;
	vector<Node> children_; //array of all child nodes
	bool terminal_;

};




//each node represents a letter as part of a word in the GADDAG trie
//nodes should be able to have children inserted dynamically to save on memory usage, otherwise each node (even terminal) would store space 
// for 27 (alphabet + joiner) child nodes. better to add them as trie is constructed.
//tree is directed, so there will be nodes of duplicate letters. nodes should not be identified by the letter they represent.
