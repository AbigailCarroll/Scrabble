#include <string>
#include "Scrabble.h"
using namespace std;

Scrabble::Scrabble()
{
	BoardRep = new Board();
	ToVerify = new Board();
	root = new Node();
	root->GenerateGADDAG("GADDAG.txt");
	root->verifyGADDAG("GADDAG.txt");
	TileBag = new Bag("Bag.txt");
	for (size_t i = 0; i < 2; i++)
	{
		Player[i] = new Agent;
		Player[i]->AddtoRack(TileBag->Pull(7));
	}
	firstTurn = true;
	isOver_ = false;
}

Scrabble::Scrabble(bool playerOneComp, bool playerTwoComp, string bagFilepath)
{
	BoardRep = new Board();
	ToVerify = new Board();
	root = new Node();
	root->GenerateGADDAG("GADDAG.txt");
	TileBag = new Bag(bagFilepath);
	Player[0] = new Agent(playerOneComp);
	Player[1] = new Agent(playerTwoComp);
	Player[0]->AddtoRack(TileBag->Pull(7));
	Player[1]->AddtoRack(TileBag->Pull(7));
	//Player[0]->AddtoRack({ '[', 'A', 'T', 'B', 'A', 'G', 'S' }); //for testing
	//Player[1]->AddtoRack({ 'C', 'A', 'T', 'B', 'A', 'G', 'S' }); //for testing
	firstTurn = true;
	isOver_ = false;
} 

Board* Scrabble::getFullBoard()
{
	Board *Store = BoardRep;
	for (size_t i = 0; i < toVerify_Vector.size(); i++)
	{
		Store->PlaceTile(get<0>(toVerify_Vector[i]), get<1>(toVerify_Vector[i]), false);
	}
	return Store;
}

Board* Scrabble::getBoardRep()
{
	return BoardRep;
}

Board* Scrabble::getToVerify()
{
	return ToVerify;
}

Agent* Scrabble::getAgent(int index)
{
	return Player[index];
}

vector<tuple<unsigned char, char>> Scrabble::getToVerify_Vector()
{
	return toVerify_Vector;
}

void Scrabble::RacktoBoard(Agent* player, int rackIndex, unsigned char boardIndex)
{
	
	char letter = player->RemoveFromRack(rackIndex);
	//cout << "Placing letter: " << letter << " at position " << int(boardIndex) << endl;
	if (letter == '[')
	{
		ToVerify->PlaceTile(boardIndex, letter, true);
	}
	else
	{
		ToVerify->PlaceTile(boardIndex, letter, false);
	}
	toVerify_Vector.push_back(make_tuple(boardIndex, letter));
}

void Scrabble::RacktoBoard(Agent* player, int rackIndex, unsigned char boardIndex, bool blank)
{

	char letter = player->RemoveFromRack(rackIndex);
	//cout << "Placing letter: " << letter << " at position " << int(boardIndex) << endl;
	ToVerify->PlaceTile(boardIndex, letter, blank);
	toVerify_Vector.push_back(make_tuple(boardIndex, letter));
}

void Scrabble::RacktoBoard(Agent* player, char rackLetter, unsigned char boardIndex)
{
	if (player->RemoveFromRack(rackLetter))
	{
		ToVerify->PlaceTile(boardIndex, rackLetter, false);
		toVerify_Vector.push_back(make_tuple(boardIndex, rackLetter));
	}
	else
	{
		cout << "ERROR: Tried to remove letter from rack that wasn't there" << endl;
	}
	
}

void Scrabble::BoardtoRack(int index, char* rack)
{
	if (ToVerify->getLetter(index) != '0')
	{
		for (size_t i = 0; i < toVerify_Vector.size(); i++)
		{
			if (get<0>(toVerify_Vector[i]) == index)
			{
				toVerify_Vector.erase(toVerify_Vector.begin() + i);
			}
		}
		ToVerify->ReplaceTile(index, rack);
	}
}

void Scrabble::ALLBoardtoRack(char* rack)
{
	for (size_t i = 0; i < toVerify_Vector.size(); i++)
	{
		ToVerify->ReplaceTile(get<0>(toVerify_Vector[i]), rack);
	}
	toVerify_Vector.clear();
}

