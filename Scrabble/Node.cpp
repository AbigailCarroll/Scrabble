#include "GADDAG.h"
#include <iostream>;
using namespace std;

Node::Node()
{
	content_ = '*';
}

Node::Node(char content, int reference)
{
	content_ = content;
	children_ = {};
	reference_ = reference;
	terminal_ = false;
}

Node::~Node()
{
}

Node::Node(const Node& other)
{
	content_ = other.content_;
	children_ = other.children_;
	reference_ = other.reference_;
}

Node& Node::operator=(const Node& other)
{
	if (&other != this)
	{
		content_ = other.content_;
		children_ = other.children_;
		reference_ = other.reference_;
	}
	return *this;
}

vector<Node> Node::getChildren()
{
	return children_;
}

void Node::printChildren()
{
	cout << "Child Nodes : ";
	for (size_t i = 0; i < children_.size(); i++)
	{
		cout << children_[i].content_ << ", ";
	}
	cout << endl;
}


int Node::containsChild(char childContent) //returns a reference to the child node if it exists
{
	int left = 0, right = children_.size()-1;
	int mid;
	
	
	while (left <= right)
	{
		
		mid = (left + right) / 2;
		
		if (children_[mid].content_ == childContent && !children_[mid].terminal_)
		{
			return children_[mid].reference_;
		}
		if (children_[mid].content_ < childContent)
		{
			left = mid + 1;
		}
		else
		{
			right = mid - 1;
		}
	}
	return -1;
}

bool Node::sortChildren() //using insertion sort for quick implementation and small size of sorted array (max of 27)
{
	Node store;
	int j;
	for (size_t i = 1; i < children_.size(); i++)
	{
		store = children_[i];
		j = i;
		while (j > 0 && children_[j-1].content_ > store.content_)
		{
			children_[j] = children_[j - 1];
			j--;
		}
		children_[j] = store;
	}
	return true;
}

bool Node::addChild(Node node)
{
	children_.push_back(node);
	sortChildren();
	return true;
}

char Node::getContent()
{
	return content_;
}

int Node::getReference()
{
	return reference_;
}

void Node::setTerminal()
{
	terminal_ = true;
}

bool Node::getTerminal()
{
	return terminal_;
}