#include <string>
#include <vector>
using namespace std;

class Bag {

public:

	Bag(string filepath); //default constructor

	~Bag(); //destructor

	vector<char> Pull(int i); //recieves number of tiles to take from bag, returns pointer to array of those tiles.

	char* Replace(int* i); //recieves array to place back in bag, returns a new array of tiles that are taken out.

private:

	int Random(int i);
	vector<char> arr_;
};
