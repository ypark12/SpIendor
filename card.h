/*
 * card.h
 * COMP11 Splendor
 * Abe Park    11/26/2019
 *
 * Define the Card class type
 *
 */

#ifndef CARD_H
#define CARD_H
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Card {
public:
  //constructor
  Card();

  //getters and setters
  string getRow();
  int getPrestige();
  string getDiscountColor();
  int getPrice(int index);
  
  void setRow(string row);
  void setPrestige(int prestige);
  void setDiscountColor(string discountColor);
  void setPrice(int price, int index);
  
private:
  static const int C_GEMS = 5;

  string row;             //String that determines the card's row(category)
  int prestige;           //Number that determines the card's numeric value
  string discountColor;   //String that determines the card's color
  int price[C_GEMS];      //Number array that determines the card's price
};
#endif
