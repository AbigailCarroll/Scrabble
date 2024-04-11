#include <string>
#include <random>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Bag.h"
using namespace std;


Bag::Bag(string filepath)
{
	/*for (size_t i = 0; i < 9; i++) { arr_.push_back('A'); }
	for (size_t i = 9; i < 11; i++) { arr_.push_back('B'); }
	for (size_t i = 11; i < 13; i++) { arr_.push_back('C'); }
	for (size_t i = 29; i < 31; i++) { arr_.push_back('F'); }
	for (size_t i = 13; i < 17; i++) { arr_.push_back('D'); }
	for (size_t i = 17; i < 29; i++) { arr_.push_back('E'); }
	for (size_t i = 31; i < 34; i++) { arr_.push_back('G'); }
	for (size_t i = 34; i < 36; i++) { arr_.push_back('H'); }
	for (size_t i = 36; i < 45; i++) { arr_.push_back('I'); }
	arr_.push_back('J');
	arr_.push_back('K');
	for (size_t i = 47; i < 51; i++) { arr_.push_back('L'); }
	for (size_t i = 51; i < 53; i++) { arr_.push_back('M'); }
	for (size_t i = 53; i < 59; i++) { arr_.push_back('N'); }
	for (size_t i = 59; i < 67; i++) { arr_.push_back('O'); }
	for (size_t i = 67; i < 69; i++) { arr_.push_back('P'); }
	arr_.push_back('Q');
	for (size_t i = 70; i < 76; i++) { arr_.push_back('R'); }
	for (size_t i = 76; i < 80; i++) { arr_.push_back('S'); }
	for (size_t i = 80; i < 86; i++) { arr_.push_back('T'); }
	for (size_t i = 86; i < 90; i++) { arr_.push_back('U'); }
	for (size_t i = 90; i < 92; i++) { arr_.push_back('V'); }
	for (size_t i = 92; i < 94; i++) { arr_.push_back('W'); }
	arr_.push_back('X');
	for (size_t i = 95; i < 97; i++) { arr_.push_back('Y'); }
	arr_.push_back('Z');
	for (size_t i = 98; i < 100; i++) { arr_.push_back('['); } // represents blank, used as it's ascii value is 91, one more than Z at 90.*/

	ifstream file(filepath);
	string line;
	int i = 0;
	while (getline(file, line))
	{
		int num = stoi(line);
		for (size_t i = 0; i < num; i++)
		{
			arr_.push_back(char(i+65));
		}
	}
	 
}

Bag::~Bag()
{
	arr_.clear();
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
