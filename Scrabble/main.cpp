#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "Bag.h"
#include "Consts.h"
#include "GADDAG.h"
using namespace std;
using namespace sf;



void PlaceTile(char letter, int x, int y, char (*toVerify)[BOARD_SIZE], Sprite** TileSprite, sf::RenderWindow& window) //takes a tile, adds it to the 
//toVerify array, then draws the tile on the board
{//returns false if tile could not be placed
    int tile = int(letter);
    tile = tile - 65;
    toVerify[x][y] = letter;
    TileSprite[tile]->setPosition(5 + (73 * x), 5 + (73 * y));
    window.draw(*TileSprite[tile]);
}



void DrawBoard(char(*BoardRep)[BOARD_SIZE], char(*toVerify)[BOARD_SIZE], Sprite board, Sprite** TileSprite, sf::RenderWindow& window)
{
    window.draw(board);
    int tile;
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        for (size_t j = 0; j < BOARD_SIZE; j++)
        {
            if (BoardRep[i][j] != '0')
            {
                tile = int(BoardRep[i][j]);
                tile -= 65;
                TileSprite[tile]->setPosition(5 + (73 * i), 5 + (73 * j));
                window.draw(*TileSprite[tile]);
            }
            else if (toVerify[i][j] != '0')
            {
                tile = int(toVerify[i][j]);
                tile -= 65;
                TileSprite[tile]->setPosition(5 + (73 * i), 5 + (73 * j));
                window.draw(*TileSprite[tile]);
            }
        }
    }
}

void DrawRack(char* rack, Sprite** TileSprite, sf::RenderWindow& window)
{
    int x;
    for (size_t i = 0; i < 7; i++)
    {
        if (rack[i] != '0')
        {
            x = int(rack[i]) - 65;
            
            TileSprite[x]->setPosition(TILE_SIZE*16, (i * TILE_SIZE)+5);
            window.draw(*TileSprite[x]);
        }
        else
        {
            TileSprite[26]->setPosition(TILE_SIZE * 16, (i * TILE_SIZE) + 5);
            window.draw(*TileSprite[26]);
        }
    }
}

/*void EraseTile(int x, int y, char(*toVerify)[BOARD_SIZE], Sprite board, Sprite** TileSprite, sf::RenderWindow& window)
//will only erase tiles that are not verified, so only send toVerify array
{
    if (toVerify[x][y] != '0')
    {
        TileSprite[27]->setPosition(5 + (73 * x), 5 + (73 * y)); //27 blank tile, creates see-through sprite
        window.draw(*TileSprite[27]);
        toVerify[x][y] = '0';
    }
}*/ 

// ^ should not be necessary as the board is redrawn every frame, and any tile that is not set will be set as empty.

void ReplaceTile(int x, int y,char* Rack,char(*toVerify)[BOARD_SIZE], vector<tuple<int, int, char>> toVerify_Vector) //takes x/y coordinate of tile to be moved back to the rack
{
    if (toVerify[x][y] != '0')
    {
        for (size_t i = 0; i < RACK_SIZE; i++)
        {
            if (Rack[i] == '0')
            {
                Rack[i] = toVerify[x][y];
                toVerify[x][y] = '0';
            }
        }
    }
    
}

string getWord(char** Store, int direction, int x, int y) //direction is up/down or left/right, 1 for up/down 0 for left/right
{
    string word = "";
    int targetX = x, targetY = y;
    while (Store[targetX][targetY] != '0') //gets all letters to the left and top of the target tile.
    {
        word = Store[targetX][targetY] + word;
        targetX = targetX - 1 + direction;
        targetY = targetY - direction;
    }
    targetX = x - direction + 1;
    targetY = y + direction;
    while (Store[targetX][targetY] != '0') //gets all letters to the right and below the target tile.
    {
        word = word + Store[targetX][targetY];
        targetX = targetX + 1 - direction;
        targetY = targetY + direction;
    }

    return word;
}

void DisplayBoard(char(*Board)[BOARD_SIZE], vector<tuple<int, int, char>> toVerify_Vector, string message)
{
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        for (size_t j = 0; j < BOARD_SIZE; j++)
        {
            cout << Board[j][i] << ", ";
        }
        cout << endl;
    }
    for (size_t i = 0; i < toVerify_Vector.size(); i++)
    {
        cout << get<0>(toVerify_Vector[i]) << ", " << get<1>(toVerify_Vector[i]) << ": " << get<2>(toVerify_Vector[i]) << endl;
    }
    cout << message << endl;
}

