#include <string>
using namespace std;

class Bag {

public:

	Bag(); //default constructor

	~Bag(); //destructor

	char* Pull(int i); //recieves number of tiles to take from bag, returns pointer to array of those tiles.

	char* Replace(int* i); //recieves array to place back in bag, returns a new array of tiles that are taken out.

	int getSize();

private:

	int Random(int i);

	int size_;
	vector<char> arr_;
};
