/*
* game.cpp
* COMP11 Splendor
* Abe Park    11/26/2019
*
* Purpose: Implements a Game class that reads the data file and stores 
*          information into the vector varaibles. The class then draws the
*          outline of the game, Splendor, by using the information stored in 
*          its variables and by including card.h and player.h into its system.
*/

#include "game.h"

/* ------------------------------------------------------------------------ */
/* Description: Stores information passed as arguments from the splendor.cpp
 *              and initialiezs the member variables.
 */
Game::Game(string filename, string play_nobles){
  //include nobles or not
  if(play_nobles == "true"){
    this->play_nobles = true;
  }else{
    this->play_nobles = false;
  }
  
  //initialization for the game
  initializeBoard();
  initializeGrid();
  initializeGemBank();
  initializeInPlayCards();
  
  //read data
  setupCard(filename);
}

/* ------------------------------------------------------------------------ */
/* Description: Initialiezs char board[BOARD_HEIGHT][BOARD_WIDTH]
 */
void Game::initializeBoard() {
  for(int i = 0; i < BOARD_HEIGHT; i++){
    for(int j = 0; j < BOARD_WIDTH; j++){
      board[i][j] = ' ';
      board_color[i][j] = "white";
    }
  }
}

/* ------------------------------------------------------------------------ */
/* Description: Initialiezs Card *grid[CARD_ROWS][CARD_COLS]
 */
void Game::initializeGrid() {
  for (int i = 0; i < CARD_ROWS; i++) {
    for (int j = 0; j < CARD_COLS; j++) {
      grid[i][j] = nullptr;
    }
  }
}

/* ------------------------------------------------------------------------ */
/* Description: Initialiezs int gem_bank[GEMS]
 */
void Game::initializeGemBank() {
  for (int i = 0; i < GEMS-1; i++) {
    gem_bank[i] = 4;
  }
  gem_bank[GEMS-1] = 5;
}

/* ------------------------------------------------------------------------ */
/* Description: Initialiezs inPlay_TYPE integer values
 */
void Game::initializeInPlayCards() {
  inPlay_Mines = 40;
  inPlay_Trans = 30;
  inPlay_Vendors = 20;
  inPlay_Nobles = 10;
}


/* ------------------------------------------------------------------------ */
/* Description: Reads the input data file and stores it into vector varaibles
 */
void Game::setupCard(string filename) {
  ifstream infile(filename.c_str());
  if (infile.fail()) {
    cerr << "ERROR: Error opening file, please check "
    << filename << endl;
    exit(EXIT_FAILURE);
  }
  
  //read the first line
  string card_info;
  getline(infile, card_info);
  
  //read input data
  readData(&infile);
  setupNoble();
  
  infile.close();
}

/* ------------------------------------------------------------------------ */
/* Description: Reads the data file and stores information into card class and
 *              vector variables by different categories
 */
void Game::readData(ifstream *infile_p) {
  istringstream linestream;
  string card_info, row, discountColor;
  int prestige;
  int price[GEMS-1];
  Card *curr_card = new Card();
  
  while (!(infile_p->eof())) {
    //read data into member variables
    getline(*infile_p, card_info);
    linestream.str(card_info);
    linestream >> row >> prestige >> discountColor;
    for (int i = 0; i < GEMS-1; i++) {
      linestream >> price[i];
    }
    int *price_p = price;
    //copy read data into card class
    dataToCard(curr_card, row, prestige, discountColor, price_p);
    linestream.clear();
    curr_card = new Card();
  }
  delete curr_card;
}

/* -------------------------------------------- */
/* Description: Reads the data file and stores information into card class and
 *              vector variables by different categories
 */
void Game::dataToCard(Card* curr_card, string row, int prestige, 
                      string discountColor, int *price_p) {
  curr_card->setRow(row);
  curr_card->setPrestige(prestige);
  curr_card->setDiscountColor(discountColor);
  for (int i = 0; i < GEMS-1; i++) {
    curr_card->setPrice(*(price_p + i), i);
  }
  //sort read information into different sets of vector class
  if (row == "m") {
    mines.push_back(curr_card);
  } else if (row == "t") {
    transportations.push_back(curr_card);
  } else if (row == "v") {
    vendors.push_back(curr_card);
  } else if (row == "n") {
    nobles.push_back(curr_card);
  }
}

/* ------------------------------------------------------------------------ */
/* Description: Read data into Card *noble and noble_price[i][j]
 */
void Game::setupNoble() {
  for (int i = 0; i < NUM_NOBLES; i++) {
    noble[i] = nobles.at(i);
  }
  for (int i = 0; i < NUM_NOBLES; i++) {
    for (int j = 0; j < GEMS-1; j++) {
      noble_price[i][j] = nobles.at(i)->getPrice(j);
    }
  }
}

