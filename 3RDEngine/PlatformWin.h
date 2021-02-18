#pragma once

#ifndef _3RDE_PLATFORM_WIN_
#define _3RDE_PLATFORM_WIN_

#include "Interfaces.h"

class gPlatformWin :
    public IPlatform
{
public:

	class gWindow : public IWindow
	{
	public:
		gWindow( const gWINDOWPARAMS& parameters );
		~gWindow( );

		// window
		void* getWindowHanlde(); // return weak pointer of hWnd
		void showWindow( bool show );
		void setWindowParameters( const gWINDOWPARAMS& parameters );
		const gWINDOWPARAMS& getWindowParameters( ) const;
		bool updateWindow();

	protected:
		gWindow() {};
		gWindow(const gWindow&) {};

		bool createWindow();
		void adjustRect(void* rect);

		gWINDOWPARAMS m_parameters;
		void* m_wHandle;
	};

	gPlatformWin();
	~gPlatformWin();

	// filesystem : files
	SPFILE openFile(const gString& filename, bool writeable, bool binary = false, bool addAtEnd = false) const;
	bool isFileExist(const gString& fileName) const;
	bool deleteFile(const gString& fileName) const;
	bool moveFile(const gString& srcName, const gString& dstName) const;
	bool copyFile(const gString& srcName, const gString& dstName) const;

	// filesystem : Folders
	bool isFolderExist(const gString& folderName) const;
	bool createFolder(const gString& folderName)  const;
	bool deleteFolder(const gString& folderName) const;

	const gString& getEngineFolder() const;
	const gString& getCurrentFolder() const;
	void setCurrentFolder(const gString& Folder);

	// filesystem : list enumerators
	SPLISTENUMERATOR getFolderContentEnumerator( const gString& folderName, const gString& filter)  const;
	SPLISTENUMERATOR getSubFoldersEnumerator( const gString& folderName)  const;

	// filesystem : dialogs
	bool getOpenFileDialog( gString& inoutName, const gString& filter) const;
	bool getSaveFileDialog( gString& inoutName, const gString& filter) const;
	bool getDirectoryDialog( gString& inoutDir ) const;

	// window
	void* getWindowHanlde();
	void showWindow(bool show);
	void setWindowParameters(const IPlatform::gWINDOWPARAMS& parameters);
	const IPlatform::gWINDOWPARAMS& getWindowParameters() const;

	IPlatform::ePLATFORMTYPE getPlatformType() const;

	bool initialize();

protected:
	gPlatformWin( const gPlatformWin& other ) {};

	bool finalize();

	gString m_engineFolderName;
	gString m_currentFolderName;

	IPlatform::gWINDOWPARAMS m_windowParameters;

};

#endif

