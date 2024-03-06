#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "Bag.h"
#include "Consts.h"
#include "GADDAG.h"
using namespace std;
using namespace sf;



void PlaceTile(char letter, int x, int y, char (*arr)[BOARD_SIZE], Sprite** TileSprite, sf::RenderWindow& window)
{
    int tile = int(letter);
    tile = tile - 65;
    arr[x][y] = letter;
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

void EraseTile(int x, int y, char(*toVerify)[BOARD_SIZE], Sprite board, Sprite** TileSprite, sf::RenderWindow& window) 
//will only erase tiles that are not verified, so only send toVerify array
{
    if (toVerify[x][y] != '0')
    {
        TileSprite[27]->setPosition(5 + (73 * x), 5 + (73 * y)); //27 blank tile, creates see-through sprite
        window.draw(*TileSprite[27]);
        toVerify[x][y] = '0';
    }
}

void ReplaceTile(int x, int y,char* tiles ,char(*toVerify)[BOARD_SIZE]) //takes x/y coordinate of tile to be moved back to the rack
{
    if (toVerify[x][y] != '0')
    {
        for (size_t i = 0; i < 7; i++)
        {
            if (tiles[i] == '0')
            {
                tiles[i] = toVerify[x][y];
                toVerify[x][y] = '0';
            }
        }
    }
}

bool VerifyBoard(char(*BoardRep)[BOARD_SIZE], char(*toVerify)[BOARD_SIZE])
{
    int Verify[7][2]; //stores the x and y position of tiles that need to be verified
    int start = 0;
    string word = "";
    for (size_t i = 0; i < 7; i++)
    {
        for (size_t j = 0; j < 2; j++)
        {
            Verify[i][j] = -1;
        }
    }
    char Store[BOARD_SIZE][BOARD_SIZE];
    for (size_t i = 0; i < BOARD_SIZE; i++) //merge BoardRep and toVerify into one board, Store
    {
        for (size_t j = 0; j < BOARD_SIZE; j++)
        {
            Store[i][j] = BoardRep[i][j];
            if (toVerify[i][j] != '0')
            {
                Store[i][j] = toVerify[i][j];
                Verify[start][0] = i;
                Verify[start][1] = j;
                start++;
            }
        }
    }
    for (size_t i = 0; i < 7; i++)
    {
        int targetX = Verify[i][0], targetY = Verify[i][1];
        while (Store[targetX][targetY] != '0') //gets all letters to the left of the target tile.
        {
            word = Store[targetX][targetY] + word;
            targetX--;
        }
        targetX = Verify[i][0];
        while (Store[targetX][targetY] != '0') //gets all letters to the right of the target tile.
        {
            word = Store[targetX][targetY] + word;
            targetX++;
        }

    }


    delete[] Store;
    return true;
}

bool VerifyWord(string word) //perform binary search to match given word against the dictionary of words.
{

}

int main()
{

    char* tiles = new char[7];
    Bag bag;
    tiles = bag.Pull(7);
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

    char BoardRep[BOARD_SIZE][BOARD_SIZE]; //representation of board in char array form
    char toVerify[BOARD_SIZE][BOARD_SIZE]; //array to place tiles on before they are verified, is later copied into the BoardRep 
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
                x = (x + 5) / TILE_SIZE;
                y = (y + 5) / TILE_SIZE;
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    
                    if (x == 16 && y <= 7 && tiles[y] != '0') //select from the rack
                    {
                        toPlace = y;
                        cout << toPlace << endl;
                    }
                    else if (toPlace != -1 && x <= 15 && y <= 15 && BoardRep[x][y] == '0') //place from rack to the board
                    {
                        PlaceTile(tiles[toPlace], x, y, toVerify, TileSprite, window);
                        tiles[toPlace] = '0';
                        toPlace = -1;
                    }
                   
                }
                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    ReplaceTile(x, y, tiles, toVerify);
                }
            }
        }
        

        window.clear();
        window.draw(background);
        DrawBoard(BoardRep, toVerify, board, TileSprite, window);
        DrawRack(tiles, TileSprite, window);
        
        window.display();
    }
    
    return 0;
}

//TODO BEFORE IMPLEMENTING COMPUTER PLAYER
//Replace function in Bag.cpp
//verify moves
//button to call verify
//button to call replace
//

//If you want to export as .exe you'll need to add static dependencies :) 