/* ------------------------------------------------------------------------ */
/* Description: Returns a specifc string value given specific int value
*/
string Game::indexToColor(int index) {
  if (index == 0) { return "white"; }
  else if (index == 1) { return "red"; }
  else if (index == 2) { return "pink"; }
  else if (index == 3) { return "blue"; }
  else if (index == 4) { return "green"; }
  else if (index == 5) { return "yellow"; }
  else { return ""; }
}


/* ------------------------------------------------------------------------ */
/* Description: Returns a specifc int value given specific string values
*/
int Game::colorToIndex(string color) {
  if (color == "white") { return 0; }
  else if (color == "red") { return 1; }
  else if (color == "pink") { return 2; }
  else if (color == "blue") { return 3; } 
  else if (color == "green") { return 4; }
  else if (color == "gold") { return 5; }
  else { return -1 ;}
}


/* ------------------------------------------------------------------------ */
//main game loop
void Game::playGame(){
  screen_bg("black");
  screen_fg("white");
  
  bool game_over = false, player1_turn = true;
  bool victory_1 = false, victory_2 = false;
  
  string turn, command;
  bool loyalty[NUM_NOBLES] = { true, true, true };
  bool *loyalty_p = loyalty;
  while(!game_over) {
    setupBoard();

    //if a player hits 15 prestige points, end the game
    if (endGame(&game_over, victory_1, victory_2)) break;
    //check no one has more than 10 gems
    if (!checkPlayerGems()) continue;
    
    //switch players
    turn = (player1_turn ? " PLAYER 1" : " PLAYER 2");
    cout << turn << ", enter your next move: ";
    while (getline(cin, command)) {
      if(cinQuery(player1_turn, command, &game_over)) break;
    }
    //check if a player earns loyalty points and if a player has 15 points
    if (play_nobles) {
      checkLoyalty(player1_turn, loyalty_p);
    }
    checkScore(player1_turn, &victory_1, &victory_2);
    
    //change turns
    player1_turn = !player1_turn;
  }
  screen_bg("white");
  screen_fg("black");
}

/* -------------------------------------------- */
/* Description: Operates all setup functions requierd before asking for queries               
 */
void Game::setupBoard() {
  //reset the screen
  clearBoard();
  screen_clear();
  screen_home();
  
  //setupInPlayCard
  setupInPlayCard();
  
  //draw the board
  drawBoard();
}

/* ------------------------------------------------------------------------ */
/* Description: Updates Card *grid[CARD_ROWS][CARD_COLS]; decrements 
 *              inPlay_TYPE integers when a card is off the deck
 */
void Game::setupInPlayCard() {
  //Mines_inPlay
  for (int i = 0; i < CARD_COLS; i++) {
    if (grid[2][i] == nullptr) {
      if (inPlay_Mines > 0) {
        grid[2][i] = mines.at(inPlay_Mines-1);
        inPlay_Mines--;
      }
    }
  }
  //Transportations_inPlay
  for (int i = 0; i < CARD_COLS; i++) {
    if (grid[1][i] == nullptr) {
      if (inPlay_Trans > 0) {
        grid[1][i] = transportations.at(inPlay_Trans-1);
        inPlay_Trans--;
      }
    }
  }
  //Vendors_inPlay
  for (int i = 0; i < CARD_COLS; i++) {
    if (grid[0][i] == nullptr) {
      if (inPlay_Vendors > 0) {
        grid[0][i] = vendors.at(inPlay_Vendors-1);
        inPlay_Vendors--;
      }
    }
  }
}

/* ------------------------------------------------------------------------ */
/* Description: Prints statement to indicate the winner of the game;
 *              sets bool game_over to true to end the program and deallocates 
 *              memory before the program closes      
 */
bool Game::endGame(bool *game_over_p, bool victory_1, bool victory_2) {
  if (victory_1 && victory_2) {
    cout << " You losers tied - BORING!!!!" << endl;
    *game_over_p = true;
    deallocateMemory();
    return true;
  } else if (victory_2) {
    cout << " Congratulations Player 2: YOU WIN!!!!" << endl;
    *game_over_p = true;
    deallocateMemory();
    return true;
  } else if (victory_1) {
    cout << " Congratulations Player 1: YOU WIN!!!!" << endl;
    *game_over_p = true;
    deallocateMemory();
    return true;
  } else {
    return false;
  }
}

/* ------------------------------------------------------------------------ */
/* Description: get each player's current prestige points; compare the values
 *              at the end of player_2's turn; assgin true value to
 *              appropriate boolean variables to get correct result          
 */
