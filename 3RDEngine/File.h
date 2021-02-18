#pragma once

#ifndef _3RDE_FILE_
#define _3RDE_FILE_

#include "Interfaces.h"
#include <fstream>

class gFile : public IFile
{
public:

	gFile( const gString& filename, bool writeable, bool binary = false, bool addAtEnd = false );
	~gFile();

	bool flush() const; // write buffer to disk if possibly

	bool isBinary() const;
	bool isWriteable() const;
	const gString& getFileName() const;

	size_t getFileSize() const;

	bool seek(gFileSeek mode, unsigned int position = 0) const;
	size_t tell() const;

	size_t read( void* dst, size_t size ) const;
	size_t write( void* src, size_t size );

	size_t gets( gString& gString ) const;
	size_t gets( char* dst, size_t buffsz ) const;
	size_t puts(const gString& gString);
	size_t puts( const char* src );

	size_t printf(const char* fmt, ...);
	size_t scanf(const char* fmt, ...) const;

	char getc(bool nostep = true) const;
	bool putc(char c);
	bool eof() const;

protected:
	gFile() {};
	gFile(const gFile&) {};

	std::fstream m_fstream;
	gString m_filename;
};

#endif
