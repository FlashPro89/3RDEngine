#pragma once

#ifndef _3RDE_FILE_
#define _3RDE_FILE_
#include <cstdio>
#include "Interfaces.h"

class gFile : public IFile
{
public:

	gFile( const char* filename, bool writeable, bool binary = false, 
		bool addAtEnd = false, size_t bufferSize = 0);
	gFile( const gString& filename, bool writeable, bool binary = false, 
		bool addAtEnd = false, size_t bufferSize = 0 );
	~gFile();

	bool flush() const; // write buffer to disk if possibly

	bool isOpened() const;
	bool isBinary() const;
	bool isWriteable() const;
	const gString& getFileName() const;

	size_t getFileSize() const;

	bool seek( long position, gFileSeek mode ) const;
	long tell() const;

	size_t read( void* dst, size_t size ) const;
	size_t write( void* src, size_t size );

	bool gets( gString& str ) const;
	bool gets( char* dst, int buffsz ) const;
	bool puts( const gString& str );
	bool puts( const char* src );

	int print(const char* fmt, ...);
	int print(const gString& fmt, ...);
	int scan(char* fmt, ...) const;
	int scan(gString& fmt, ...) const;

	//for logger
	int printVA(const char* fmt, va_list args);
	int scanVA(char* fmt, va_list args) const;

	char getc(bool nostep = true) const;
	bool putc(char c);
	bool eof() const;

protected:
	gFile() {};
	gFile(const gFile&) {};

	mutable FILE* m_file;

	gString m_filename;
	bool m_isWritable;
	bool m_isBinary;


};

#endif
