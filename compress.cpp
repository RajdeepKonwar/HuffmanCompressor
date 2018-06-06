/**
 @author Rajdeep Konwar (A53225609)
 
 @date Nov 10 2017
 
 @section OVERVIEW
 Implementation of compress program.
 
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
         << "Usage: ./compress infile outfile" << endl;
    return -1;
  }

  //! Open infile (original file) in binary read mode
  FILE *file = fopen( argv[1], "rb" );
  if( !file ) {
    cout << "Invalid input file. No file was opened. Please try again." << endl;
    return -1;
  }

  //! Get size of infile
  fseek( file, 0, SEEK_END );
  unsigned int lengthIn = ftell( file );

  //! Close infile
  fclose( file );
  //! Reopen infile in binary read mode
  file = fopen( argv[1], "rb" );

  //! Char buffer of size equal to infile to store chunk
  byte *buffer = new byte [lengthIn];

  cout << "Reading from file \"" << argv[1] << "\"... " << flush;
  //! Read the entire file at once and store in buffer
  fread( buffer, sizeof( byte ), lengthIn, file );

  //! Close infile
  fclose( file );
  cout << "done.\n";
  cout << "Input file size: " << lengthIn << " bytes.\n";

  //! Populate vector with freqs of occurrence of each ASCII char in infile
  vector< int > charFreq( 256, 0 );
  for( unsigned int i = 0; i < lengthIn; i++ )
    charFreq[(int) buffer[i]]++;

  //! Open outfile (compressed file) in binary write mode
  ofstream outfile( argv[2], ios::binary );
  if( !outfile.is_open() ) {
    cout << "Unable to open output file. Please try again." << endl;
    delete[] buffer;
    return -1;
  }

  //! Bit out stream object initialized with outfile
  BitOutputStream bitOut( outfile );
  HCTree hctComp;   //! Huffman code tree object

  cout << "Building Huffman coding tree... " << flush;
  //! Build the Huffman code tree
  hctComp.buildTreeComp( charFreq, lengthIn, bitOut );
  cout << "done." << endl;

  byte symbol;
  //! Reopen infile in binary read mode for encoding symbols
  ifstream infile( argv[1], ios::binary );

  cout << "Writing to file \"" << argv[2] << "\"... " << flush;

  while( 1 ) {
    //! Read next char from infile
    symbol = infile.get();

    //! Stop encoding if infile's eof reached
    if( infile.eof() )
      break;

    //! Enocde symbols to outfile
    hctComp.encode( symbol, bitOut );
  }

  //! Complete padding of last byte to ensure we don't lose information
  bitOut.completeByte();
  cout << "done." << endl;

  //! Get size of compressed file and calculate compression ratio
  outfile.seekp( 0, outfile.end );
  unsigned int lengthOut = outfile.tellp();
  cout << "Output file size: " << lengthOut << " bytes." << endl;
  cout << "Compression ratio: " << (float) lengthOut / (float) lengthIn << endl;

  //! Close all files and deallocate memory
  infile.close();
  outfile.close();
  delete[] buffer;

  //! Finish time
  t = clock() - t;
  cout << "Time taken: " << (float) t / CLOCKS_PER_SEC << "s" << endl;

  return 0;
}
