#ifdef _WIN32

#include "PlatformWin.h"
#include "File.h"
#include "Engine.h"
#include <assert.h>
#include <functional>
#include <windows.h>


#define WINDOW_STYLE ( WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX | WS_CAPTION )

// ------------------------------------
//
//		*** class gPlatformWin::gWindow ***
//
// ------------------------------------

gPlatformWin::gWindow::gWindow()
{
	m_parameters;
}

gPlatformWin::gWindow::~gWindow()
{
	if (m_parameters.handle)
		DestroyWindow((HWND)m_parameters.handle);
	UnregisterClass("3RDE_WND_CLS", GetModuleHandle(0));
}

// window
void gPlatformWin::gWindow::showWindow(bool show)
{
	ShowWindow( (HWND)m_parameters.handle, show ?  SW_SHOW : SW_HIDE);
	updateWindow();
}

void gPlatformWin::gWindow::setWindowParameters( const gWINDOWPARAMS& parameters )
{
	m_parameters = parameters;
	RECT rect;
	adjustRect(&rect);

	HWND hWnd = static_cast<HWND>(m_parameters.handle);

	if ( !parameters.fullscreen )
	{
		SetWindowPos( hWnd, 0, rect.left, rect.top, rect.right, rect.bottom, SWP_NOZORDER);
	}
	else
	{   // ignore x, y window coord in fullscreen
		SetWindowPos( hWnd, 0, 0, 0, rect.right, rect.bottom, SWP_NOZORDER);
	}

	SetWindowText(hWnd, m_parameters.name.c_str());
}

const gPlatformWin::IWindow::gWINDOWPARAMS& gPlatformWin::gWindow::getWindowParameters() const
{
	return this->m_parameters;
}

bool gPlatformWin::gWindow::updateWindow()
{
	return UpdateWindow( static_cast<HWND>(m_parameters.handle) );
}

#ifdef _WIN64 
typedef __int64 TLPARAM;
typedef unsigned __int64 TWPARAM;
#else
typedef long TLPARAM;
typedef unsigned int TWPARAM;
#endif
typedef TLPARAM TLRESULT;

//wndproc
LRESULT WINAPI _wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}


bool gPlatformWin::gWindow::createWindow( const gWINDOWPARAMS& parameters )
{
	m_parameters = parameters;

	HWND hWnd = 0;

	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, _wndProc, 0L, 0L,
				  GetModuleHandle(NULL), NULL, LoadCursor(NULL, IDC_ARROW), NULL, NULL,
				  "3RDE_WND_CLS", NULL };

	ECHECK(RegisterClassEx(&wc), "Ошибка при регистрации класса окна!");

	RECT rect;
	adjustRect(&rect);

	hWnd = CreateWindowEx(0, "3RDE_WND_CLS", m_parameters.name.c_str(), WINDOW_STYLE, 
		0, 0, rect.right, rect.bottom, 0, 0, 0, 0);

	ECHECK( hWnd, "Ошибка при создании окна!" );

	m_parameters.handle = static_cast<void*>(hWnd);

	return true;
}

void gPlatformWin::gWindow::adjustRect( void* _rect )
{
	DWORD wStyle = WINDOW_STYLE;
	RECT rect;
	rect.left = m_parameters.x;
	rect.top = m_parameters.y;
	rect.right = m_parameters.width + m_parameters.x;
	rect.bottom = m_parameters.height + m_parameters.y;
	AdjustWindowRect(&rect, wStyle, false);

	*static_cast<RECT*>(_rect) = rect;
}

// ------------------------------------
//
//		*** class gPlatformWin ***
//
// ------------------------------------
gPlatformWin::gPlatformWin()
{
	char path[MAX_PATH];
	GetCurrentDirectory(sizeof(path), path); 

	m_currentFolderName = m_engineFolderName = path;
}

gPlatformWin::~gPlatformWin() 
{

}

// filesystem : files
SPFILE gPlatformWin::openFile(const gString& filename, bool writeable, bool binary, bool addAtEnd ) const
{
	SPFILE spFile = std::make_shared<gFile>(filename, writeable, binary, addAtEnd);
	return spFile;
}

SPFILE gPlatformWin::openFile(const char* filename, bool writeable, bool binary, bool addAtEnd) const
{
	SPFILE spFile = std::make_shared<gFile>(filename, writeable, binary, addAtEnd);
	return spFile;
}

bool gPlatformWin::isFileExist( const gString& fileName) const
{
	return false;
}

bool gPlatformWin::deleteFile(const gString& fileName) const
{
	return false;
}

bool gPlatformWin::moveFile(const gString& srcName, const gString& dstName) const
{
	return false;
}

bool gPlatformWin::copyFile(const gString& srcName, const gString& dstName) const
{
	return false;
}

// filesystem : Folders
bool gPlatformWin::isFolderExist(const gString& folderName) const
{
	return false;
}

bool gPlatformWin::createFolder(const gString& folderName)  const
{
	return false;
}

bool gPlatformWin::deleteFolder(const gString& folderName) const
{
	return false;
}

const gString& gPlatformWin::getEngineFolder() const
{
	return m_engineFolderName;
}

const gString& gPlatformWin::getCurrentFolder() const
{
	return m_currentFolderName;
}

void gPlatformWin::setCurrentFolder(const gString& folder)
{
	SetCurrentDirectory( folder.c_str() );
}

// filesystem : list enumerators
SPLISTENUMERATOR gPlatformWin::getFolderContentEnumerator(const gString& folderName, const gString& filter)  const
{
	SPLISTENUMERATOR spList;
	return spList;
}

SPLISTENUMERATOR gPlatformWin::getSubFoldersEnumerator( const gString& folderName)  const
{
	SPLISTENUMERATOR spList;
	return spList;
}

// filesystem : dialogs
bool gPlatformWin::getOpenFileDialog(gString& inoutName, const gString& filter) const
{
	return false;
}

bool gPlatformWin::getSaveFileDialog(gString& inoutName, const gString& filter) const
{
	return false;
}

bool gPlatformWin::getDirectoryDialog(gString& inoutDir) const
{
	return false;
}

// window
gPlatformWin::IWindow* gPlatformWin::getWindow()
{
	return &m_window;
}

IPlatform::ePLATFORMTYPE gPlatformWin::getPlatformType() const
{
	return IPlatform::ePLATFORMTYPE::PT_WIN;
}

bool gPlatformWin::initialize()
{
	m_window.createWindow(IPlatform::IWindow::gWINDOWPARAMS("3RDE_TEST", 800, 600));
	return true;
}

bool gPlatformWin::runMainLoop()
{
	MSG msg = { 0 };
	while (true)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				//cleanUp();
				return true;
			};

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//if (frame_move())
		//	frame_render();
	}
}

bool gPlatformWin::finalize()
{
	return true;
}

#endif