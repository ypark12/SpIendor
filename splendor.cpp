/*
 * splendor.cpp
 * COMP11 Splendor
 * Abe Park    11/26/2019
 *
 * Desgined to initiate the Splendor program.
 *
 */

#include <iostream>
#include <string>
#include "game.h"
using namespace std;

int main(int argc, char *argv[]) {
  if(argc < 3){
    cerr << "ERROR: Expecting a data filename and an indicator";
    cerr << " of whether to play with nobles." << endl;
    exit(EXIT_FAILURE);
  }

  Game my_game(argv[1], argv[2]);
  my_game.playGame();

  return 0;
}
