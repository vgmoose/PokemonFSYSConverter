
#ifndef IGTL_BYTE_TOOLS_CPP
#define IGTL_BYTE_TOOLS_CPP

#include "bytetools.h"

unsigned int UChar4ToUIntHL( unsigned char * d ){

	return	(((unsigned int)d[0])<<24) |
			(((unsigned int)d[1])<<16) |
			(((unsigned int)d[2])<<8) |
			(((unsigned int)d[3])<<0);
}

unsigned int UChar3ToUIntHL( unsigned char * d ){

	return	(((unsigned int)d[0])<<16) |
			(((unsigned int)d[1])<<8) |
			(((unsigned int)d[2])<<0);
}

unsigned int UChar2ToUIntHL( unsigned char * d ){

	return	(((unsigned int)d[0])<<8) |
			(((unsigned int)d[1])<<0);
}

unsigned int UCharNToUIntHL( unsigned char * d, const unsigned int & n  ){

	switch( n ){
		case 0: return 0;
		case 1:	return *d; break;
		case 2: return UChar2ToUIntHL( d ); break;
		case 3: return UChar3ToUIntHL( d ); break;
		case 4: return UChar4ToUIntHL( d ); break;
		default:
			return UChar4ToUIntHL( d + (n-4) ); break;
		break;
	}
}

float UChar4ToFloatHL( unsigned char * d ){

	uint D = UChar4ToUIntHL( d );
	return *((float *)(( uint * )&D));
}

unsigned char NibbleToHexChar( const unsigned char & data ){

	switch( data & 15 ){
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
			return ( (data & 15) + ('A' - 10));
		default:
			return ( (data & 15) + '0');
	}
}

std::string UCharToString( const unsigned char data ){

	std::string rets;
	rets += NibbleToHexChar( data >> 4 );
	rets += NibbleToHexChar( data );
	return rets;
}

std::string UCharsToString( const unsigned char * data, unsigned int n ){

	std::string rets = "";
	const unsigned char * c = data;
	const unsigned char * c_final = c + n;
	while( c != c_final ){
		rets += UCharToString( *c );
		rets += " ";
		c++;
	}
	return rets;
}

std::string UInt4ToString( const unsigned int & x ){

	std::string rets = "";
	unsigned char * c = (unsigned char*)&x;
	rets += UCharToString( *(c + 3) );
	rets += UCharToString( *(c + 2) );
	rets += UCharToString( *(c + 1) );
	rets += UCharToString( *(c + 0) );
	return rets;
}

bool UCharCompare( const unsigned char * A, const unsigned char * B, unsigned int i ){

	const unsigned char * a = A;
	const unsigned char * b = B;
	const unsigned char * a_final = a + i;
	while( a != a_final ){

		if( *a != *b ) { return false; }
		a++; b++;
	}
	return true;
}
bool UCharCompare( const unsigned char * A, const char * B, unsigned int i ){ return UCharCompare( (uchar*)A, (uchar*)B, i ); }


unsigned int GLubyteNToUIntHL( unsigned char * d, const unsigned int & n  ){

	switch( n ){
		case 0: return 0;
		case 1:	return *d; break;
		case 2: return UChar2ToUIntHL( d ); break;
		case 3: return UChar3ToUIntHL( d ); break;
		case 4: return UChar4ToUIntHL( d ); break;
		default:
			return UChar4ToUIntHL( d + (n-4) ); break;
		break;
	}
}

float UInt2ToFloatS( const unsigned int & v ){//Convert a unsigned int into a signed 2 byte float

	if( (v & 32768) != 0 ){

		return ( -(float)( ((int)65535 - (int)v) & 32767 ) ) / ((float)32767.0);
	}else{

		return ((float)( (int)v )) / ((float)32767.0);
	}
}

unsigned int GetLowestPowerOfTwo( const unsigned int & v ){

	unsigned int shift = 0;
	unsigned int value = v;
	while( shift < 31 ){

		if( value & 0x80000000 ){

			return (1 << (31 - shift) );
		}
		value <<= 1;
		shift++;
	}
	return 0;
}

unsigned int GetHighestPowerOfTwo( const unsigned int & v ){

	unsigned int onecount = 0;
	unsigned int shift = 1;
	unsigned int value = v;
	while( value != 0 ){
		onecount += (value & 1);	//Avoid an if statement.
		value >>= 1;
		shift <<= 1;
	}
	shift >>= (onecount == 1);	//Avoid another
	return shift;
}

#endif// IGTL_BYTE_TOOLS_CPP