char* Scrabble::getPlayerRack(int index)
{
	return Player[index]->getRack();
}

bool Scrabble::isEmpty(unsigned char x, unsigned char y)
{
	if (BoardRep->getLetter(x,y) == '0' && ToVerify->getLetter(x,y) == '0')
	{
		return true;
	}
	return false;
}

int Scrabble::getPoints(Board* Store)
{
	int points = 0;
	int multiplier = 1;
	int direction;
	int currentSpace = get<0>(toVerify_Vector[0]);
	if (abs(get<0>(toVerify_Vector[0]) - get<0>(toVerify_Vector[1])) < 15)
	{
		direction = 1;
	}
	else
	{
		direction = 15;
	}
	while (Store->getLetter(currentSpace) != '0')
	{
		int letterMultiplier = 1;
		for (size_t i = 0; i < toVerify_Vector.size(); i++)
		{
			if (get<0>(toVerify_Vector[i]) == currentSpace)
			{
				letterMultiplier = letterMultiplier * Store->getLetterBonus(currentSpace);
				multiplier = multiplier * Store->getWordBonus(currentSpace);
				break;
			}
		}
		if (!Store->getBlank(currentSpace))
		{
			points += point_value[Store->getLetter(currentSpace) - 'A'] * letterMultiplier;
		}
		currentSpace -= direction;
	}
	currentSpace = get<0>(toVerify_Vector[0]) + direction;
	while (Store->getLetter(currentSpace) != '0')
	{
		int letterMultiplier = 1;
		for (size_t i = 0; i < toVerify_Vector.size(); i++)
		{
			if (get<0>(toVerify_Vector[i]) == currentSpace)
			{
				letterMultiplier = letterMultiplier * Store->getLetterBonus(currentSpace);
				multiplier = multiplier * Store->getWordBonus(currentSpace);
				break;
			}
		}
		if (!Store->getBlank(currentSpace))
		{
			points += point_value[Store->getLetter(currentSpace) - 'A'] * letterMultiplier;
		}
		currentSpace += direction;
	}
	if (direction == 1) { direction = 15; }
	else { direction = 1; }
	for (size_t i = 0; i < toVerify_Vector.size(); i++) //checks for 2 letter words formed by two longer words placed adjacent
	{
		currentSpace = get<0>(toVerify_Vector[i]);
		int new_multiplier = Store->getWordBonus(currentSpace);
		int addPoints = point_value[get<1>(toVerify_Vector[i]) - 'A'] * Store->getLetterBonus(currentSpace);
		string word;
		word.push_back(get<1>(toVerify_Vector[i]));
		currentSpace -= direction;
		while (Store->getLetter(currentSpace) != '0')
		{
			word.push_back(Store->getLetter(currentSpace));
			addPoints += point_value[Store->getLetter(currentSpace) - 'A'];
			currentSpace -= direction;
		}
		currentSpace = get<0>(toVerify_Vector[i]) + direction;
		while (Store->getLetter(currentSpace) != '0')
		{
			word.push_back(Store->getLetter(currentSpace));
			addPoints += point_value[Store->getLetter(currentSpace) - 'A'];
			currentSpace -= direction;
		}
		if (word.length() > 1)
		{
			points += (addPoints * new_multiplier);
		}
	}
	points = points * multiplier;
	if (toVerify_Vector.size() == 7)
	{
		points += 50;
	}
	return points;
}


string Scrabble::getWord(Board* Store, int direction, unsigned char x, unsigned char y)
{
	string word = "";
	int targetX = x, targetY = y;
	bool connected = firstTurn;
	while (Store->getLetter(targetX, targetY) != '0') //gets all letters to the left and top of the target tile.
	{
		if (ToVerify->getLetter(targetX, targetY) == '0')
		{
			connected = true;
		}
		word = Store->getLetter(targetX, targetY) + word;
		targetX = targetX - 1 + direction;
		targetY = targetY - direction;
	}
	targetX = x - direction + 1;
	targetY = y + direction;
	while (Store->getLetter(targetX, targetY) != '0') //gets all letters to the right and below the target tile.
	{
		if (ToVerify->getLetter(targetX, targetY) == '0')
		{
			connected = true;
		}
		word = word + Store->getLetter(targetX, targetY);
		targetX = targetX + 1 - direction;
		targetY = targetY + direction;
	}
	if (!connected)
	{
		return "";
	}
	return word;
}