bool VerifyBoard(char(*BoardRep)[BOARD_SIZE], char(*toVerify)[BOARD_SIZE],vector<tuple<int, int, char>> toVerify_Vector,  Node* root) //NEEDS TO BE OPTIMISED
{
    /*
    int** Verify = new int* [7]; //stores the x and y position of tiles that need to be verified
    for (size_t i = 0; i < 7; i++)
    {
        Verify[i] = new int[2];
        for (size_t j = 0; j < 2; j++)
        {
            Verify[i][j] = -1;
        }
    }*/

    int start = 0;
    char** Store = new char*[BOARD_SIZE];
    //char Store[BOARD_SIZE][BOARD_SIZE];
    for (size_t i = 0; i < BOARD_SIZE; i++) //creates Store, a copy of BoardRep, consider changing to std::copy if possible in future.
    {
        Store[i] = new char[BOARD_SIZE];
        for (size_t j = 0; j < BOARD_SIZE; j++)
        {
            Store[i][j] = BoardRep[i][j];
        }
    }
    for (size_t i = 0; i < toVerify_Vector.size(); i++)
    {
        Store[get<0>(toVerify_Vector[i])][get<1>(toVerify_Vector[i])] = get<2>(toVerify_Vector[i]); // cop
    }

    bool xLine = true;
    bool yLine = true;
    for (size_t i = 0; i < toVerify_Vector.size(); i++) //this loop ensures that all the tiles fall along the same vertical or horizontal line
    {//need to check that either all X values are the same, or all Y values are the same, maximum O(2n) time, where N is the number of tiles
        if (get<0>(toVerify_Vector[i]) != get<0>(toVerify_Vector[0]))//check xs are the same
        {
            xLine = false;
        }
        if (get<1>(toVerify_Vector[i]) != get<1>(toVerify_Vector[0]))//check ys are the same
        {
            yLine = false;
        }
    }
    if (xLine == false && yLine == false)
    {
        return false;
    }

    for (size_t i = 0; i < toVerify_Vector.size(); i++)
    {
       
            /*string word = "";
            int targetX = Verify[i][0], targetY = Verify[i][1];
            while (Store[targetX][targetY] != '0') //gets all letters to the left of the target tile.
            {
                word = Store[targetX][targetY] + word;
                targetX--;
            }
            targetX = Verify[i][0] + 1;
            while (Store[targetX][targetY] != '0') //gets all letters to the right of the target tile.
            {
                word = word + Store[targetX][targetY];
                targetX++;
            }
            */
            vector<string> words;
            //words.push_back(getWord(Store, 0, Verify[i][0], Verify[i][1])); //checks left/right
           // words.push_back(getWord(Store, 1, Verify[i][0], Verify[i][1])); //checks up/down

            words.push_back(getWord(Store, 0, get<0>(toVerify_Vector[i]), get<1>(toVerify_Vector[i]))); //checks left/right
            words.push_back(getWord(Store, 1, get<0>(toVerify_Vector[i]), get<1>(toVerify_Vector[i])));  //checks up/down
            for (size_t j = 0; j < words.size(); j++)
            {
                if (words[j].length() > 1)
                {
                    cout << "word to verify is: " << words[j] << endl;
                    reverse(words[j].begin(), words[j].end());
                    cout << "reversed word is: " << words[j] << endl;
                    if (!find(root, words[j]))
                    {
                        return false;
                    }
                }
            }
            
            
        
        
    }// board has been verified, cleanup step
    for (size_t i = 0; i < toVerify_Vector.size(); i++)
    {
        BoardRep[get<0>(toVerify_Vector[i])][get<1>(toVerify_Vector[i])] = get<2>(toVerify_Vector[i]);
    }

    cout << "TEST 1" << endl;
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        delete[] Store[i];
        for (size_t j = 0; j < BOARD_SIZE; j++)
        {
            toVerify[i][j] = '0';
            
        }
    }
    delete[] Store;
    return true;
}



