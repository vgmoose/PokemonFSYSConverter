#ifndef IGTL_BYTE_TOOLS
#define IGTL_BYTE_TOOLS

#include <string>

//HL = High to Low, AKA data in is from MSB to LSB
//LH = Low to High, AKA data in is from LSB to MSB (Network Byte Order)

#ifndef uchar
typedef unsigned char uchar;
#endif
#ifndef ushort
typedef unsigned short ushort;
#endif
#ifndef uint
typedef unsigned int uint;
#endif

//
//UChars to integers
//
unsigned int UChar4ToUIntHL( unsigned char * d );
unsigned int UChar3ToUIntHL( unsigned char * d );
unsigned int UChar2ToUIntHL( unsigned char * d );
unsigned int UCharNToUIntHL( unsigned char * d, const unsigned int & n  );

//
//UChars to floats
//
float UChar4ToFloatHL( unsigned char * d );

//
//Byte to readable string conversions
//
unsigned char NibbleToHexChar( const unsigned char & data );
std::string UCharToString( const unsigned char data );
std::string UCharsToString( const unsigned char * data, unsigned int n );
std::string UInt4ToString( const unsigned int & x );

//
//Byte comparisons
//
bool UCharCompare( const unsigned char * A, const unsigned char * B, unsigned int i );
bool UCharCompare( const unsigned char * A, const char * B, unsigned int i );

//
//OpenGL Byte manipulation tools
//
unsigned int GLubyteNToUIntHL( unsigned char * d, const unsigned int & n  );


//
//Common compressed color formats
//


//
//Common compression algorithms(LZ77, LZSS...)
//

//
//Quantization conversions
//
float UInt2ToFloatS( const unsigned int & v );

unsigned int GetLowestPowerOfTwo( const unsigned int & v );
unsigned int GetHighestPowerOfTwo( const unsigned int & v );

#endif// IGTL_BYTE_TOOLS
