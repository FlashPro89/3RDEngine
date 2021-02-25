#include "File.h"
#include "Engine.h" // for exception handler
#include <assert.h>

gFile::gFile(const char* filename, bool writeable, 
	bool binary, bool addAtEnd, size_t bufferSize) :
								  m_filename(filename),
								  m_isWritable(writeable),
								  m_isBinary(binary)
{
	gString mode;

	writeable ? mode += 'w' : mode += 'r';
	binary ? mode += 'b' : mode += 't';

	//ELOGMSG("Try open file: %s mode:%s", m_filename.c_str(), mode.c_str());
	ECHECK(!fopen_s(&m_file, m_filename.c_str(), mode.c_str()),
		("Cannot open file: " + m_filename).c_str());


	if (bufferSize > 1)
		setvbuf(m_file, 0, _IOFBF, bufferSize);
}


gFile::gFile( const gString& filename, bool writeable,
	bool binary, bool addAtEnd, size_t bufferSize ) :
								  m_filename(filename),
								  m_isWritable(writeable), 
						          m_isBinary(binary)
{
	gString mode;

	if (addAtEnd)
		mode = "a";
	else
		writeable ? mode = 'w' : mode = 'r';

	binary ? mode += 'b' : mode += 't';
	
	//ELOGMSG( "Try open file: %s mode:%s", m_filename.c_str(), mode.c_str());
	ECHECK( !fopen_s( &m_file, m_filename.c_str(), mode.c_str() ), 
		( "Cannot open file: " + m_filename).c_str() );

	if ( bufferSize > 1 )
		setvbuf(m_file, 0, _IOFBF, bufferSize);
}

gFile::~gFile()
{
	if (m_file)
	{
		ELOGMSG((gString("Closing file:") + m_filename).c_str());
		int err = fclose(m_file);

		if (err != 0)
		{
			ELOGWRN((gString("Failed close file:") + m_filename).c_str());
		}
	}
}

bool gFile::flush() const // write buffer to disk if possibly
{
	return fflush(m_file);
}

bool gFile::isOpened() const
{
	return(m_file != 0);
}

bool gFile::isBinary() const
{
	return m_isBinary;
}

bool gFile::isWriteable() const
{
	return m_isWritable;
}

const gString& gFile::getFileName() const
{
	return m_filename;
}

size_t gFile::getFileSize() const
{
	size_t ret, curPos = tell();
	seek( 0, gFileSeek::GFS_END );
	ret = tell();
	seek( curPos, gFileSeek::GFS_SET );
	return ret;
}

bool gFile::seek( long position, gFileSeek mode ) const
{
	int origin;
	switch (mode)
	{
	case gFileSeek::GFS_SET:
		origin = SEEK_SET; break;
	case gFileSeek::GFS_CURRENT:
		origin = SEEK_CUR; break;
	case gFileSeek::GFS_END:
		origin = SEEK_END; break;
	};
	return fseek(m_file, position, origin);
}

long gFile::tell() const
{
	return ftell( m_file );
}

size_t gFile::read(void* dst, size_t size) const
{
	return fread_s( dst, size, 1, size, m_file );
}

size_t gFile::write( void* src, size_t size )
{
	assert(!m_isWritable && "Write to ReadOnly file!");
	return fwrite( src, 1, size, m_file );
}

bool gFile::gets( gString& str ) const
{
	return 0 != fgets( &str[0], str.capacity(), m_file );
}

bool gFile::gets( char* dst, size_t buffsz ) const
{
	return 0 != fgets( dst, buffsz, m_file);
}

bool gFile::puts( const gString& str )
{
	return 0 != fputs( str.c_str(), m_file );
}

bool gFile::puts(const char* src)
{
	return 0 != fputs(src, m_file);
}

size_t gFile::print( const char* fmt, ... )
{
	int result = 0;

	va_list argList;
	__crt_va_start(argList, fmt);
	result = printVA( fmt, argList);
	__crt_va_end(argList);

	return result;
}

size_t gFile::print( const gString& fmt, ...)
{
	int result = 0;

	va_list argList;
	const char* str = fmt.c_str();
	__crt_va_start( argList, str );
	result = printVA( str, argList);
	__crt_va_end(argList);

	return result;
}

size_t gFile::scan( char* fmt, ... ) const
{
	int result = 0;
	va_list argList;
	__crt_va_start(argList, fmt);
	result = scanVA( fmt, argList );
	__crt_va_end(argList);

	return result;
}

size_t gFile::scan( gString& fmt, ...) const
{
	int result = 0;
	va_list argList;
	char* str = &fmt[0];
	__crt_va_start(argList, str);
	result = scanVA(str, argList);
	__crt_va_end(argList);

	return result;
}

size_t gFile::printVA(const char* fmt, va_list args)
{
	return _vfprintf_s_l(m_file, fmt, NULL, args);

}

size_t gFile::scanVA(char* fmt, va_list args) const
{
	return _vfscanf_s_l(m_file, fmt, NULL, args);
}

char gFile::getc(bool nostep) const
{
	if(nostep)
		return static_cast<char>( fgetc(m_file) );

	char c = fgetc(m_file);
	seek(-1, gFileSeek::GFS_CURRENT);
	return c;
}

bool gFile::putc(char c)
{
	return fputc( static_cast<int>(c), m_file);
}

bool gFile::eof() const
{
	return feof(m_file);
}

