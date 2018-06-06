/**
 @author Rajdeep Konwar (A53225609)
 
 @date Nov 10 2017
 
 @section OVERVIEW
 Implementation of class HCTree's member functions and variables.
 
 @section ASSIGNMENT NUMBER
 PA3
 */

#include <algorithm>    //! reverse
#include <math.h>       //! pow
#include <string>       //! string
#include <queue>        //! priority_queue
#include "HCTree.h"

/** Input params: Vector of freqs, input file length, ref. to bit output stream
 *  Return param: None
 *  Description : Use the Huffman algorithm to build a Huffman coding trie.
 */
void HCTree::buildTreeComp( const vector< int > &freqs, unsigned int length,
                            BitOutputStream &bout ) {
  HCNode *left, *right, *top;
  priority_queue< HCNode*, vector< HCNode* >, HCNodePtrComp > priorQ;

  //! Populate priority queue and leaves vector only with leaf nodes having freq
  for( int i = 0; i < 256; i++ ) {
    if( freqs[i] ) {
      HCNode *p = new HCNode( true, freqs[i], (byte) i );
      priorQ.push( p );
      leaves[i] = p;
    }
  }

  //! Loop till priority queue (forest) has only one single tree remaining
  while( priorQ.size() && priorQ.size() != 1 ) {
    //! Get the top element of priority queue and remove it from queue
    left  = priorQ.top();
    priorQ.pop();

    //! Get the next top element of priority queue and remove it from queue
    right = priorQ.top();
    priorQ.pop();

    //! Create an internal node with sum of the above two's freqs as its freq
    top           = new HCNode( false, (left->count + right->count), 0 );

    //! Establish pointer associations
    top->child0   = left;
    top->child1   = right;
    left->parent  = top;
    right->parent = top;

    //! Push the newly created internal node to priority queue
    priorQ.push( top );
  }

  //! Handle empty files by keeping root at nullptr
  if( priorQ.size() )
    root = priorQ.top();

  //! Write header information to compressed file first
  writeHeader( root, bout );

  //! Write character count to compressed file next
  writeCharCount( length, bout );
}

/** Input params: HCNode pointer, reference to bit output stream
 *  Return param: None
 *  Description : Write header information to the compressed file first.
 */
void HCTree::writeHeader( HCNode *node, BitOutputStream &bitOut ) {
  if( node == nullptr )
    return;

  //! Leaf nodes: write 1 followed by 8-bit byte symbol
  if( node->isLeaf ) {
    bitOut.writeBit( 1 );

    //! Output 8-bit byte symbol stored at leaf node
    for( int i = 0; i < 8; i++ )
      bitOut.writeBit( (node->symbol & (1 << i)) >> i );
  }
  //! Internal nodes: write 0 followed by left then right subtrees
  else {
    bitOut.writeBit( 0 );

    //! Recursively call left then right subtrees
    writeHeader( node->child0, bitOut );
    writeHeader( node->child1, bitOut );
  }
}

/** Input params: Input file length, reference to bit output stream
 *  Return param: None
 *  Description : Write character count of input file to compressed file after
 *                header info using 4 bytes.
 */
void HCTree::writeCharCount( unsigned int length, BitOutputStream &bitOut ) {
  byte ch = 0;

  //! Since int is 4 bytes (i.e. 32 bits), construct 4 8-bits char
  for( int i = 24; i >= 0; i -= 8 ) {
    ch = (length >> i) & ~0;

    //! Write the 8-bits symbol containing 1/4th char count info
    for( int j = 0; j < 8; j++ )
      bitOut.writeBit( (ch & (1 << j)) >> j );
  }
}

/** Input params: Reference to the bit input object
 *  Return param: Character count that should be in uncompressed file
 *  Description : Use the Huffman algorithm to build a Huffman coding trie.
 */
unsigned int HCTree::buildTreeUncomp( BitInputStream &bin ) {
  //! Read header info from compressed file (start at root - nullptr)
  readHeader( nullptr, root, bin );

  //! Return character count read from compressed file
  return (readCharCount( bin ));
}

/** Input params: Pointers to parent, node and reference to bit input stream
 *  Return param: None
 *  Description : Read header information from compressed file.
 */
