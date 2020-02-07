/*
 * player.h
 * COMP11 Splendor
 * Abe Park    11/26/2019
 *
 * Define the Player class type
 *
 */

#ifndef PLAYER_H
#define PLAYER_H
#include <iostream>
#include <fstream>
#include <string>
#include "vector_full.h"
#include "card.h"
using namespace std;

class Player {
public:
  //constructor
  Player();

  //getters
  int getNumCard();
  int totalPrestige();
  int getPrestige();
  int getDiscount(int index);
  int getGemCount(int index);
  Card *getReserve(int index);

  //setters
  void setNumCard();
  void setDiscount(int index);
  void setGemCount(int new_gems, int index);
  void setReserve(Card *c);
  void setTotalPrestige(int new_prestige);
  
  void switchReserve(int index);
private:
  static const int P_GEMS = 5;
  static const int NUM_R_CARD = 3;

  //member variables
  int discount_gems[P_GEMS];  // Num array storing the player's purchased cards
  int current_gems[P_GEMS+1]; // Num array storing the player's current gems
  Card *reserve[3];           // Card *array storing the player's reserved card
  int prestige;               // Number that shows the player's current points
  int num_card;
};
#endif
