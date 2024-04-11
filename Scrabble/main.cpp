#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
//#include "Bag.h"
//#include "Board.h"
#include "Consts.h"
//#include "GADDAG.h"
#include "Scrabble.h"
//#include "ComputerPlayer.h"
using namespace std;
using namespace sf;

void DrawBoard(Board* board, vector<tuple<unsigned char, char>> toVerify, Sprite boardSprite, Sprite** TileSprite, sf::RenderWindow& window)
{
    window.draw(boardSprite);
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        for (size_t j = 0; j < BOARD_SIZE; j++)
        {
            char letter = board->getLetter(i, j);
            if (letter != '0')
            {
                int tile = int(letter) - 65;
                TileSprite[tile]->setPosition(5 + (73 * i), 5 + (73 * j));
                window.draw(*TileSprite[tile]);
            }
        }
    }
    
    for (size_t i = 0; i < toVerify.size(); i++)
    {
        unsigned char x = get<0>(toVerify[i]) % 15;
        unsigned char y = get<0>(toVerify[i]) / 15;
        int tile = int(get<1>(toVerify[i])) - 65;
        TileSprite[tile]->setPosition(5 + (73 * x), 5 + (73 * y));
        window.draw(*TileSprite[tile]);
    }
}

void DrawRack(char* rack, Sprite** TileSprite, sf::RenderWindow& window)
{
    int x;
    for (size_t i = 0; i < 7; i++)
    {
        //cout << "drawing: " << rack[i] << endl;
        if (rack[i] != '0')
        {
            x = int(rack[i]) - 65;

            TileSprite[x]->setPosition(TILE_SIZE * 16, (i * TILE_SIZE) + 5);
            window.draw(*TileSprite[x]);
        }
        else
        {
            TileSprite[26]->setPosition(TILE_SIZE * 16, (i * TILE_SIZE) + 5);
            window.draw(*TileSprite[26]);
        }
    }
}




void DisplayBoard(char(*Board)[BOARD_SIZE], vector<tuple<int, int, char>> toVerify_Vector, string message) //displays the content of the board as text in the console.
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




int main()
{
    Scrabble Game(false, true, "Bag.txt");
    int toPlace = -1;
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


    Texture SubmitButtonTexture;
    SubmitButtonTexture.loadFromFile("Submit.png");
    sf::Sprite submit_button;
    submit_button.setTexture(SubmitButtonTexture);
    submit_button.setScale(1, 1);
    submit_button.setPosition(900, 1150);

    Texture GenerateButtonTexture;
    GenerateButtonTexture.loadFromFile("Generate.png");
    sf::Sprite generate_button;
    generate_button.setTexture(GenerateButtonTexture);
    generate_button.setScale(1, 1);
    generate_button.setPosition(600, 1150);


    //char BoardRep[BOARD_SIZE][BOARD_SIZE]; //representation of board in char array form
    //char toVerify[BOARD_SIZE][BOARD_SIZE]; //array to place tiles on before they are verified, is later copied into the BoardRep 
    //the array is good for when you want to check if a specific coordiant has a tile waiting to be verified
    //vector is good to get all the tiles you want to verify, without having to search through the whole array (225 entries)
    //it's more bookkeeping, but makes the VerifyBoard function MUCH more efficient, useful as it has to be run a lot to verify all the potential bot moves.
    //array once it's verified the tiles form legal words
    /*for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        for (size_t j = 0; j < BOARD_SIZE; j++)
        {
            BoardRep[i][j] = '0';
            toVerify[i][j] = '0';
        }
    }*/

    while (window.isOpen())
    {
        if (!Game.getAgent(0)->getComputer())
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

                        if (Tilex == 16 && Tiley <= 7 && Game.getPlayerRack(0)[Tiley] != '0') //select from the rack
                        {
                            toPlace = Tiley;
                            cout << toPlace << endl;
                        }
                        else if (toPlace != -1 && Tilex <= 15 && Tiley <= 15 && Game.isEmpty(Tilex, Tiley)) //place from rack to the board
                        {
                            unsigned char index = Tiley * 15 + Tilex;
                            Game.RacktoBoard(Game.getAgent(0), toPlace, (Tiley * 15 + Tilex));
                            //PlaceTile(Rack[toPlace], Tilex, Tiley, toVerify, TileSprite, window);
                            toPlace = -1;
                        }
                        else if (x > 900 && x < 1200 && y > 1150 && y < 1250) //if submit button is clicked
                        {
                            if (Game.VerifyBoard())
                            {
                                cout << "Board verified" << endl;
                            }
                            else { cout << "Board not verified" << endl; }
                        }


                    }
                    if (event.mouseButton.button == sf::Mouse::Right)
                    {
                        Game.BoardtoRack((Tiley * 15 + Tilex), Game.getAgent(0)->getRack());
                    }
                }
            }
        }




        window.clear();
        window.draw(background);


        DrawBoard(Game.getBoardRep(), Game.getToVerify_Vector(), board, TileSprite, window);
        //DrawBoard(BoardRep, toVerify, board, TileSprite, window);
        DrawRack(Game.getAgent(0)->getRack(), TileSprite, window);
        window.draw(submit_button);
        window.draw(generate_button);


        window.display();
        
        
            
    }

    delete[] TileSprite;
    delete[] TileTexture;
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