void Game::checkScore(bool p1_turn, bool *victory_1_p, bool *victory_2_p) {
  if (!p1_turn) {
    int p1_point = player_1.totalPrestige();
    int p2_point = player_2.totalPrestige();
    if (p1_point < 15 && p2_point < 15) {
      return;
    }
    if (p2_point > p1_point) {
      *victory_2_p = true;
    } else if (p1_point > p2_point) {
      *victory_1_p = true;
    } else if (p1_point == p2_point) {
      //edge case: if two players have the same points, campare their num_cards
      int p1_cards = player_1.getNumCard();
      int p2_cards = player_2.getNumCard();
      if (p1_cards > p2_cards) {
        *victory_2_p = true;
      } else if (p2_cards > p1_cards) {
        *victory_1_p = true;
      } else if (p1_cards == p2_cards) {
        *victory_1_p = true;
        *victory_2_p = true;
      }
    }
  }
  return;
}


/* ------------------------------------------------------------------------ */
/* Description: Check if each player has more than 10 gems; if a player has
 *              more than 10 gems, ask a player to return a string value of
 *              gem color to gem_bank. Return true player's gem is under 10
 */
bool Game::checkPlayerGems() {
  if (!gemLimit(player_1)) {
    string gem = returnGemQuery(1);
    returnGem(&player_1, colorToIndex(gem));
    return false;
  } else if (!gemLimit(player_2)) {
    string gem = returnGemQuery(2);
    returnGem(&player_2, colorToIndex(gem));
    return false;
  } else {
    return true;
  }
}

/* ------------------------------------------------------------------------ */
/* Description: Return true if player's total gems are more than 10
 */
bool Game::gemLimit(Player player) {
  int sum = 0;
  for (int i = 0; i < GEMS; i++) {
    sum += player.getGemCount(i);
  }
  if (sum > 10) {
    return false;
  } else {
    return true;
  }
}

/* ------------------------------------------------------------------------ */
/* Description: Ask a player to return a gem until he provides a valid string
 *              value; return the string value
 */
string Game::returnGemQuery(int player_i) {
  if (player_i == 1) {
    cout << " PLAYER 1, you have more than 10 gems!"
         << " Name one gem you would like to return: ";
  } else {
    cout << " PLAYER 2, you have more than 10 gems!"
         << " Name one gem you would like to return: ";
  }
  string gem;
  getline(cin, gem);
  while (colorToIndex(gem) == -1) {
    cout << " Invalid color. Name one gem you would like to return: ";
    getline(cin, gem);
  }
  return gem;
}

/* ------------------------------------------------------------------------ */
/* Description: Update player's current gem counts and gem_bank[]
 */
void Game::returnGem(Player *player_p, int index) {
  player_p->setGemCount(-1, index);
  setGemBank(1, index);
}


/* ------------------------------------------------------------------------ */
/* Description: Check if a player meets conditions to earn loyalty;
 *              check only if nobles are still valid (in the board)
 */
void Game::checkLoyalty(bool p1_turn, bool *loyalty_p) {
  if (p1_turn) {
    if (*(loyalty_p+0)) {
      earnLoyalty(loyalty_p, &player_1, 0);
    } 
    if (*(loyalty_p+1)) {
      earnLoyalty(loyalty_p, &player_1, 1);
    } 
    if (*(loyalty_p+2)) {
      earnLoyalty(loyalty_p, &player_1, 2);
    }
  } else {
    if (*(loyalty_p+0)) {
      earnLoyalty(loyalty_p, &player_2, 0);
    } 
    if (*(loyalty_p+1)) {
      earnLoyalty(loyalty_p, &player_2, 1);
    } 
    if (*(loyalty_p+2)) {
      earnLoyalty(loyalty_p, &player_2, 2);
    }
  }
}

/* ------------------------------------------------------------------------ */
/* Description: Compare a player's current gem discount to that of the noble;
 *              if conditions are met, update the player's prestige points and
 *              erase the noble off the board (set noble to nullptr)
 */
void Game::earnLoyalty(bool *loyalty_p, Player *player_p, int order) {
  bool flag = true;
  for (int i = 0; i < GEMS-1; i++) {
    if (noble_price[order][i] == 0) continue;
    if (noble_price[order][i] > player_p->getDiscount(i)) {
      flag = false;
      break;
    }
  }
  if (flag) {
    *(loyalty_p+order) = false;
    player_p->setTotalPrestige(3);
    noble[order] = nullptr;
  }
}

/* ------------------------------------------------------------------------ */
/* Description: Receives a string value from player; returns true if the player 
 *              input is valid, else false
 */
bool Game::cinQuery(bool p1_turn, string command, bool *game_over_p) {
  istringstream linestream;
  string query;
  linestream.str(command);
  linestream >> query;
  if (query != "p2" && query != "p3" && query != "r" && query != "b" &&
      query != "br" && query != "q") {
    linestream.clear();
    cout << " Invalid move - enter a valid move: ";
    return false;
  }
  if (gameQuery(&linestream, p1_turn, query, game_over_p)) return true;
  else return false;
}

