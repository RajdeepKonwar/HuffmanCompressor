/**
 @author Rajdeep Konwar (A53225609)
 
 @date Nov 10 2017
 
 @section OVERVIEW
 Implementation of class HCNode's member functions.
 
 @section ASSIGNMENT NUMBER
 PA3
 */

#include "HCNode.h"

/** Input params: Reference to the other HCNode
 *  Return param: Boolean
 *  Description : Operator (less than) for insertion into priority queue.
 *
 *  Compare this HCNode and other for priority ordering. Smaller count means
 *  higher priority. Use node symbol for deterministic tiebreaking.
 */
bool HCNode::operator<( const HCNode &other ) {
  //! If counts are different, higher count has higher priority in queue
  if( count != other.count )
    return (count > other.count);

  /** Counts are equal. use symbol value to break tie.
   *  Higher ASCII value symbol has higher priority in queue.
   */
  return (symbol < other.symbol);
}
