/**
 @author Rajdeep Konwar (A53225609)
 
 @date Nov 10 2017
 
 @section OVERVIEW
 Declaration of class BitInputStream's member functions.
 
 @section ASSIGNMENT NUMBER
 PA3
 */

#ifndef BITINPUTSTREAM_H
#define BITINPUTSTREAM_H

#include <iostream>

using namespace std;

class BitInputStream {
private:
  unsigned char buff;   //! 1-byte buffer storing 8 bits
  int           nbits;  //! No. of bits that have been read from the buffer
  istream       &in;    //! Reference to the bytewise input stream

public:
  //! Initialize a BitInputStream that will use the given istream for input
  BitInputStream( istream &is ) : buff(0), nbits(8), in(is) {}

  //! Fill the bitwise buffer by reading one byte from the input stream
  void fill();

  //! Read 1 bit from the bitwise buffer
  unsigned int readBit();
};

#endif // BITINPUTSTREAM_H