/* ------------------------------------------------------------------------ */
/* Description: Call specific function given specfic string values; returns 
 *              true if the player has given valid input values, therefore, 
 *              desired function is called, else false
 */
bool Game::gameQuery(istringstream *linestream_p, bool p1_turn, string query, 
                     bool *game_over_p) {
  if (query == "p2") {
    return call_p2(linestream_p, p1_turn);
  }
  else if (query == "p3") {
    return call_p3(linestream_p, p1_turn);
  }
  else if (query == "r") {
    return call_reserve(linestream_p, p1_turn);
  }
  else if (query == "b") {
    return call_b(linestream_p, p1_turn);
  }
  else if (query == "br") {
    return call_br(linestream_p, p1_turn);
  }
  else if (query == "q") {
    deallocateMemory();
    *game_over_p = true;
    return true;
  }
  else {
    return false;
  }
}

/* ------------------------------------------------------------------------ */
/* Description: Deletes all memories in the heap, stored in vector class
 */
void Game::deallocateMemory() {
  for (int i = 0; i < mines.size(); i++) {
    delete mines.at(i);
  }
  for (int i = 0; i < transportations.size(); i++) {
    delete transportations.at(i);
  }
  for (int i = 0; i < vendors.size(); i++) {
    delete vendors.at(i);
  }
  for (int i = 0; i < nobles.size(); i++) {
    delete nobles.at(i);
  }
}

/* ------------------------------------------------------------------------ */
/* Description: Receives a string value of gem color from the player, transform
 *              it into specifc integer value (g1_i) to access gem_bank[g1_i];
 *              returns true if player gets two gems, else false
 */
bool Game::call_p2(istringstream *linestream_p, bool p1_turn) {
  string g1;
  *linestream_p >> g1;
  linestream_p->clear();
  int g1_i = colorToIndex(g1);
  
  //int value '-1' indicates that given input string is not a valid color
  if (g1_i == -1) {
    cout << " Invalid move (invalid color) - enter a valid move: ";
    return false;
  } 
  //splendor rule: takes 2 gems of same color only when there are 4 gems left
  if (gem_bank[g1_i] != 4) {
    cout << " Invalid move (unavailable gems) - enter a valid move: ";
    return false;
  }

  do_p2(p1_turn, g1_i);
  return true;
}

/* ------------------------------------------------------------------------ */
/* Description: Decrements gem_bank by 2 and gives 2 gems of specific color to
 *              the player
 */
void Game::do_p2(bool p1_turn, int g1_i) {
  gem_bank[g1_i] -= 2;
  if (p1_turn) {
    player_1.setGemCount(2, g1_i);
  } else {
    player_2.setGemCount(2, g1_i);
  }
}


/* ------------------------------------------------------------------------ */
/* Description: Receives three string value of three different colors from 
 *              the player, transform it into specifc integer values to access
 *              three different gem_banks[];
 *              returns true if the player gets three gems, else false
 */
bool Game::call_p3(istringstream *linestream_p, bool p1_turn) {
  string g1 = "s1", g2 = "s2", g3 = "s3";
  *linestream_p >> g1 >> g2 >> g3;
  linestream_p->clear();
  if (g1 == "s1" || g2 == "s2" || g3 == "s3") {
    cout << " Invalid move (too few colors) - enter a valid move: ";
    return false;
  }
  
  int g1_i = colorToIndex(g1);
  int g2_i = colorToIndex(g2);
  int g3_i = colorToIndex(g3);
  //int value '-1' indicates that given input string is not a valid color
  if (g1_i == -1 || g2_i == -1 || g3_i == -1) {
    cout << " Invalid move (invalid color) - enter a valid move: ";
    return false;
  } 
  
  if (g1 == g2 || g2 == g3 || g1 == g3) {
    cout << " Invalid move (duplicate color) - enter a valid move: ";
    return false;
  }

  //before accessing gem_bank[i], check if there is enough gems left
  if (!checkBank(g1_i) || !checkBank(g2_i) || !checkBank(g3_i)) {
    cout << " Invalid move (unavailable gems) - enter a valid move: ";
    return false;
  }
  
  do_p3(p1_turn, g1_i, g2_i, g3_i);
  return true;
}

/* ------------------------------------------------------------------------ */
/* Description: Returns true if there is any gem in the requested gem_bank[i]
 */
bool Game::checkBank(int index) {
  if (gem_bank[index] > 0) {
    return true;
  } else {
    return false;
  }
}

/* ------------------------------------------------------------------------ */
/* Description: Decrements one from three called gem_bank[i] and give one gem
 *              from three different colors to the player
 */
