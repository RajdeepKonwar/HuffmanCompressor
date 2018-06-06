/**
 @author Rajdeep Konwar (A53225609)
 
 @date Nov 10 2017
 
 @section OVERVIEW
 Implementation of uncompress program.
 
 @section ASSIGNMENT NUMBER
 PA3
 */

#include <fstream>
#include "HCNode.h"
#include "HCTree.h"

int main( int argc, char **argv ) {
  //! Start time
  clock_t t   = clock();

  //! Check for Arguments
  if( argc != 3 ) {
    cout << "Invalid number of arguments." << endl
         << "Usage: ./uncompress infile outfile" << endl;
    return -1;
  }

  //! Open infile (compressed file) in binary read mode, starting at the end
  ifstream infile( argv[1], ios::in | ios::binary | ios::ate );
  if( !infile.is_open() ) {
    cout << "Invalid input file. No file was opened. Please try again." << endl;
    return -1;
  }

  //! Get size of infile
  unsigned int length1 = infile.tellg();
  infile.seekg( 0, infile.beg );

  //! Open outfile (uncompressed file) in binary write mode
  ofstream outfile( argv[2], ios::binary );
  if( !outfile.is_open() ) {
    cout << "Unable to open output file. Please try again." << endl;
    return -1;
  }

  cout << "Reading header from file \"" << argv[1] << "\"." << endl;
  cout << "Input file size: " << length1 << " bytes." << endl;

  //! Bit input stream object initialized with infile
  BitInputStream bitIn( infile );
  HCTree hctUncomp;

  cout << "Building Huffman coding tree... " << flush;
  //! Build Huffman code tree and get character count from compressed file
  unsigned int chCnt = hctUncomp.buildTreeUncomp( bitIn );
  cout << "done." << endl;

  cout << "Writing to file \"" << argv[2] << "\"... " << flush;
  int symbol;
  unsigned int counter = 0;

  //! Loop till counter is not equal to character count
  while( counter != chCnt ) {
    //! Decode and get symbol from compressed file
    symbol = hctUncomp.decode( bitIn );

    //! EOF reached somehow
    if( symbol == -1 )
      break;

    //! Write decoded symbol to outfile
    outfile.put( (byte) symbol );

    //! Increment counter
    counter++;
  }
  cout << "done." << endl;

  //! Get size of uncompressed file and calculate uncompression ratio
  outfile.seekp( 0, outfile.end );
  unsigned int length2 = outfile.tellp();
  cout << "Output file size: " << length2 << " bytes." << endl;
  cout << "Uncompression ratio: " << (float) length2 / (float) length1 << endl;

  //! Close all files
  infile.close();
  outfile.close();

  //! Finish time
  t = clock() - t;
  cout << "Time taken: " << (float) t / CLOCKS_PER_SEC << "s" << endl;

  return 0;
}
