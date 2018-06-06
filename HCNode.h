/**
 @author Rajdeep Konwar (A53225609)
 
 @date Nov 10 2017
 
 @section OVERVIEW
 Declaration of class HCNode's member functions and variables.
 
 @section ASSIGNMENT NUMBER
 PA3
 */

#ifndef HCNODE_H
#define HCNODE_H

#include <iostream>

typedef unsigned char byte;

using namespace std;

class HCNode {
public:
  bool    isLeaf;     //! State of node (leaf or internal)
  int     count;      //! Frequency-count of symbols
  byte    symbol;     //! Byte in the file we're keeping track of
  HCNode  *child0;    //! Pointer to '0' child (left)
  HCNode  *child1;    //! Pointer to '1' child (right)
  HCNode  *parent;    //! Pointer to parent

  //! Inline constructor
  HCNode( bool state, int count, byte symbol, HCNode *left = nullptr,
          HCNode *right = nullptr, HCNode *p = nullptr ) :
                                    isLeaf(state), count(count), symbol(symbol),
                                    child0(left), child1(right), parent(p) {}

  //! Less-than comparison, so HCNodes will work in std::priority_queue
  bool operator<( const HCNode &other );
};

//! Shut the linker up
ostream & operator<<( ostream &, const HCNode & ) __attribute__((weak));

//! For printing an HCNode to an ostream. Possibly useful for debugging.
ostream & operator<<( ostream &stm, const HCNode &n ) {
  stm << "[" << n.symbol << "," << n.count << "," << (int) (n.symbol) << "]";
  return stm;
}

#endif // HCNODE_H