void Game::do_p3(bool p1_turn, int g1_i, int g2_i, int g3_i) {
  gem_bank[g1_i]--;
  gem_bank[g2_i]--;
  gem_bank[g3_i]--;
  if (p1_turn) {
    player_1.setGemCount(1, g1_i);
    player_1.setGemCount(1, g2_i);
    player_1.setGemCount(1, g3_i);
  } else {
    player_2.setGemCount(1, g1_i);
    player_2.setGemCount(1, g2_i);
    player_2.setGemCount(1, g3_i);
  }
  return;
}


/* ------------------------------------------------------------------------ */
/* Description: Receives a string value of type and an integer value 
 *              that locates the desired card to reserve; returns true if 
 *              the player reserves a desired card, else false
 */
bool Game::call_reserve(istringstream *linestream_p, bool p1_turn) {
  string type;
  int index;
  *linestream_p >> type >> index;
  linestream_p->clear();
  
  //error check
  if (!querySanity(type, index)) {
    return false;
  }
  if (p1_turn) {
    //reserve card for player_1 && error check
    if (!do_reserve(&player_1, type, index)) {
      return false;
    }
  } else {
    //reserve card for player_2 && error check
    if (!do_reserve(&player_2, type, index)) {
      return false;
    }
  }
  return true;
}

/* ------------------------------------------------------------------------ */
/* Description: Evaluates player input and returns true if all input values are
 *              valid to be used in function, else false
 */
bool Game::querySanity(string type, int index) {
  if (type != "m" && type != "t" && type != "v") {
    cout << " Invalid move (invalid card type) - enter a valid move: ";
    return false;
  } else if (index < 1 || index > 4) {
    cout << " Invalid move (invalid card column) - enter a valid move: ";
    return false;
  } else
  return true;
}


/* ------------------------------------------------------------------------ */
/* Description: Returns true if the player reserves a card, else false
 */
bool Game::do_reserve(Player *player_p, string type, int index) {
  if (!checkReserve(*player_p)) {
    cout << " Invalid move (reserve is already full) - ";
    cout << "enter a valid move: ";
    return false;
  }
  //gives one "gold" gem when reserving a card, only when there is any
  if (gem_bank[GEMS-1] > 0) {
    gem_bank[GEMS-1] --;
    player_p->setGemCount(1, GEMS-1);
  }
  
  if (type == "m") {
    player_p->setReserve(grid[2][index-1]);
    grid[2][index-1] = nullptr;
  } else if (type == "t") {
    player_p->setReserve(grid[1][index-1]);
    grid[1][index-1] = nullptr;
  } else if (type == "v") {
    player_p->setReserve(grid[0][index-1]);
    grid[0][index-1] = nullptr;
  }
  return true;
}

/* ------------------------------------------------------------------------ */
/* Description: Returns true if there is room to reserve a card, else false
 */
bool Game::checkReserve(Player player) {
  for (int i = 0; i < CARD_ROWS; i++) {
    if (player.getReserve(i) == nullptr) {
      return true;
    }
  }
  return false;
}


/* ------------------------------------------------------------------------ */
/* Description: Returns true if the function is called, else false
 */
bool Game::call_b(istringstream *linestream_p, bool p1_turn) {
  string type;
  int index;
  *linestream_p >> type >> index;
  linestream_p->clear();
  
  //error check
  if (!querySanity(type, index)) {
    return false;
  }
  if (p1_turn) {
    //buy card for player_1 && error check
    if (!do_b(&player_1, type, index)) {
      return false;
    }
  } else {
    //buy card for player_2 && error check
    if (!do_b(&player_2, type, index)) {
      return false;
    }
  }
  return true;
}

/* ------------------------------------------------------------------------ */
/* Description: Returns true if player buys a card and updates player's status
 *              , else false
 */
bool Game::do_b(Player *player_p, string type, int index) {
  Card *curr_card = grid[typeToIndex(type)][index-1];
  if (curr_card == nullptr) {
    cout << " Invalid move (no card in that column) - ";
    cout << "enter a valid move: ";
    return false;
  }
  if (!comparePrice(*player_p, curr_card)) {
    cout << " Invalid move (cannot afford card) - ";
    cout << "enter a valid move: ";
    return false;
  }
  //updates player's gemcount[] and gem_bank[]
  changeGems(player_p, curr_card);
  
  //updates player's color discount value
  player_p->setDiscount(colorToIndex(curr_card->getDiscountColor()));
  
  //updates player's prestige points
  player_p->setTotalPrestige(curr_card->getPrestige());
  
  //updates player's number of cards bought
  player_p->setNumCard();
  
  //erase the bought card off the board
  grid[typeToIndex(type)][index-1] = nullptr;
  
  return true;
}

