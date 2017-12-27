/////////////////////////////////////////////////////////////////////////////
//Author: 	Izcalli64 - www.gocaco.com
//Updated: 	2008-11-17
//File:		igtl_Directory.h
//Bullet: 	Simply enumerates a single directory
//
//This is free; Use and distribute freely for any purpose
//
//Code is derived from each relevant OS's API; Thus, it is already free.
//
//wxWidgets helped in deriving this code (support them)
//
//
//TODO: Eliminate STL dependency?
//		*NIX
//		MAC testing
//		furthur Windows error testing
//
//



#ifndef IGTL_DIRECTORY_READER_H
#define IGTL_DIRECTORY_READER_H

#include <string>
using std::string;

#ifdef WIN32	//WIN32

	#include <stddef.h>
	#define WIN32_EXTRA_LEAN_AND_MEAN
	#include <windows.h>	//gyahhh...
	#include <winbase.h>	//Requires Kernel32.dll
#endif
#ifdef UNIX		//*NIX POSIX ?

	#error "*NIX Not supported yet"
#endif
#ifdef DARWIN	//OS X

	#error "OS X Not supported yet"
#endif
#ifdef MAC	//old MAC

	#error "MAC Not supported yet"
#endif


namespace IGTL{

//
//!Cross platform class for simple directory enumeration & loading
//
class Dir{

	public:

		Dir();	//!< Nothin'
		Dir( const std::string  & D );	//!< Starts by opening a directory
		~Dir();

		inline bool Valid() const { return m_curr_valid; }//!< Returns true while this enumeration procedure is valid

		bool GetDir( std::string  & S ) const;//!< Gets the current directory name (sets it to S), returns false on failure
		bool Get( std::string  & S ) const;//!< Gets the current filename (sets it to S), returns false on failure

		void Close();//!< Closes the directory.
		bool Open( const std::string  & D );	//!< Opens a specified directory, returns false if failure
		bool First();	//!Start at the first directory entry, again.
		bool Next();	//!< Goes to the next filename, returns false if end of directory

		bool IsDir() const;	//!< Returns true if this current file is ANOTHER directory
		bool IsHidden() const;//!< Returns true if this current file is a hidden file
		bool IsSys() const;	//!< Returns true if this current file is a system file
		bool IsReadOnly() const;//!< Returns true if this current file is read-only
		bool IsNormal() const;//!< Returns true if this current file is a normal file

		//MakeDir() -> //CreateDirectory( dirname.c_str(), NULL );  -> works great, requires kernel32.lib for windows

	private:

		bool m_curr_valid;	//!< True if this directory is valid

		#ifdef WIN32	//Microsoft windows

			std::string m_curr_directory;	//!< The current directory path (formatted however you need it)
			WIN32_FIND_DATA msw_finddata;
			HANDLE msw_handle;
			DWORD msw_attr;
		#endif
		#ifdef UNIX		//Ideally, any POSIX linux style operating system
			//*NIX POSIX ?
		#endif
		#ifdef DARWIN	//IS the new Leaopard not backwards compliant for directories?
			//OS X
		#endif
		#ifdef MAC		//Old way to enumerate on a maccy
			//old MAC
			//FSIterator
			//err = FSOpenIterator(&dirRef, kFSIterateFlat, &m_iterator);
			//FSCloseIterator( m_iterator ) ;
		#endif
};
};

#endif // IGTL_DIRECTORY_READER_H
