/**
 @author Rajdeep Konwar (A53225609)
 
 @date Nov 10 2017
 
 @section OVERVIEW
 Declaration of class BitOutputStream's member functions.
 
 @section ASSIGNMENT NUMBER
 PA3
 */

#ifndef BITOUTPUTSTREAM_H
#define BITOUTPUTSTREAM_H

#include <iostream>

using namespace std;

class BitOutputStream {
private:
  unsigned char buff;   //! 1-byte buffer storing 8 bits
  int           nbits;  //! No. of bits that have been written to the buffer
  ostream       &out;   //! Reference to the bytewise output stream

public:
  //! Initialize a BitOutputStream that will use the given ostream for output
  BitOutputStream( ostream &os ) : buff(0), nbits(0), out(os) {}

  //! Perform padding on the last byte with zeroes
  void completeByte();

  //! Send the buffer to the output, and clear it
  void flush();

  /** Write the least significant bit of the argument to
   *  the bit buffer, and increment the bit buffer index.
   *  But flush the buffer first, if it is full.
   */
  void writeBit( unsigned int bit );
};

#endif //! BITOUTPUTSTREAM_H
