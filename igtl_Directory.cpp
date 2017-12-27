#ifndef IGTL_DIRECTORY_READER_CPP
#define IGTL_DIRECTORY_READER_CPP

#include "igtl_Directory.h"

namespace IGTL{


Dir::Dir(){	//!< Nothin'

	m_curr_valid = false;
	//m_curr_directory.reserve(255);
	//m_curr_filename.reserve(255);

	#ifdef WIN32
		//msw_finddata = INVALID_HANDLE_VALUE;
		msw_handle = INVALID_HANDLE_VALUE;
		msw_attr = 0;
	#endif
}
Dir::Dir( const std::string  & D ){	//!< Starts by opening a directory

	#ifdef WIN32

		Open( D );
	#endif
}
Dir::~Dir(){	//!< Starts by opening a directory

	#ifdef WIN32

		Close();
	#endif
}
void Dir::Close(){//!< Closes the directory.

	#ifdef WIN32

		if( msw_handle != INVALID_HANDLE_VALUE ){

			if ( !::FindClose(msw_handle) ){

				//Was an error on closing!
			}
		}

		msw_handle = INVALID_HANDLE_VALUE;
	#endif
	m_curr_valid = false;
}

bool Dir::Open( const std::string  & D ){	//!< Opens a specified directory, returns false if failure

	#ifdef WIN32
		//
		//Close any open directory first!
		//
		msw_handle = INVALID_HANDLE_VALUE;
		m_curr_valid = false;

		//
		//Format directory string as needed
		//
		m_curr_directory = D;

		if( *(m_curr_directory.end() - 1) != '\\' ){

			m_curr_directory += '\\';
		}

		if( msw_handle == INVALID_HANDLE_VALUE ){	//Open our directory after closeing the old one

			msw_handle = ::FindFirstFile( (m_curr_directory+"*.*").c_str() , &msw_finddata );
		}

		if( msw_handle == INVALID_HANDLE_VALUE ){	//Did we get any problems?

			DWORD err = ::GetLastError();

			if ( (err != ERROR_FILE_NOT_FOUND) && (err != ERROR_NO_MORE_FILES) ){

				//wxLogSysError(err, _("Can not enumerate files in directory '%s'"),m_dirname.c_str());
			}
		}else{

			m_curr_valid = true;
		}
	#endif
	return m_curr_valid;
}

bool Dir::First(){	//!Start at the first directory entry, again.

	#ifdef WIN32

		if( m_curr_valid ){	// Must be valid to do a first (Open was called)

			m_curr_valid = false;
			msw_handle = ::FindFirstFile( (m_curr_directory+"*.*").c_str() , &msw_finddata);

			if( msw_handle == INVALID_HANDLE_VALUE ){

				DWORD err = ::GetLastError();

				if ( (err != ERROR_FILE_NOT_FOUND) && (err != ERROR_NO_MORE_FILES) ){

					//wxLogSysError(err, _("Can not enumerate files in directory '%s'"),m_dirname.c_str());
				}
			}else{

				m_curr_valid = true;
			}
		}
	#endif

	return m_curr_valid;
}

bool Dir::Next(){	//!< Goes to the next filename, returns false if end of directory

	#ifdef WIN32
		if( m_curr_valid ){	// Must be valid to do a first (Open was called)

			m_curr_valid = ( ::FindNextFile(msw_handle, &msw_finddata) != 0 );
		}
	#endif

	return m_curr_valid;
}

bool Dir::GetDir( std::string  & S ) const {//!< Gets the current directory name (sets it to S), returns false on failure

	if( m_curr_valid ){

		#ifdef WIN32

			S = m_curr_directory;
		#endif
	}
	return m_curr_valid;
}

bool Dir::Get( std::string  & S ) const {//!< Gets the current filename (sets it to S), returns false on failure

	if( m_curr_valid ){

		#ifdef WIN32

			S = msw_finddata.cFileName;
		#endif
	}
	return m_curr_valid;
}

bool Dir::IsDir() const{	//!< Returns true if this current file is ANOTHER directory

	if( m_curr_valid ){

		#ifdef WIN32

			return ( (msw_finddata.dwFileAttributes) & FILE_ATTRIBUTE_DIRECTORY) != 0;
		#endif
	}

	return false;
}

bool Dir::IsHidden() const{//!< Returns true if this current file is a hidden file

	if( m_curr_valid ){

		#ifdef WIN32

			return ( (msw_finddata.dwFileAttributes) & (FILE_ATTRIBUTE_HIDDEN)) != 0;
		#endif
	}

	return false;
}

bool Dir::IsSys() const{	//!< Returns true if this current file is a system file

	if( m_curr_valid ){

		#ifdef WIN32
			return ( (msw_finddata.dwFileAttributes) & (FILE_ATTRIBUTE_SYSTEM)) != 0;
		#endif
	}

	return false;
}

bool Dir::IsReadOnly() const{//!< Returns true if this current file is read-only

	if( m_curr_valid ){

		#ifdef WIN32

			return ( (msw_finddata.dwFileAttributes) & (FILE_ATTRIBUTE_READONLY)) != 0;
		#endif
	}

	return false;
}

bool Dir::IsNormal() const{//!< Returns true if this current file is a normal file

	if( m_curr_valid ){

		#ifdef WIN32

			//FILE_ATTRIBUTE_ARCHIVE
			//FILE_ATTRIBUTE_COMPRESSED
			//FILE_ATTRIBUTE_DEVICE
			//FILE_ATTRIBUTE_DIRECTORY
			//FILE_ATTRIBUTE_ENCRYPTED
			//FILE_ATTRIBUTE_HIDDEN
			//FILE_ATTRIBUTE_NORMAL
			//FILE_ATTRIBUTE_NOT_CONTENT_INDEXED
			//FILE_ATTRIBUTE_OFFLINE
			//FILE_ATTRIBUTE_READONLY
			//FILE_ATTRIBUTE_REPARSE_POINT
			//FILE_ATTRIBUTE_SPARSE_FILE
			//FILE_ATTRIBUTE_SYSTEM
			//FILE_ATTRIBUTE_TEMPORARY
			//FILE_ATTRIBUTE_VALID_FLAGS

			return ( (msw_finddata.dwFileAttributes) & (FILE_ATTRIBUTE_NORMAL)) != 0;
		#endif
	}

	return false;
}

};

#endif //IGTL_DIRECTORY_READER_CPP