/* ------------------------------------------------------------------------ */
/* Description: Returns a specific int value given a specfic string value 
 *              in order to access grid[][]
 */
int Game::typeToIndex(string type) {
  if (type == "m") { return 2; }
  else if (type == "t") { return 1; }
  else if (type == "v") { return 0; }
  else { return -1; }
}

/* ------------------------------------------------------------------------ */
/* Description: Returns true if player can buy the desired card with/without
 *              using his joker money ("gold" gem), else false
 */
bool Game::comparePrice(Player player, Card *curr_card) {
  int joker = player.getGemCount(GEMS-1);
  for (int i = 0; i < GEMS-1; i++) {
    int pocket = player.getGemCount(i);
    int price = curr_card->getPrice(i) - player.getDiscount(i);
    //continue loop if player can pay for the price without using "gold" gem
    if (pocket >= price) {
      continue;
    } else if (pocket + joker >= price) {
    //continue loop if player can pay for the price using "gold" gem
      joker -= (price - pocket);
      continue;
    } else {
      return false;
    }
  }
  return true;
}

/* ------------------------------------------------------------------------ */
/* Description: Updates gems in possesion of a given player and the discount
 *              value; updates gem_bank in accordance with the player's change
 */
void Game::changeGems(Player *player_p, Card *curr_card) {
  for (int i = 0; i < GEMS-1; i++) {
    int joker = player_p->getGemCount(GEMS-1);
    int pocket = player_p->getGemCount(i);
    //to prevent having negative number to be compared and added to gem_bank[]
    int price = curr_card->getPrice(i) - player_p->getDiscount(i);
    if (price < 0) { price = 0; }
    
    //continue loop if player can pay for the price without using "gold" gem
    if (pocket >= price) {
      player_p->setGemCount(-price, i);
      setGemBank(price, i);
      continue;
    } else if (pocket + joker >= price) {
    //continue loop if player can pay for the price using "gold" gem
      price -= pocket;
      player_p->setGemCount(-pocket, i);
      player_p->setGemCount(-price, GEMS-1);
      setGemBank(pocket, i);
      setGemBank(price, GEMS-1);
      continue;
    }
  }
}

/* ------------------------------------------------------------------------ */
/* Description: setter; adds given "new_gem" value to gem_bank[i]
 */
void Game::setGemBank(int new_gems, int i) {
  gem_bank[i] += new_gems;
}


/* ------------------------------------------------------------------------ */
/* Description: Returns true if a function is called, else false
 */
bool Game::call_br(istringstream *linestream_p, bool p1_turn) {
  int index;
  *linestream_p >> index;
  linestream_p->clear();
  if (p1_turn) {
    if (do_br(&player_1, index)) {
      //buy reserved card for player_1 && error check
      return true;
    } else {
      return false;
    }
  } else {
    if (do_br(&player_2, index)) {
      //buy reserved card for player_2 && error check
      return true;
    } else {
      return false;
    }
  }
}

/* ------------------------------------------------------------------------ */
/* Description: Returns true if player buys a card and updates player's status
 *              , else false
 */
bool Game::do_br(Player *player_p, int index) {
  Card *curr_card = player_p->getReserve(index-1);
  if (index < 1 || index > 3 || curr_card == nullptr) {
    cout << " Invalid move (no card at that index) - ";
    cout << "enter a valid move: ";
    return false;
  }
  if (!comparePrice(*player_p, curr_card)) {
    cout << " Invalid move (cannot afford card) - ";
    cout << "enter a valid move: ";
    return false;
  }
  //updates player's gemcount[] and gem_bank[]
  changeGems(player_p, curr_card);
  
  //updates player's color discount value
  player_p->setDiscount(colorToIndex(curr_card->getDiscountColor()));
  
  //updates player's prestige points
  player_p->setTotalPrestige(curr_card->getPrestige());
  
  //erase the bought card off the board
  player_p->switchReserve(index-1);
  
  //updates player's number of cards bought
  player_p->setNumCard();
  
  return true;
}


