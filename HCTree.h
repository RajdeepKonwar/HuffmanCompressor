/**
 @author Rajdeep Konwar (A53225609)
 
 @date Nov 10 2017
 
 @section OVERVIEW
 Declaration of class HCTree's member functions and variables.
 
 @section ASSIGNMENT NUMBER
 PA3
 */

#ifndef HCTREE_H
#define HCTREE_H

#include <vector>
#include "HCNode.h"
#include "BitInputStream.h"
#include "BitOutputStream.h"

//! A 'function class' for use as the Compare class in the HCNode priority_queue
class HCNodePtrComp {
public:
  bool operator()( HCNode* &lhs, HCNode* &rhs ) const {
    return *lhs < *rhs;
  }
};

//! Huffman Code Tree class.
class HCTree {
private:
  HCNode            *root;    //! Root of the Huffman tree
  vector< HCNode* > leaves;   //! Leaves of the tree

  //! Does a postorder traversal, deleting nodes
  static void deleteAll( HCNode *n );

public:
  //! Explicit keyword is used to avoid accidental implicit conversions
  explicit HCTree() : root(nullptr) {
    leaves = vector< HCNode* >( 256, (HCNode*) nullptr );
  }

  //! Default destructor
  ~HCTree();

  //! Build Huffman Code Tree for compressor
  void          buildTreeComp( const vector< int > &freqs, unsigned int length,
                               BitOutputStream &bout );

  //! Write header info to compressed file
  void          writeHeader( HCNode *node, BitOutputStream &bitOut );

  //! Write character count to compressed file
  void          writeCharCount( unsigned int length, BitOutputStream &bitOut );

  //! Build Huffman Code Tree for uncompressor
  unsigned int  buildTreeUncomp( BitInputStream &in );

  //! Read header info from compressed file
  void          readHeader( HCNode *par, HCNode* &node, BitInputStream &bitIn );

  //! Read character count from compressed file
  unsigned int  readCharCount( BitInputStream &bitIn );

  //! Encode symbol to compressed file
  void          encode( byte symbol, BitOutputStream &out ) const;

  //! Decode symbol from compressed file
  int           decode( BitInputStream &in ) const;
};

#endif // HCTREE_H