bool Scrabble::VerifyBoard(int playernum)
{
	Board Store = *BoardRep;
	for (size_t i = 0; i < toVerify_Vector.size(); i++)
	{
		if (get<1>(toVerify_Vector[i]) == '[')
		{
			cout << "What letter should the blank tile be?";
			char blank;
			cin >> blank;
			blank = toupper(blank);
			Store.PlaceTile(get<0>(toVerify_Vector[i]), blank, true);
		}
		else
		{
			Store.PlaceTile(get<0>(toVerify_Vector[i]), get<1>(toVerify_Vector[i]), ToVerify->getBlank(get<0>(toVerify_Vector[i])));
		}
	}
	if (firstTurn)
	{
		if (Store.getLetter(112) == '0')
		{
			cout << "First word not placed on centre tile" << endl;
			return false;
		}
		if (toVerify_Vector.size() < 2)
		{
			cout << "First word cannot be one letter long" << endl;
			return false;
		}
	}
	bool xLine = true;
	bool yLine = true;
	for (size_t i = 0; i < toVerify_Vector.size(); i++) //this loop ensures that all the tiles fall along the same vertical or horizontal line
	{//need to check that either all X values are the same, or all Y values are the same, maximum O(2n) time, where N is the number of tiles
		if (get<0>(toVerify_Vector[i]) % 15 != get<0>(toVerify_Vector[0]) % 15)//check xs are the same
		{
			xLine = false;
		}
		if (get<0>(toVerify_Vector[i]) / 15 != get<0>(toVerify_Vector[0]) / 15)//check ys are the same
		{
			yLine = false;
		}
	}
	if (xLine == false && yLine == false) { cout << "failed due to line rule" << endl; return false; }
	
	

	for (size_t i = 0; i < toVerify_Vector.size(); i++)
	{
		vector<string> words;
		words.push_back(getWord(&Store, 0, get<0>(toVerify_Vector[i]) % 15, get<0>(toVerify_Vector[i]) / 15 )); //checks left/right
		words.push_back(getWord(&Store, 1, get<0>(toVerify_Vector[i]) % 15, get<0>(toVerify_Vector[i]) / 15));  //checks up/down
		
		if (words[0].length() == 0 && words[1].length() == 0)
		{
			cout << "Board not verified: Solo word" << endl;
			return false;
		}
		if (words[0].length() == 1 && words[1].length() == 1)
		{
			cout << "Board not verified: Solo Letter" << endl;
			return false;
		}
		for (size_t j = 0; j < words.size(); j++)
		{
			if (words[j].length() > 1)
			{
				reverse(words[j].begin(), words[j].end());
				if (!root->find(words[j]))
				{
					return false;
				}
			}
		}
	}// board has been verified, cleanup step
	
	int points = getPoints(&Store);
	Player[playernum]->AddPoints(points);
	*BoardRep = Store;
	if (firstTurn)
	{
		firstTurn = false;
	}
	for (size_t i = 0; i < toVerify_Vector.size(); i++)
	{
		BoardRep->getTile(get<0>(toVerify_Vector[i]))->setPlaced();
	}
	ToVerify->Clear();
	toVerify_Vector.clear();
	return true;
}

bool Scrabble::isRackEmpty(char* rack)
{
	for (size_t i = 0; i < 7; i++)
	{
		if (rack[i] != '0')
		{
			return false;
		}
	}
	return true;
}


