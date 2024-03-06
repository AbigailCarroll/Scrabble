#include <string>
#include <random>
#include <iostream>
#include "Bag.h"
using namespace std;


Bag::Bag()
{
	size_ = 99; //always points to last element of array

	arr_ = new char[101]; //100 tiles, plus one for the empty location used to make the shrinking easier.
	//this is, frankly, a dumb way to populate the bag
	//but i cannot be bothered to change it right now
	//save for when procrastination is necessary
	//and change it to arr_ = {'A', 'A', ..... 'Z'};
	for (size_t i = 0; i < 9; i++) { arr_[i] = 'A'; }
	for (size_t i = 9; i < 11; i++) { arr_[i] = 'B'; }
	for (size_t i = 11; i < 13; i++) { arr_[i] = 'C'; }
	for (size_t i = 13; i < 17; i++) { arr_[i] = 'D'; }
	for (size_t i = 17; i < 29; i++) { arr_[i] = 'E'; }
	for (size_t i = 29; i < 31; i++) { arr_[i] = 'F'; }
	for (size_t i = 31; i < 34; i++) { arr_[i] = 'G'; }
	for (size_t i = 34; i < 36; i++) { arr_[i] = 'H'; }
	for (size_t i = 36; i < 45; i++) { arr_[i] = 'I'; }
	arr_[45] = 'J';
	arr_[46] = 'K';
	for (size_t i = 47; i < 51; i++) { arr_[i] = 'L'; }
	for (size_t i = 51; i < 53; i++) { arr_[i] = 'M'; }
	for (size_t i = 53; i < 59; i++) { arr_[i] = 'N'; }
	for (size_t i = 59; i < 67; i++) { arr_[i] = 'O'; }
	for (size_t i = 67; i < 69; i++) { arr_[i] = 'P'; }
	arr_[69] = 'Q';
	for (size_t i = 70; i < 76; i++) { arr_[i] = 'R'; }
	for (size_t i = 76; i < 80; i++) { arr_[i] = 'S'; }
	for (size_t i = 80; i < 86; i++) { arr_[i] = 'T'; }
	for (size_t i = 86; i < 90; i++) { arr_[i] = 'U'; }
	for (size_t i = 90; i < 92; i++) { arr_[i] = 'V'; }
	for (size_t i = 92; i < 94; i++) { arr_[i] = 'W'; }
	arr_[94] = 'X';
	for (size_t i = 95; i < 97; i++) { arr_[i] = 'Y'; }
	arr_[97] = 'Z';
	for (size_t i = 98; i < 100; i++) { arr_[i] = '['; } // represents blank, used as it's ascii value is 91, one more than Z at 90.
	arr_[100] = '0'; // represents empty, used for shrinking the bag
}

char* Bag::Pull(int i)
{
	if (size_ < 7)
	{
		i = size_;
	}
	char* outputarr = new char[i];
	int rand;
	mt19937 rng(time(NULL));
	char copy;
	for (size_t j = 0; j < i; j++)
	{
		rand = Random(size_);
		copy = arr_[rand];
		outputarr[j] = copy;
		
		for (size_t k = rand; k < size_; k++)
		{
			arr_[k] = arr_[k + 1];
		}
		size_--;
	}
	for (size_t j = 0; j < i; j++)
	{
		cout << outputarr[j] << endl;
	}
	return outputarr;
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

int Bag::getSize()
{
	return size_;
}