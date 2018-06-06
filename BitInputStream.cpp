/**
 @author Rajdeep Konwar (A53225609)
 
 @date Nov 10 2017
 
 @section OVERVIEW
 Implementation of class BitInputStream's member functions.
 
 @section ASSIGNMENT NUMBER
 PA3
 */

#include "HCTree.h"
#include "BitInputStream.h"

/** Input params: None
 *  Return param: None
 *  Description : Fill the buffer with a new byte from istream.
 *
 *  Get a byte from istream to bitwise buffer and set no. of bits to zero
 */
void BitInputStream::fill() {
  buff  = in.get();   //! Read one byte from istream to bitwise buffer

  //! Handle end of file
  if( in.eof() ) {
    nbits = -1;       //! Special return case for eof
    return;
  }

  nbits = 0;          //! No bits have been read from bitwise buffer
}

/** Input params: None
 *  Return param: unsigned int bit
 *  Description : Read the next bit from the buffer.
 */
unsigned int BitInputStream::readBit() {
  //! Fill bitwise buffer if there are no more unread bits
  if( nbits == 8 )
    fill();

  //! Special case to return when hit eof
  if( nbits == -1 )
    return 2;

  //! Get the next unread bit (least significant bit!) from the bitwise buffer
  unsigned int nextBit = ((buff & (1 << nbits)) >> nbits);

  //! Increment the number of bits read from the bitwise buffer
  nbits++;

  //! Return the bit just read
  return nextBit;
}