void Scrabble::GenerateMoves(int playernum)
{
	cout << "Starting move generation" << endl;
	vector<char> rack_vector;
	for (size_t j = 0; j < 7; j++)
	{
		if (Player[playernum]->getRack()[j] != '0')
		{
			rack_vector.push_back(Player[playernum]->getRack()[j]);
		}
	}
	cout << "Rack Size: " << rack_vector.size() << endl;
	cout << "Rack: " << Player[playernum]->getRack() << endl;
	if (firstTurn)
	{
		for (size_t i = 0; i < rack_vector.size(); i++)
		{
			string word;
			vector<char> new_rack = rack_vector;
			char L = new_rack[i];
			new_rack.erase(new_rack.begin() + i);
			if (L == '[')
			{
				for (size_t i = 0; i < 26; i++)
				{
					L = char(i + 65);
					word.push_back(L);
					GetWordsHorizontal(new_rack, root->findChild(L), 112, 112, word, 1, 0, 2, new_rack.size());
					word.pop_back();
				}
			}
			else
			{
				word.push_back(L);
				GetWordsHorizontal(new_rack, root->findChild(L), 112, 112, word, 1, getPointValue(L, 112), 2, new_rack.size());
			}
			
			
		}
	}
	else
	{
		for (size_t i = 0; i < 225; i++)
		{
			unsigned char x = i % 15;
			unsigned char y = i / 15;
			if (BoardRep->getLetter(i) != '0')
			{
				string word;
				word.push_back(BoardRep->getLetter(i));
				if (root->findChild(BoardRep->getLetter(i)) == nullptr)
				{
					cout << "child was nullptr" << endl;
				}
				else
				{
					GetWordsVertical(rack_vector, root->findChild(BoardRep->getLetter(i)), i, i, word, 1, point_value[BoardRep->getLetter(i) - 'A'], 1 * BoardRep->getWordBonus(i), rack_vector.size());
					GetWordsHorizontal(rack_vector, root->findChild(BoardRep->getLetter(i)), i, i, word, 1, point_value[BoardRep->getLetter(i) - 'A'], 1 * BoardRep->getWordBonus(i), rack_vector.size());
				}
			}
		}
	}
	
	
	PlayBestMove(playernum);
}

bool Scrabble::GetWordsVertical(vector<char> rack, Node* node, unsigned char currentSpace, unsigned char anchor, string word, int postJoin, int points, float multiplier, int rack_size)
{
	unsigned char x = currentSpace % 15;
	unsigned char y = currentSpace / 15;
	bool isSpace = true;
	if (currentSpace > 225 || currentSpace < 0)
	{
		return false;
	}
	if ((currentSpace - (15 * postJoin)) < 0 || (currentSpace - (15 * postJoin)) > 224)
	{
		isSpace = false;
	}
	if (isSpace && BoardRep->getLetter(currentSpace - (15 * postJoin)) != '0')
	{
		char L = BoardRep->getLetter(currentSpace - (15 * postJoin));
		if (node->findChild(L) != nullptr && (currentSpace - (15 * postJoin)) % 15 == x)
		{
			GetWordsVertical(rack, node->findChild(L), currentSpace - (15 * postJoin), anchor, word + L, postJoin, (points + point_value[L - 'A']), multiplier, rack_size);
		}
		else
		{
			return false;
		}
	}
	else
	{
		for (size_t i = 0; i < rack.size(); i++)
		{
			char store = rack[i];
			vector<char> new_rack = rack;
			new_rack.erase(new_rack.begin() + i);
			if (node->findChild(store) != nullptr && store != '[' && (currentSpace - (15 * postJoin)) % 15 == x && isSpace)
			{
				GetWordsVertical(new_rack, node->findChild(store), currentSpace - (15 * postJoin), anchor, word + store, postJoin, (points + getPointValue(store, currentSpace - 15 * postJoin)), multiplier * BoardRep->getWordBonus(currentSpace - 15 * postJoin), rack_size);
			}
			else if (store == '[') //if there is a blank tile on the rack
			{
				for (size_t i = 0; i < 26; i++)
				{
					if (node->getChild(i) != nullptr && (currentSpace - (15 * postJoin)) % 15 == x && isSpace)
					{
						GetWordsVertical(new_rack, node->getChild(i), currentSpace - (15 * postJoin), anchor, word + char(i + 65) , postJoin, points, multiplier * BoardRep->getWordBonus(currentSpace - 15 * postJoin), rack_size);
					}
				}
			}

		}
		if (node->findChild('+') != nullptr)
		{ //once a + is seen, algorithm needs to reset back to anchor square and begin going to opposite direction when looking to place new tiles
			GetWordsVertical(rack, node->findChild('+'), anchor, anchor, word + '+', -1, points, multiplier, rack_size);
		}
		
	}
	if (node->getTerminal() && rack.size() < rack_size)
	{
		points = points * multiplier;
		if (rack.size() == 0)
		{
			points += 50;
		}
		if (postJoin == 1 && BoardRep->getLetter(anchor + 15) == '0')
		{
			Valid_Vertical_Words.insert(make_tuple(anchor, word, points));
		}
		else if (postJoin == -1 && BoardRep->getLetter(currentSpace + 15) == '0')
		{
			Valid_Vertical_Words.insert(make_tuple(anchor, word, points));
		}
	}
	
}

