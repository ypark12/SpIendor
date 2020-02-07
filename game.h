/*
 * game.h
 * COMP11 Splendor
 * Abe Park    11/26/2019
 *
 * Define the Game class type
 *
 */

#ifndef GAME_H
#define GAME_H
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "termfuncs.h"
#include "card.h"
#include "player.h"
using namespace std;

class Game {
public:
  //constructor
  Game(string filename, string play_nobles);

  //main gameplay
  void playGame();

private:
  //pre_game functions
  void initializeBoard();
  void initializeGrid();
  void initializeGemBank();
  void initializeInPlayCards();
  void setupCard(string filename);
  void readData(ifstream *infile_p);
  void dataToCard(Card* curr_card, string row, int prestige, 
                  string discountColor, int *price_p);
  void setupNoble();

  //in_game functions
  void setupBoard();
  void setupInPlayCard();
  bool checkBank(int index);
  bool checkReserve(Player player);
  int typeToIndex(string type);
  int colorToIndex(string color);
  bool querySanity(string type, int index);
  void setGemBank(int new_gems, int i);
  void deallocateMemory();
  
  //endGame
  bool endGame(bool *game_over_p, bool victory_1, bool victory_2);
  void checkScore(bool p1_turn, bool *victory_1_p, bool *victory_2_p);
  
  //loyalty
  void checkLoyalty(bool p1_turn, bool *loyalty_p);
  void earnLoyalty(bool *loyalty_p, Player *player_p, int order);
  
  //query
  bool cinQuery(bool p1_turn, string command, bool *game_over_p);
  bool gameQuery(istringstream *linestream_p, bool p1_turn, string query, 
                 bool *game_over_p);
  
  //'p2' functions
  bool call_p2(istringstream *linestream, bool p1_turn);
  void do_p2(bool p1_turn, int g1_i);
  
  //'p3' functions
  bool call_p3(istringstream *linestream_p, bool p1_turn);
  void do_p3(bool p1_turn, int g1_i, int g2_i, int g3_i);
  
  //'r' functions
  bool call_reserve(istringstream *linestream_p, bool p1_turn);
  bool do_reserve(Player *player_p, string type, int index);
  
  //'b' functions
  bool call_b(istringstream *linestream_p, bool p1_turn);
  bool do_b(Player *player_p, string type, int index);
  
  //'br' functions
  bool call_br(istringstream *linestream_p, bool p1_turn);
  bool do_br(Player *player_p, int index);
  
  //buying gems
  bool comparePrice(Player player, Card *curr_card);
  void changeGems(Player *player_p, Card *curr_card);
  
  //returning gems
  bool gemLimit(Player player);
  void returnGem(Player *player_p, int index);
  string returnGemQuery(int player_i);
  bool checkPlayerGems();
  
  //drawing functions
  void drawBoard();
  void drawNoble(int vpos, int hpos, Card *c);
  void drawPlayer(int vpos, int hpos, Player *p);
  void drawDeck(int vpos, int hpos, string type);
  void drawCard(int vpos, int hpos, Card *c);
  void drawGem(int vpos, int hpos, string color);
  void clearBoard();
  string indexToColor(int index);

  static const int BOARD_HEIGHT = 41;
  static const int BOARD_WIDTH = 120;
  static const int CARD_WIDTH = 13;
  static const int CARD_HEIGHT = 8;
  static const int CARD_ROWS = 3;
  static const int CARD_COLS = 4;
  static const int NUM_NOBLES = 3;
  static const int GEMS = 6;

  //nobles activated true/false
  bool play_nobles;

  //display
  char board[BOARD_HEIGHT][BOARD_WIDTH];
  string board_color[BOARD_HEIGHT][BOARD_WIDTH];

  //cards in play
  Card *grid[CARD_ROWS][CARD_COLS];
  Card *noble[NUM_NOBLES];
  
  int noble_price[NUM_NOBLES][GEMS-1];
  
  int inPlay_Mines;
  int inPlay_Trans;
  int inPlay_Vendors;
  int inPlay_Nobles;
  
  int gem_bank[GEMS];

  //deck vectors
  Vector nobles;
  Vector vendors;
  Vector transportations;
  Vector mines;
  
  //two players
  Player player_1;
  Player player_2;
};

#endif
