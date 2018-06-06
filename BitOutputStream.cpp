/**
 @author Rajdeep Konwar (A53225609)
 
 @date Nov 10 2017
 
 @section OVERVIEW
 Implementation of class BitOutputStream's member functions.
 
 @section ASSIGNMENT NUMBER
 PA3
 */

#include "BitOutputStream.h"

/** Input params: None
 *  Return param: None
 *  Description : Completes the last byte (bit overflow) by padding with zeros.
 */
void BitOutputStream::completeByte() {
  if( nbits )
    for( int i = nbits; i <= 8; i++ )
      writeBit( 0 );  //! Pad with zero
}

/** Input params: None
 *  Return param: None
 *  Description : Send the bitwise buffer to the output stream,
 *                and clear the bitwise buffer.
 */
void BitOutputStream::flush() {
  out.put( buff );  //! Write the bitwise buffer to the ostream
  buff  = 0;        //! Clear the bitwise buffer
  nbits = 0;        //! Bitwise buffer is cleared, so there are 0 bits in it
}

/** Input params: unsigned int bit
 *  Return param: None
 *  Description : Write bit to the buffer.
 *
 *  Write the least significant bit of the argument to the bit buffer, and
 *  increment the bit buffer index. But flush the buffer first, if it is full.
 */
void BitOutputStream::writeBit( unsigned int bit ) {
  //! Flush the bitwise buffer if it is full
  if( nbits == 8 )
    flush();

  //! Set the next open bit of the bitwise buffer to 'bit'
  buff = (buff & ~(1 << nbits)) | (bit << nbits);

  //! increment the number of bits in our bitwise buffer
  nbits++;
}
