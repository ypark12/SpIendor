/*
 * player.cpp
 * COMP11 Splendor
 * Abe Park    11/26/2019
 *
 * Purpose: Implements a Player class that stores game data. The member
 *          variables in the class are updated every round according to the
 *          users' plays.
 *
 */

#include "player.h"

/* ------------------------------------------------------------------------ */
/* Name: Player (constructor)
 * Arguments: n/a
 * Description: Initializes the class member variables
 */
Player::Player() {
  //initialization
  for (int i = 0; i < P_GEMS; i++) {
    discount_gems[i] = 0;
  }

  for (int i = 0; i < P_GEMS+1; i++) {
    current_gems[i] = 0;
  }

  for (int i = 0; i < NUM_R_CARD; i++) {
    reserve[i] = nullptr;
  }

  prestige = 0;
  num_card = 0;
}

/* ------------------------------------------------------------------------ */
/* Name: int getNumCard()
 * Arguments:
 * Description: getter; returns a 'num_card' int when called
 */
int Player::getNumCard() {
  return num_card;
}


/* ------------------------------------------------------------------------ */
/* Name: int getDiscount()
 * Arguments: int i
 * Description: getter; returns a 'discount_gems[i]' int when called
 */
int Player::getDiscount(int index) {
  return discount_gems[index];
}


/* ------------------------------------------------------------------------ */
/* Name: int getGemCount()
 * Arguments: int i
 * Description: getter; returns a 'current_gems[i]' int when called
 */
int Player::getGemCount(int index) {
  return current_gems[index];
}


/* ------------------------------------------------------------------------ */
/* Name: Card *getReserve()
 * Arguments: int i
 * Description: getter; returns a 'reserve[i]' Card * when called
 */
Card *Player::getReserve(int index) {
  return reserve[index];
}


/* ------------------------------------------------------------------------ */
/* Name: int totalPrestige()
 * Arguments: n/a
 * Description: getter; returns a 'prestige' int when called
 */
int Player::totalPrestige() {
  return prestige;
}

/* ------------------------------------------------------------------------ */
/* Name: void setNumCard()
 * Arguments:
 * Description: setter; increments 'num_card' by 1 when called
 */
void Player::setNumCard() {
  num_card++;
}

/* ------------------------------------------------------------------------ */
/* Name: void setDiscount()
 * Arguments: int i
 * Description: setter; increments 'discount_gems[i]' by 1 when called
 */
void Player::setDiscount(int index) {
  discount_gems[index]++;
}


/* ------------------------------------------------------------------------ */
/* Name: void setGemCount()
 * Arguments: int new_gems, int i
 * Description: setter; increments 'current_gems[i]' by new_gems when called
 */
void Player::setGemCount(int new_gems, int index) {
  current_gems[index] += new_gems;
}


/* ------------------------------------------------------------------------ */
/* Name: void setReserve()
 * Arguments: Card *c
 * Description: setter; updates reserve[i] by a passed Card *c when called
 */
void Player::setReserve(Card *c) {
  for (int i = 0; i < NUM_R_CARD; i++) {
    if (reserve[i] == nullptr) {
      reserve[i] = c;
      break;
    }
  }
}


/* ------------------------------------------------------------------------ */
/* Name: void switchReserve() 
 * Arguments: int index
 * Description: moves the reserved card to left if space is empty
 */
void Player::switchReserve(int index) {
  reserve[index] = nullptr;
  for (int i = 0; i < NUM_R_CARD-1; i++) {
    if (reserve[i] == nullptr) {
      reserve[i] = reserve[i+1];
      reserve[i+1] = nullptr;
    }
  }
}


/* ------------------------------------------------------------------------ */
/* Name: void setTotalPrestige()
 * Arguments: int new_prestige
 * Description: setter; increments 'prestige' by new_prestige when called
 */
void Player::setTotalPrestige(int new_prestige) {
  prestige += new_prestige;
}
