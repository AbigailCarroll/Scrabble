#include <vector>
using namespace std;

class Agent
{

public:

	Agent();

	Agent(bool computer_);

	char RemoveFromRack(int index);

	bool RemoveFromRack(char L);

	void AddtoRack(char L);

	void AddtoRack(vector<char> L);

	vector<char> RemoveALLRack(); //takes all tiles on the rack and places them back into the bag

	void AddPoints(int points_);

	char* getRack();

	bool getComputer();

	int getPoints();

	void setPoints(int points_);

private:
	char rack[7];
	int points;
	bool computer; //if true the player is controlled by the computer agent;
};
