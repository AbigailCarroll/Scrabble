#include <string>
#include <random>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Bag.h"
using namespace std;


Bag::Bag(string filepath)
{
	ifstream file(filepath);
	string line;
	int i = 0;
	while (getline(file, line))
	{
		int num = stoi(line);
		for (size_t j = 0; j < num; j++)
		{
			arr_.push_back(char(i+65));
		}
		i++;
	}
	 
}

Bag::~Bag()
{
	arr_.clear();
}

char Bag::Pull()
{
	if (arr_.size() < 1)
	{
		cout << "ERROR: Bag is empty" << endl;
		return '0';
	}
	int rand;
	mt19937 rng(time(NULL));
	char output;
	rand = Random(arr_.size() - 1);
	output = arr_[rand];
	arr_.erase(arr_.begin() + rand);
	return output;
}

vector<char> Bag::Pull(int i)
{
	if (arr_.size() < 7)
	{
		i = arr_.size();
	}
	vector<char> output;
	int rand;
	mt19937 rng(time(NULL));
	char copy;
	for (size_t j = 0; j < i; j++)
	{
		rand = Random(arr_.size()-1);
		//cout << "Rand: " << rand << endl;
		//cout << "arr_[rand]: " << arr_[rand] << endl;
		output.push_back(arr_[rand]);
		arr_.erase(arr_.begin() + rand); // erases letter at index rand
		//cout << arr_.size() << endl;
	}
	cout << "Rack: ";
	for (size_t j = 0; j < i; j++)
	{
		cout << output[j] << ", ";
	}
	cout << endl;
	return output;
}


int Bag::Random(int i) //generates random number between 0 and i (inclusive of both)
{
	int output;
	random_device device;
	mt19937 rng(device());
	uniform_int_distribution<mt19937::result_type> random(0, i);
	output = random(rng);
	return output;
}

bool Bag::isBagEmpty()
{
	if (arr_.size() == 0)
	{
		return true;
	}
	return false;
}