int main()
{

    char* Rack = new char[7];
    Bag bag;
    Rack = bag.Pull(7);
    
    Rack[0] = 'C';
    Rack[1] = 'A';
    Rack[2] = 'T';
    Rack[3] = 'S';
    Rack[4] = 'B';
    Rack[5] = 'A';
    Rack[6] = 'G';

    
    int toPlace = -1;

    cout << "Starting GADDAG generation..." << endl;
    Node* root = GenerateGADDAG("GADDAG.txt");
    cout << "Finished GADDAG generation" << endl;

    //if (!verifyGADDAG(root, "GADDAG.txt")) { cout << "Invalid GADDAG" << endl; }
    //else { cout << "Valid GADDAG" << endl; }

   
    RenderWindow window(VideoMode(1304, 1304), "Scrabble!");
    Texture boardTexture;
    boardTexture.loadFromFile("Board.jpg");
    string path;
    Texture** TileTexture;
    TileTexture = (Texture**)malloc(27 * sizeof(Texture));
    Sprite** TileSprite;
    TileSprite = (Sprite**)malloc(27 * sizeof(Sprite));
    for (size_t i = 0; i < 27; i++) // create and load
    {
        path = "Tiles\\";
        char cast = i + 65;
        path += cast;
        path += ".png";
        TileTexture[i] = new Texture();
        path += TileTexture[i]->loadFromFile(path);
        TileSprite[i] = new Sprite();
        TileSprite[i]->setTexture(*TileTexture[i]);
        TileSprite[i]->setScale(0.75, 0.75);
    }

    boardTexture.setSmooth(true);
    sf::Sprite board;
    board.setTexture(boardTexture);
    board.setScale(1.5, 1.5);
    sf::RectangleShape background;
    background.setSize(sf::Vector2f(1304, 1304));
    

    Texture ButtonTexture;
    ButtonTexture.loadFromFile("Submit.png");
    sf::Sprite submit_button;
    submit_button.setTexture(ButtonTexture);
    submit_button.setScale(1, 1);
    submit_button.setPosition(900, 1150);


    char BoardRep[BOARD_SIZE][BOARD_SIZE]; //representation of board in char array form
    char toVerify[BOARD_SIZE][BOARD_SIZE]; //array to place tiles on before they are verified, is later copied into the BoardRep 
    //the array is good for when you want to check if a specific coordiant has a tile waiting to be verified
    //vector is good to get all the tiles you want to verify, without having to search through the whole array (225 entries)
    //it's more bookkeeping, but makes the VerifyBoard function MUCH more efficient, useful as it has to be run a lot to verify all the potential bot moves.
    vector<tuple<int, int, char>> toVerify_Vector; // X, Y, Letter value.
    //array once it's verified the tiles form legal words
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        for (size_t j = 0; j < BOARD_SIZE; j++)
        {
            BoardRep[i][j] = '0';
            toVerify[i][j] = '0';
        }
    }


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event)) // events go in here.
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                int x = Mouse::getPosition(window).x;
                int y = Mouse::getPosition(window).y;
                int Tilex = (x + 5) / TILE_SIZE;
                int Tiley = (y + 5) / TILE_SIZE;
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    
                    if (Tilex == 16 && Tiley <= 7 && Rack[Tiley] != '0') //select from the rack
                    {
                        toPlace = Tiley;
                        cout << toPlace << endl;
                    }
                    else if (toPlace != -1 && Tilex <= 15 && Tiley <= 15 && BoardRep[Tilex][Tiley] == '0' && toVerify[Tilex][Tiley] == '0') //place from rack to the board
                    {
                        PlaceTile(Rack[toPlace], Tilex, Tiley, toVerify, TileSprite, window);
                        toVerify_Vector.push_back(make_tuple(Tilex, Tiley, Rack[toPlace]));
                        Rack[toPlace] = '0';
                        toPlace = -1;
                    }
                    else if (x > 900 && x < 1200 && y > 1150 && y < 1250) //if submit button is clicked
                    {
                        if (VerifyBoard(BoardRep, toVerify, toVerify_Vector, root))
                        {
                            cout << "Board verified" << endl;
                            toVerify_Vector.clear();
                        }
                        else { cout << "Board not verified" << endl; }
                    }
                    
                   
                }
                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    ReplaceTile(Tilex, Tiley, Rack, toVerify, toVerify_Vector);
                    for (size_t i = 0; i < toVerify_Vector.size(); i++)
                    {
                        if (Tilex == get<0>(toVerify_Vector[i]) && Tiley == get<1>(toVerify_Vector[i]))
                        {
                            toVerify_Vector.erase(toVerify_Vector.begin() + i);
                            cout << i << endl;
                        }
                    }
                }
            }
        }
        

        window.clear();
        window.draw(background);
        DrawBoard(BoardRep, toVerify, board, TileSprite, window);
        DrawRack(Rack, TileSprite, window);
        window.draw(submit_button);
       

        window.display();
    }
    
    delete[] TileSprite;
    delete[] TileTexture;
    delete[] Rack;
    return 0;
}

//TODO 

//Start implementing computer player
//begin with move generation, once this is done  

//
//VARIABLES

//TileTexture - an array of tile textures, textures are the pixel pattern, when combined with the shape (e.g, rectangle) they become a sprite
//TileSprite - array of tile sprites

//STRUCTURE:

//tiles that have been placed and verified to form valid words are stored in the 2d array BoardRep.
//tiles that are placed but not verified are in the toVerify 2d array.

//If you want to export as .exe you'll need to add static dependencies :) 