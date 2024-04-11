#include "Agent.h"
#include <iostream>

using namespace std;

const static int RACK_SIZE = 7;

Agent::Agent()
{
	for (size_t i = 0; i < RACK_SIZE; i++)
	{
		rack[i] = '0';
	}
	points = 0;
	computer = true;
}

Agent::Agent(bool computer_)
{
	for (size_t i = 0; i < 7; i++)
	{
		rack[i] = '0';
	}
	points = 0;
	computer = computer_;
}

char Agent::RemoveFromRack(int index)
{
	char out = rack[index];
	rack[index] = '0';
	return out;
}

bool Agent::RemoveFromRack(char letter)
{
	for (size_t i = 0; i < RACK_SIZE; i++)
	{
		if (rack[i] == letter)
		{
			rack[i] == '0';
			return true;
		}
	}
	return false;
}

void Agent::AddtoRack(char L)
{
	for (size_t i = 0; i < RACK_SIZE; i++)
	{
		if (rack[i] == '0')
		{
			rack[i] == L;
			break;
		}
	}
}

void Agent::AddtoRack(vector<char> L)
{
	for (size_t i = 0; i < RACK_SIZE; i++)
	{
		if (rack[i] == '0' && L.size() > 0)
		{
			cout << "placing letter " << L.back() << " in rack position " << i << endl;
			rack[i] = L.back();
			L.pop_back();
		}
		
	}
}

void Agent::AddPoints(int points_)
{
	this->points += points_;
}

 vector<char> Agent::ShuffleRack()
{
	 vector<char> out;
	 for (size_t i = 0; i < RACK_SIZE; i++)
	 {
		 if (this->rack[i] != '0')
		 {
			 out.push_back(this->rack[i]);
		 }
	 }
	 return out;
}

 char* Agent::getRack()
 {
	 return this->rack;
 }

 bool Agent::getComputer()
 {
	 return computer;
 }