/* ------------------------------------------------------------------------ */
//draw all of the elements onto the game board
void Game::drawBoard(){
  int vpos = 0;
  int hpos = 1;
  
  //draw the card decks
  if(inPlay_Vendors > 0){
    drawDeck(vpos, hpos, "Vendors");
  }
  vpos += CARD_HEIGHT + 1;
  if(inPlay_Trans > 0){
    drawDeck(vpos, hpos, "Transport");
  }
  vpos += CARD_HEIGHT + 1;
  if(inPlay_Mines > 0){
    drawDeck(vpos, hpos, "Mines");
  }
  
  //draw the in play cards
  vpos = 0;
  hpos = CARD_WIDTH + 2;
  for(int i = 0; i < CARD_ROWS; i++){
    for(int j = 0; j < CARD_COLS; j++){
      if(grid[i][j] != nullptr){
        drawCard(vpos, hpos, grid[i][j]);
      }
      hpos += CARD_WIDTH + 1;
    }
    vpos += CARD_HEIGHT + 1;
    hpos = CARD_WIDTH + 2;
  }
  
  //draw the gems
  int gemOffset = ((CARD_WIDTH + 1) * 5) + 3;
  vpos = 1;
  hpos = gemOffset;
  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 2; j++){
      int index = (i * 2) + j;
      
      if(gem_bank[index] > 0){
        board[vpos][hpos + 5] = gem_bank[index] + 48;
        board_color[vpos][hpos + 5] = indexToColor(index);
        drawGem(vpos + 1, hpos, indexToColor(index));
      }
      hpos += CARD_WIDTH;
    }
    vpos += CARD_HEIGHT + 1;
    hpos = gemOffset;
  }
  
  //draw players
  drawPlayer(vpos + 1, 1, &player_1);
  drawPlayer(vpos + 1, ((CARD_WIDTH + 1) * 4), &player_2);
  
  //draw nobles
  if(play_nobles){
    hpos = ((CARD_WIDTH + 1) * 7) + 2;
    vpos = 2;
    for(int i = 0; i < NUM_NOBLES; i++){
      if(noble[i] != nullptr){
        drawNoble(vpos, hpos, noble[i]);
      }
      vpos += CARD_HEIGHT + 1;
    }
  }
  
  //print out the finished board
  for(int i = 0; i < BOARD_HEIGHT; i++){
    for(int j = 0; j < BOARD_WIDTH; j++){
      if(board_color[i][j] != "white"){
        screen_fg(board_color[i][j]);
      }
      cout << board[i][j];
      screen_fg("white");
    }
    cout << endl;
  }
  cout << endl;
}


/* ------------------------------------------------------------------------ */
//draws a noble at the specified position of the board
void Game::drawNoble(int vpos, int hpos, Card *c){
  string top    = "|\\  /*\\  /|";
  string second = "| \\/   \\/ |";
  string blank  = "N         N";
  string bottom = "~~~~~~~~~~~";
  
  //draw the basic card outline
  int length = top.length();
  for(int i = 0; i < length; i++){
    board[vpos][hpos + i] = top[i];
    board[vpos + 1][hpos + i] = second[i];
    
    for(int j = 2; j < 5; j++){
      board[vpos + j][hpos + i] = blank[i];
    }
    board[vpos + 5][hpos + i] = bottom[i];
  }
  
  //draw the prices
  vpos += 2;
  for(int i = 0; i < GEMS-1; i++){
    int price = c->getPrice(i);
    if(price > 0){
      board_color[vpos][hpos + 4] = indexToColor(i);
      board_color[vpos][hpos + 5] = indexToColor(i);
      board_color[vpos][hpos + 6] = indexToColor(i);
      board[vpos][hpos + 4] = '|';
      board[vpos][hpos + 5] = price + 48;
      board[vpos][hpos + 6] = '|';
      vpos++;
    }
  }
}


/* ------------------------------------------------------------------------ */
//draws a player at the specified position of the board
void Game::drawPlayer(int vpos, int hpos, Player *p){
  //player string
  string prestige = to_string(p->totalPrestige());
  string player_s;
  if(hpos < 10){
    player_s = " PLAYER 1: " + prestige + " prestige point(s)";
  }else{
    player_s = " PLAYER 2: " + prestige + " prestige point(s)";
  }
  int length = player_s.length();
  for(int i = 0; i < length; i++){
    board[vpos][hpos + i] = player_s[i];
  }
  
  //discount string
  vpos++;
  string bonus_s = "Discounts: ";
  length = bonus_s.length();
  for(int i = 0; i < GEMS-1; i++){
    int bonus = p->getDiscount(i);
    bonus_s += "|" + to_string(bonus) + "| ";
    
    board_color[vpos][hpos + length] = indexToColor(i);
    board_color[vpos][hpos + length + 1] = indexToColor(i);
    board_color[vpos][hpos + length + 2] = indexToColor(i);
    
    length += 4;
    if(bonus >= 10){
      length += 1;
    }
  }
  for(int i = 0; i < length; i++){
    board[vpos][hpos + i] = bonus_s[i];
  }
  
  //gem string
  vpos++;
  string gems_s = "     Gems: ";
  length = gems_s.length();
  for(int i = 0; i < GEMS; i++){
    int gems = p->getGemCount(i);
    gems_s += "(" + to_string(gems) + ") ";
    
    board_color[vpos][hpos + length] = indexToColor(i);
    board_color[vpos][hpos + length + 1] = indexToColor(i);
    board_color[vpos][hpos + length + 2] = indexToColor(i);
    
    length += 4;
  }
  for(int i = 0; i < length; i++){
    board[vpos][hpos + i] = gems_s[i];
  }
  
  //reserve string
  vpos++;
  string reserve_s = " Reserved:";
  length = reserve_s.length();
  for(int i = 0; i < length; i++){
    board[vpos][hpos + i] = reserve_s[i];
  }
  
  hpos += reserve_s.length();
  for(int i = 0; i < 3; i++){
    Card *c = p->getReserve(i);
    if(c == nullptr){
      drawDeck(vpos, hpos, "");
    }else{
      drawCard(vpos, hpos, c);
    }
    hpos += CARD_WIDTH + 1;
  }
}