bool Scrabble::GetWordsHorizontal(vector<char> rack, Node* node, unsigned char currentSpace, unsigned char anchor, string word, int postJoin, int points, float multiplier, int rack_size)
{
	unsigned char x = currentSpace % 15;
	unsigned char y = currentSpace / 15;

	bool isSpace = true;
	if (currentSpace > 225 || currentSpace < 0)
	{
		return false;
	}
	if ((currentSpace - (1 * postJoin)) < 0 || (currentSpace - (1 * postJoin)) > 224)
	{
		isSpace = false;
	}
	if (isSpace && BoardRep->getLetter(currentSpace - (1 * postJoin)) != '0')
	{
		char L = BoardRep->getLetter(currentSpace - (1 * postJoin));
		if (node->findChild(L) != nullptr && (currentSpace - (1 * postJoin)) / 15 == y)
		{
			GetWordsHorizontal(rack, node->findChild(L), currentSpace - (1 * postJoin), anchor, word + L, postJoin, (points + point_value[L - 'A']), multiplier, rack_size);
		}
		else
		{
			return false;
		}
	}
	else
	{
		for (size_t i = 0; i < rack.size(); i++)
		{
			char store = rack[i];
			vector<char> new_rack = rack;
			new_rack.erase(new_rack.begin() + i);
			if (node->findChild(store) != nullptr && store != '[' && (currentSpace - (1 * postJoin)) / 15 == y && isSpace)
			{
				GetWordsHorizontal(new_rack, node->findChild(store), currentSpace - (1 * postJoin), anchor, word + store, postJoin, (points + getPointValue(store, currentSpace - 1 * postJoin)), multiplier * BoardRep->getWordBonus(currentSpace - 1 * postJoin), rack_size);
			}
			else if (store == '[' && isSpace) //if there is a black tile on the rack
			{
				for (size_t i = 0; i < 26; i++)
				{
					if (node->getChild(i) != nullptr && (currentSpace - (1 * postJoin)) / 15 == y)
					{
						GetWordsHorizontal(new_rack, node->getChild(i), currentSpace - (1 * postJoin), anchor, word + char(i + 65), postJoin, points, multiplier * BoardRep->getWordBonus(currentSpace - 1 * postJoin), rack_size);
					}
				}
			}

		}
		if (node->findChild('+') != nullptr)
		{ //once a + is seen, algorithm needs to reset back to anchor square and begin going to opposite direction when looking to place new tiles
			GetWordsHorizontal(rack, node->findChild('+'), anchor, anchor, word + '+', -1, points, multiplier, rack_size);
		}

	}
	if (node->getTerminal() && rack.size() < rack_size)
	{
		points = points * multiplier;
		if (rack.size() == 0)
		{
			points += 50;
		}
		if (postJoin == 1 && BoardRep->getLetter(anchor + 1) == '0')
		{
			Valid_Horizontal_Words.insert(make_tuple(anchor, word, points));
		}
		else if(postJoin == -1 && BoardRep->getLetter(currentSpace + 1) == '0')
		{
			Valid_Horizontal_Words.insert(make_tuple(anchor, word, points));
		}
	}

}

int Scrabble::getPointValue(char L, unsigned char board_index)
{
	return point_value[L - 'A'] * BoardRep->getLetterBonus(board_index) * !BoardRep->getTile(board_index)->isBlank();
}

