/*
 * card.cpp
 * COMP11 Splendor
 * Abe Park    11/26/2019
 *
 * Purpose: Implements a Card class that stores information from the data file
 *          and returns output given the user's query.
 *
 */

#include "card.h"

/* ------------------------------------------------------------------------ */
/* Name: Card (constructor)
 * Arguments: string row, int prestige, string discountColor, int price[C_GEMS]
 * Description: Stores information passed as arguments from the game.cpp into
 *              each individual class.
 */
Card::Card() {
  row = "";
  prestige = 0;
  discountColor = "";
  for (int i = 0; i < C_GEMS; i++) {
    price[i] = 0;
  };
}


/* ------------------------------------------------------------------------ */
/* Name: string getRow()
 * Arguments: n/a
 * Description: getter; returns a 'row' string when called
 */
string Card::getRow() {
  return row;
}


/* ------------------------------------------------------------------------ */
/* Name: int getPrestige()
 * Arguments: n/a
 * Description: getter; returns a 'prestige' int when called
 */
int Card::getPrestige() {
  return prestige;
}


/* ------------------------------------------------------------------------ */
/* Name: string getDiscountColor()
 * Arguments: n/a
 * Description: getter; returns a 'discountColor'string when called
 */
string Card::getDiscountColor() {
  return discountColor;
}


/* ------------------------------------------------------------------------ */
/* Name: int getPrice()
 * Arguments: int i
 * Description: getter; returns a 'price[i]' int when called
 */
int Card::getPrice(int index) {
  return price[index];
}


/* ------------------------------------------------------------------------ */
/* Name: void setRow()
 * Arguments: string row
 * Description: setter; set row to a given string value "row"
 */
void Card::setRow(string row) {
  this->row = row;
}


/* ------------------------------------------------------------------------ */
/* Name: void setPrestige()
 * Arguments: int prestige
 * Description: setter; set prestige to a given int value "price"
 */
void Card::setPrestige(int prestige) {
  this->prestige = prestige;
}


/* ------------------------------------------------------------------------ */
/* Name: void setDiscountColor()
 * Arguments: string discountColor
 * Description: setter; set discountColor to a given string "discountColor"
 */
void Card::setDiscountColor(string discountColor) {
  this->discountColor = discountColor;
}


/* ------------------------------------------------------------------------ */
/* Name: void setPrice()
 * Arguments: int price, int index
 * Description: setter; set price[index] to a given int value "price"
 */
void Card::setPrice(int price, int index) {
  this->price[index] = price;
}