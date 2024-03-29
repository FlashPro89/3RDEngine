#pragma once

#ifndef _3RDE_INTERFACES_
#define _3RDE_INTERFACES_

#include <memory>
#include <string>

typedef std::string gString;

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;

typedef void(*lpfnThrowException)(const gString&,const gString&,int,const gString&);

class ISystem
{
public:
	virtual ~ISystem() {};
	virtual bool initialize() = 0;

protected:
	ISystem() {};
	ISystem(const ISystem& s) {}

	virtual bool finalize() = 0;
};

class IExceptionHandler
{
public:
	virtual ~IExceptionHandler() {};

	virtual const char* getExceptionDescription() const = 0;

protected:
	IExceptionHandler() {};
};

class IFile
{
public:

	enum class gFileSeek
	{
		GFS_SET = 0,
		GFS_CURRENT = 1,
		GFS_END = 2
	};

	~IFile() {};

	virtual bool flush() const = 0; // write buffer to disk if possibly

	virtual bool isOpened() const = 0;
	virtual bool isBinary() const = 0;
	virtual bool isWriteable() const = 0;
	virtual const gString& getFileName() const = 0;

	virtual size_t getFileSize() const = 0;

	// pos methods
	virtual bool seek( long position, gFileSeek mode = gFileSeek::GFS_SET ) const = 0;
	virtual long tell() const = 0;

	// binary I/O
	virtual size_t read( void* dst, size_t size ) const = 0;
	virtual size_t write( void* src, size_t size ) = 0;

	// text I/O
	virtual bool gets(gString& gString) const = 0;
	virtual bool gets(char* dst, int buffsz) const = 0;
	virtual bool puts(const gString& gString) = 0;
	virtual bool puts(const char* src) = 0;

	// formatted text I/O
	virtual int print( const char* fmt, ... ) = 0;
	virtual int print( const gString& fmt, ...) = 0;
	virtual int scan( char* fmt, ... ) const = 0;
	virtual int scan( gString& fmt, ...) const = 0;

	virtual char getc( bool nostep = true ) const = 0;
	virtual bool putc( char c ) = 0;
	virtual bool eof( ) const = 0;

protected:
	IFile() {};
	IFile(const IFile&) {};

};
typedef std::shared_ptr<IFile> SPFILE;

class IListEnumerator
{
public:
	~IListEnumerator() {}

	virtual bool get( gString& content ) const = 0; // while( this->get(content) ) ... list.add(content) ... e.t.c
	virtual unsigned int getElementsNum()const = 0;

	virtual bool isEmpty() = 0;

protected:
	IListEnumerator() {}
	IListEnumerator(const IListEnumerator&) {}
};
typedef std::shared_ptr<IListEnumerator> SPLISTENUMERATOR;


class IPlatform : public ISystem
{
public:

	enum class ePLATFORMTYPE
	{
		PT_WIN,
		PT_LINUX,
		PT_ANDROID,
		PL_IOS
	};

	class IWindow 
	{
	//friend class IPlatform;
	public:
		struct gWINDOWPARAMS
		{
			gWINDOWPARAMS() : name(""), width(0), height(0), fullscreen(false), x(0), y(0) {};
			gWINDOWPARAMS(gString _name, unsigned short _width, unsigned short _height,
				bool _fullscreen = false, unsigned _x = 0, unsigned short _y = 0) :
				name(_name), width(_width), height(_height), fullscreen(_fullscreen), x(_x), y(_y) {}

			gString name;
			unsigned short width;
			unsigned short height;
			unsigned short x;
			unsigned short y;
			bool fullscreen;
			void* handle;
		};

		IWindow(const gWINDOWPARAMS& parameters) {};
		~IWindow() {};

		// window
		virtual void showWindow(bool show) = 0;
		virtual void setWindowParameters(const gWINDOWPARAMS& parameters) = 0;
		virtual const gWINDOWPARAMS& getWindowParameters() const = 0;
		virtual bool updateWindow() = 0;
	protected:
		IWindow() {};
		IWindow(const IWindow&) {};
	};

	virtual ~IPlatform() {};

	// filesystem : files
	virtual SPFILE openFile(const gString& filename, bool writeable, bool binary = false, bool addAtEnd = false) const = 0;
	virtual SPFILE openFile(const char* filename, bool writeable, bool binary = false, bool addAtEnd = false) const = 0;

	virtual bool isFileExist( const gString& fileName ) const = 0;
	virtual bool deleteFile( const gString& fileName ) const = 0;
	virtual bool moveFile( const gString& srcName, const gString& dstName ) const = 0;
	virtual bool copyFile( const gString& srcName, const gString& dstName ) const = 0;

	// filesystem : Folders
	virtual bool isFolderExist( const gString& folderName ) const = 0;
	virtual bool createFolder( const gString& folderName )  const  = 0;
	virtual bool deleteFolder( const gString& folderName ) const = 0;

	virtual const gString& getEngineFolder() const = 0;
	virtual const gString& getCurrentFolder() const = 0;
	virtual void setCurrentFolder( const gString& Folder ) = 0;