void Scrabble::PlayBestMove(int playernum)
{
	int H_or_V = 1;
	tuple<unsigned char, string, int> highest_points = make_tuple(0, "", 0);
	for (auto i : Valid_Horizontal_Words)
	{
		if (get<2>(i) > get<2>(highest_points))
		{
			highest_points = i;
		}
	}
	for (auto i : Valid_Vertical_Words)
	{
		if (get<2>(i) > get<2>(highest_points))
		{
			highest_points = i;
			H_or_V = 15;
		}
	}
	

	string word = get<1>(highest_points);
	int square = get<0>(highest_points);
	int postjoin = 1;
	cout << "Highest point value word is: " << word << " with " << get<2>(highest_points) << " points and anchor " << square %15 << ", " << square / 15 << endl;
	for (size_t i = 0; i < word.length(); i++)
	{
		if (square < 0 || square > 225)
		{
			cout << "ERROR: SQUARE OUTSIDE BOUNDS OF ARRAY IN PLAYBESTMOVE() AT: " << square << endl;
			break;
		}
		if (word[i] == '+')
		{
			postjoin = -1;
			square = get<0>(highest_points);
			square -= H_or_V * postjoin;
		}
		else if(BoardRep->getLetter(square) == '0')
		{
			
			if (Player[playernum]->RemoveFromRack(word[i]))
			{
				ToVerify->PlaceTile(square, word[i], false);
				toVerify_Vector.push_back(make_tuple(square, word[i]));
				square -= H_or_V * postjoin;
			}
			
			else if (ToVerify->getLetter(square) == '0' && Player[playernum]->RemoveFromRack('['))
			{
				cout << "Placed a blank as " << word[i] << endl;
				ToVerify->PlaceTile(square, word[i], true);
				toVerify_Vector.push_back(make_tuple(square, word[i]));
				square -= H_or_V * postjoin;
			}
		}
		else if (BoardRep->getLetter(square) != '0')
		{
			square -= H_or_V * postjoin;
		}
	}
	if (VerifyBoard(playernum))
	{
		cout << "Rack: " << Player[playernum]->getRack() << endl;
		FillRack(playernum);
		cout << "Rack: " << Player[playernum]->getRack() << endl;
		cout << "Board Verified" << endl;
		cout << endl;
		Valid_Horizontal_Words.clear();
		Valid_Vertical_Words.clear();
		//Player[playernum]->AddPoints(get<2>(highest_points));
		if (isRackEmpty(Player[playernum]->getRack()) && TileBag->isBagEmpty())
		{
			GameOver();
		}
	}
	else
	{
		cout << "Board NOT Verified" << endl;
		if (H_or_V == 1)
		{
			Valid_Horizontal_Words.erase(highest_points);
		}
		else
		{
			Valid_Vertical_Words.erase(highest_points);
		}
		ALLBoardtoRack(Player[playernum]->getRack());
		PlayBestMove(playernum);
	}
	
}

void Scrabble::FillRack(int playernum)
{
	cout << "Filling Rack" << endl;
	for (size_t i = 0; i < 7; i++)
	{
		if (Player[playernum]->getRack()[i] == '0')
		{
			char L = TileBag->Pull();
			Player[playernum]->AddtoRack({L});
		}
	}
}

void Scrabble::DisplayBoardData()
{
	for (size_t i = 0; i < 225; i++)
	{
		if (BoardRep->getLetter(i) == '0')
		{
			cout << " ";
		}
		else
		{
			cout << BoardRep->getLetter(i);

		}
		if (i%15 == 0)
		{
			cout << endl;
		}
		
	}
}

int Scrabble::getPoints(int playernum)
{
	int output;
	return Player[playernum]->getPoints();
}

bool Scrabble::isOver()
{
	return isOver_;
}

void Scrabble::GameOver()
{
	isOver_ = true;
	for (size_t i = 0; i < 7; i++)
	{
		for (size_t j = 0; j < 2; j++)
		{
			if (Player[j]->getRack()[i] != '0')
			{
				Player[j]->setPoints(Player[j]->getPoints() - point_value[Player[j]->getRack()[i] - 'A']);
			}
		}
	}
}