void HCTree::readHeader( HCNode *par, HCNode* &node, BitInputStream &bitIn ) {
  //! Read the next bit from compressed file
  unsigned int bit = bitIn.readBit();

  //! If read bit is 0
  if( !bit ) {
    //! Create internal node and establish pointer associations
    HCNode *p = new HCNode( false, 0, 0 );
    p->parent = par;
    node      = p;

    //! Recursively read header info till we reach a leaf node
    readHeader( p, p->child0, bitIn );
    readHeader( p, p->child1, bitIn );
  }
  //! If read bit is 1
  else {
    byte ch = 0;

    //! Read next 8-bits to get character symbol at leaf node
    for( int i = 0; i < 8; i++ )
      ch = ch | (bitIn.readBit() << i);

    //! Create leaf node with ch as symbol and establish pointer associations
    HCNode *p = new HCNode( true, 0, ch );
    p->parent = par;
    node      = p;
  }
}

/** Input params: Reference to bit input stream
 *  Return param: Character count that should be in uncompressed file
 *  Description : Read 4 bytes from compressed file to get character count.
 */
unsigned int HCTree::readCharCount( BitInputStream &bitIn ) {
  //! To handle huge files (2^64 bytes = 18 446 744 073 709 551 616)
  unsigned long long int count  = 0;
  byte ch                       = 0;

  for( int i = 32; i > 0; i -= 8 ) {
    ch = 0;

    //! Read next 8-bits to get 1/4th info of character count
    for( int j = 0; j < 8; j++ )
      ch = ch | (bitIn.readBit() << j);

    //! Construct character count bytewise
    count += (unsigned int) ch * pow( 2, i );
  }

  //! Divide by 2^8 to get the integer value of character count then return it
  count /= pow( 2, 8 );
  return ((unsigned int) count);
}

/** Input params: Symbol to encode and reference to bit output stream
 *  Return param: None
 *  Description : Encode a symbol using the Huffman code tree built previously.
 */
void HCTree::encode( byte symbol, BitOutputStream &out ) const {
  int index = (int) symbol;

  //! Only consider leaf nodes having non-zero frequency for encoding
  if( leaves[index] && leaves[index]->count ) {
    string edge = "";
    HCNode *current = leaves[index];

    //! Starting at leaf traverse upward via parents to construct edge info
    while( current->parent != nullptr ) {
      if( current->parent->child0 == current )
        edge += "0";
      else if( current->parent->child1 == current )
        edge += "1";
      else
        return;

      current = current->parent;
    }

    //! Reverse the edge string to get Huffman codeword for corresponding symbol
    reverse( edge.begin(), edge.end() );

    //! Write out the codeword using bit output stream
    for( byte ch : edge ) {
      if( ch == '0' )
        out.writeBit( 0 );
      else if( ch == '1' )
        out.writeBit( 1 );
      else {
        cout << "Something went wrong while encoding " << symbol << endl;
        break;
      }
    }
  }
}

/** Input params: Reference to bit input stream
 *  Return param: ASCII value of decoded symbol
 *  Description : Use the Huffman algorithm to build a Huffman coding trie.
 */
int HCTree::decode( BitInputStream &in ) const {
  //! Start at the root
  HCNode *current = root;
  unsigned int ch;

  //! Nothing to decode (empty files)
  if( current == nullptr )
    return -1;

  //! If current node is not a leaf node
  while( current->child0 != nullptr && current->child1 != nullptr ) {
    //! Read next bit from bit input stream
    ch = in.readBit();

    //! If read bit is 0, traverse left in Huffman tree
    if( ch == 0 )
      current = current->child0;
    //! If read bit is 1, traverse right in Huffman tree
    else if( ch == 1 )
      current = current->child1;
    //! Stop decoding if eof is hit (safety check)
    else if( ch == 2 )
      return -1;
  }

  //! Return symbol stored at leaf node
  return ((int) current->symbol);
}

/** Input params: None
 *  Return param: None
 *  Description : Default destructor.
 */
HCTree::~HCTree() {
  //! delete all nodes of the Huffman tree (call with root)
  deleteAll( root );
}

/** Input params: A node in the tree
 *  Return param: None
 *  Description : Does a postorder traversal, deleting all nodes in the HCT.
 */
void HCTree::deleteAll( HCNode *n ) {
  //! start at node n
  HCNode *current = n;

  if( current == nullptr )
    return;

  //! recursively call deleteAll for all left children
  if( current->child0 != nullptr )
    deleteAll( current->child0 );

  //! recursively call deleteAll for all right children
  if( current->child1 != nullptr )
    deleteAll( current->child1 );

  //! memory deallocation
  delete current;
  current = nullptr;
}
