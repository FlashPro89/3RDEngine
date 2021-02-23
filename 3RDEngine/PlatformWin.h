#pragma once

#ifndef _3RDE_PLATFORM_WIN_
#define _3RDE_PLATFORM_WIN_

#include "Interfaces.h"

class gPlatformWin : public IPlatform
{
public:

	// ***  make friendly to class gWindow;
	bool initialize();

	class gWindow : public IWindow
	{
	public:
		gWindow();
		~gWindow();

		// window
		void* getWindowHanlde(); // return hWnd casted to pointer
		void showWindow(bool show);
		void setWindowParameters(const gWINDOWPARAMS& parameters);
		const gWINDOWPARAMS& getWindowParameters() const;
		bool updateWindow();

	protected:
		gWindow(const gWindow&) {};

		bool createWindow(const gWINDOWPARAMS& parameters);
		void adjustRect(void* rect);

		gWINDOWPARAMS m_parameters;
		void* m_wHandle;

		friend 	bool gPlatformWin::initialize();
	};

	gPlatformWin();
	~gPlatformWin();

	// filesystem : files
	SPFILE openFile(const gString& filename, bool writeable, bool binary, bool addAtEnd ) const;
	SPFILE openFile(const char* filename, bool writeable, bool binary = false, bool addAtEnd = false) const;

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

	//window
	IWindow* getWindow();

	IPlatform::ePLATFORMTYPE getPlatformType() const;

	bool runMainLoop();

protected:
	gPlatformWin( const gPlatformWin& other ) {};

	bool finalize();

	gWindow m_window;
	gString m_engineFolderName;
	gString m_currentFolderName;
};

#endif

