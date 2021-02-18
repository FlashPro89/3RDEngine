#include "File.h"

gFile::gFile(const gString& filename, bool writeable, 
	bool binary, bool addAtEnd) : m_filename(filename)
{
	auto mode = writeable ? std::ios_base::in : std::ios_base::in | std::ios_base::out;
	if (binary) mode |= std::ios_base::binary;
	if (addAtEnd) mode |= std::ios_base::ate | std::ios_base::app;

	m_fstream.open(m_filename, mode);
}

gFile::~gFile()
{
	if (m_fstream.is_open())
		m_fstream.close();
}

bool gFile::flush() const // write buffer to disk if possibly
{
	return false;
}

bool gFile::isBinary() const
{
	return 0;
}

bool gFile::isWriteable() const
{
	return 0;
}

const gString& gFile::getFileName() const
{
	return m_filename;
}

size_t gFile::getFileSize() const
{
	return 0;
}

bool gFile::seek(gFileSeek mode, unsigned int position) const
{
	return 0;
}

size_t gFile::tell() const
{
	return 0;
}

size_t gFile::read(void* dst, size_t size) const
{
	return 0;
}

size_t gFile::write(void* src, size_t size)
{
	return 0;
}

size_t gFile::gets(gString& gString) const
{
	return 0;
}

size_t gFile::gets(char* dst, size_t buffsz) const
{
	return 0;
}

size_t gFile::puts(const gString& gString)
{
	return 0;
}

size_t gFile::puts(const char* src)
{
	return 0;
}

size_t gFile::printf( const char* fmt, ... )
{
	return 0;
}

size_t gFile::scanf( const char* fmt, ... ) const
{
	return 0;
}

char gFile::getc(bool nostep) const
{
	return 0;
}

bool gFile::putc(char c)
{
	return 0;
}

bool gFile::eof() const
{
	return 0;
}