	// filesystem : list enumerators
	virtual SPLISTENUMERATOR getFolderContentEnumerator( const gString& folderName, const gString& filter)  const = 0;
	virtual SPLISTENUMERATOR getSubFoldersEnumerator( const gString& folderName)  const = 0;

	// filesystem : dialogs
	virtual bool getOpenFileDialog( gString& inoutName, const gString& filter ) const = 0;
	virtual bool getSaveFileDialog( gString& inoutName, const gString& filter) const = 0;
	virtual bool getDirectoryDialog( gString& inoutDir ) const = 0;

	//window
	virtual IWindow* getWindow() = 0;

	virtual ePLATFORMTYPE getPlatformType() const = 0;
	virtual bool runMainLoop( bool* renderEnded, bool* renderRunned ) = 0;

protected:
	IPlatform() {};
	IPlatform(const IPlatform&) {}
};

class IResources : public ISystem
{
public:
	virtual ~IResources() {};
protected:
	IResources() {};
	IResources(IResources&) {};
};

class IInput : public ISystem
{
public:
	virtual ~IInput() {};
protected:
	IInput() {};
	IInput(IInput&) {};
};

class IGraphics : public ISystem
{
public:
	struct gRenderElement
	{

	};

	class IRenderQueue
	{
	public:
		~IRenderQueue() {}

		virtual void add(gRenderElement re) = 0;
		virtual void execute() = 0;
	protected:
		IRenderQueue() {}
	};

	typedef std::shared_ptr< IGraphics::IRenderQueue> SPRENDERQUEUE;

	virtual ~IGraphics() {};
	virtual SPRENDERQUEUE getRenderQueue() = 0;

protected:
	IGraphics() {};
	IGraphics(IGraphics&) {};
};

class ISounds : public ISystem
{
public:
	virtual ~ISounds() {};
protected:
	ISounds() {};
	ISounds(const 	ISounds&) {};
};

class IPhysics : public ISystem
{
public:
	virtual ~IPhysics() {};
protected:
	IPhysics() {};
	IPhysics( const IPhysics&) {};

};

class ISceneGraph : public ISystem
{
public:
	virtual ~ISceneGraph() {};
protected:
	ISceneGraph() {};
	ISceneGraph(const ISceneGraph&) {};

};

class IUserInterface : public ISystem
{
public:
	virtual ~IUserInterface() {};
protected:
	IUserInterface() {};
	IUserInterface(const IUserInterface&) {};
};

class IScripts : public ISystem
{
public:
	virtual ~IScripts() {};
protected:
	IScripts() {};
	IScripts(const IScripts&) {};
};

class ILogger : public ISystem
{
public:
	virtual ~ILogger() {};

	enum class eLoggerLevel
	{
		GLL_MESSAGE,
		GLL_WARNING,
		GLL_ERROR,
	};

	virtual eLoggerLevel getLoggerMinLevel() = 0;
	virtual void setLoggerMinLevel(eLoggerLevel level) = 0;

	virtual void logMessage(const gString& str) = 0;
	virtual void logWarning(const gString& str) = 0;
	virtual void logError(const gString& str) = 0;

	//virtual void logMessage(const char* fmt, ...) = 0;
	//virtual void logWarning(const char* fmt, ...) = 0;
	//virtual void logError(const char* fmt, ...) = 0;

protected:
	ILogger() {};
	ILogger(const ILogger&) {};
};

class IConfiguration : public ISystem
{
public:
	virtual ~IConfiguration() {};
protected:
	IConfiguration() {};
};

typedef std::shared_ptr<IPlatform> SPPLATFORM;
typedef std::shared_ptr<IResources> SPRESOURCES;
typedef std::shared_ptr<IInput> SPINPUT;
typedef std::shared_ptr<IGraphics> SPGRAPHICS;
typedef std::shared_ptr<ISounds> SPSOUNDS;
typedef std::shared_ptr<IPhysics> SPPHYSICS;
typedef std::shared_ptr<ISceneGraph> SPSCENEGRAPH;
typedef std::shared_ptr<IUserInterface> SPUSERINTERFACE;
typedef std::shared_ptr<IScripts> SPSCRIPTS;
typedef std::shared_ptr<ILogger> SPLOGGER;
typedef std::shared_ptr<IConfiguration> SPCONFIGURATION;

typedef std::weak_ptr<IPlatform> WPPLATFORM;
typedef std::weak_ptr<IResources> WPRESOURCES;
typedef std::weak_ptr<IInput> WPINPUT;
typedef std::weak_ptr<IGraphics> WPGRAPHICS;
typedef std::weak_ptr<ISounds> WPSOUNDS;
typedef std::weak_ptr<IPhysics> WPPHYSICS;
typedef std::weak_ptr<ISceneGraph> WPSCENEGRAPH;
typedef std::weak_ptr<IUserInterface> WPUSERINTERFACE;
typedef std::weak_ptr<IScripts> WPSCRIPTS;
typedef std::weak_ptr<ILogger> WPLOGGER;
typedef std::weak_ptr<IConfiguration> WPCONFIGURATION;


#endif