/* ------------------------------------------------------------------------ */
//draws a card deck at the specified position of the board
void Game::drawDeck(int vpos, int hpos, string type){
  string top = "  _________  ";
  string second = " /         \\ ";
  string blank = "|           |";
  string bottom = " \\_________/ ";
  
  //draw the basic card outline
  int length = top.length();
  for(int i = 0; i < length; i++){
    board[vpos][hpos + i] = top[i];
    board[vpos + 1][hpos + i] = second[i];
    for(int j = 2; j < CARD_HEIGHT; j++){
      board[vpos + j][hpos + i] = blank[i];
    }
    board[vpos + CARD_HEIGHT][hpos + i] = bottom[i];
  }
  
  int start_pos = hpos + (blank.length() / 2) - (type.length() / 2);
  length = type.length();
  for(int i = 0; i < length; i++){
    board[vpos + (CARD_HEIGHT/2)][start_pos + i] = type[i];
  }
}


/* ------------------------------------------------------------------------ */
//draws a gem at the specified position of the board
void Game::drawGem(int vpos, int hpos, string color){
  string gem_image[6];
  gem_image[0] = "  _______  ";
  gem_image[1] = ".'_/_|_\\_'.";
  gem_image[2] = "\\`\\  |  /`/";
  gem_image[3] = " `\\  | //' ";
  gem_image[4] = "   `\\|/`   ";
  gem_image[5] = "     `     ";
  
  for(int i = 0; i < GEMS; i++){
    int length = gem_image[i].length();
    for(int j = 0; j < length; j++){
      board[vpos + i][hpos + j] = gem_image[i][j];
      board_color[vpos + i][hpos + j] = color;
    }
  }
}


/* ------------------------------------------------------------------------ */
//draws a card at the specified position of the board
void Game::drawCard(int vpos, int hpos, Card *c){
  string top = "  _________  ";
  string second = " /         \\ ";
  string blank = "|           |";
  string bottom = " \\_________/ ";
  
  //draw the basic card outline
  int length = top.length();
  for(int i = 0; i < length; i++){
    board[vpos][hpos + i] = top[i];
    
    if(i == 8 || i == 9){
      board_color[vpos + 1][hpos + i] = c->getDiscountColor();
    }
    board[vpos + 1][hpos + i] = second[i];
    
    for(int j = 2; j < CARD_HEIGHT; j++){
      board[vpos + j][hpos + i] = blank[i];
    }
    board[vpos + CARD_HEIGHT][hpos + i] = bottom[i];
  }
  
  //draw the prestige number and gem
  board[vpos + 2][hpos + 3] = c->getPrestige() + 48;
  board_color[vpos + 2][hpos + 8] = c->getDiscountColor();
  board_color[vpos + 2][hpos + 9] = c->getDiscountColor();
  board[vpos + 2][hpos + 8] = '\\';
  board[vpos + 2][hpos + 9] = '/';
  board_color[vpos + 1][hpos + 8] = c->getDiscountColor();
  board_color[vpos + 1][hpos + 9] = c->getDiscountColor();
  board[vpos + 1][hpos + 8] = '_';
  board[vpos + 1][hpos + 9] = '_';
  
  //draw the prices
  vpos += 4;
  for(int i = 0; i < GEMS-1; i++){
    int price = c->getPrice(i);
    if(price > 0){
      board_color[vpos][hpos + 2] = indexToColor(i);
      board_color[vpos][hpos + 3] = indexToColor(i);
      board_color[vpos][hpos + 4] = indexToColor(i);
      board[vpos][hpos + 2] = '(';
      board[vpos][hpos + 3] = price + 48;
      board[vpos][hpos + 4] = ')';
      vpos++;
    }
  }
  
}


/* ------------------------------------------------------------------------ */
//clear the board display
void Game::clearBoard(){
  for(int i = 0; i < BOARD_HEIGHT; i++){
    for(int j = 0; j < BOARD_WIDTH; j++){
      board[i][j] = ' ';
      board_color[i][j] = "white";
    }
  }
}
