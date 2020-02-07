/* vector.h
 *
 * Richard Townsend   11/6/19
 *
 * Define the Vector class type and the type name that will be used to refer to
 * the Data in a Vector (using a typedef).
 *
 */
#include <iostream>
#include <string>
#include "card.h"
using namespace std;

typedef Card *Data;

class Vector {
public:
  Vector();
  ~Vector();
  int size();
  bool empty();
  void push_back(Data value);
  void pop_back();
  Data front();
  Data back();
  Data at(int index);
  void insert(int index, Data value);
  void erase(int index);
  void setSize(int index);
private:
  int vCapacity; //Number of locations in the vector
  int vSize;     //Number of elements stored in the vector
  Data *vBuffer;  //Pointer to the vector's dynamically allocated array
  void reserve(int n);
};
