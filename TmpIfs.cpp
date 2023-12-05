// TmpIfs.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#undef max
#include <algorithm>

#include <ShlwApi.h>

#include <Wininet.h>
#pragma comment( lib, "Wininet.lib")

CWinApp theApp;

using std::cout;
using std::cerr;

// Column output delimiter
#define chSep '\t'
#define strSep "\t"

constexpr char* szHeader{
	"Internet Address" strSep
	"Size" strSep
	"Last Modified" strSep
	"Expires" strSep
	"Last Accessed" strSep
	"Last Checked" strSep
	//	"Use Count" strSep
	"Hit Rate\n"
};

static void OutputTime( const FILETIME & ft, bool bForNoneDisplayNever )
{
	cout << chSep;

	if ( ( ft.dwHighDateTime != 0 ) || ( ft.dwLowDateTime != 0 ) )
	{
		FILETIME lft;
		FileTimeToLocalFileTime( &ft, &lft );

		SYSTEMTIME st;
		FileTimeToSystemTime( &lft, &st );

		TCHAR szDateTime[40];

		GetDateFormat( LOCALE_USER_DEFAULT, DATE_SHORTDATE, &st, NULL, szDateTime, sizeof( szDateTime ) );
		cout << szDateTime;
		cout << ' ';

		GetTimeFormat( LOCALE_USER_DEFAULT, 0, &st, NULL, szDateTime, sizeof( szDateTime ) );
		cout << szDateTime;
	}
	else
	{
		cout << (bForNoneDisplayNever ? "Never" : "None");
	}
}

static void OutputCacheEntry( const INTERNET_CACHE_ENTRY_INFO & CacheEntry )
{
	cout << CacheEntry.lpszSourceUrlName;
	/*
	if ( CacheEntry.lpszLocalFileName != NULL )
	{
	cout << chSep;
	cout << CacheEntry.lpszLocalFileName;
	}
	*/
	TCHAR szSize[40];
	StrFormatByteSize( CacheEntry.dwSizeLow, szSize, sizeof( szSize ) );
	cout << chSep;
	cout << szSize;

	OutputTime( CacheEntry.LastModifiedTime, false );
	OutputTime( CacheEntry.ExpireTime, true );
	OutputTime( CacheEntry.LastAccessTime, false );
	OutputTime( CacheEntry.LastSyncTime, true );

	//cout << chSep;
	//cout << CacheEntry.dwUseCount;
	////				_itoa( CacheEntry.dwUseCount, szSize, 10 );
	////				fputs( szSize, stdout );

	cout << chSep;
	cout << CacheEntry.dwHitRate;
	cout << "\n";
}

int main(int /*argc*/, char* /*argv*/[])
{
	int nRetCode;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		cerr << _T("Fatal Error: MFC initialization failed\n");
		nRetCode = 1;
	}
	else
	{
		/* If COM isn't initialized, things just appear as normal folders! */
		const auto hr = CoInitialize( NULL );

		if ( SUCCEEDED( hr ) )
		{

			cerr << "TmpIfs V1.2.1.0\n";

			// The buffer for the cache entry must be large enough to encompass the largest entry, not just the size of the structure!
			// Code uses CacheEntry type, so map the 2 things together
			union
			{
				BYTE Cache[10'000];
				INTERNET_CACHE_ENTRY_INFO CacheEntry;
			};

			CacheEntry.dwStructSize = sizeof( INTERNET_CACHE_ENTRY_INFO );

			cout << szHeader;

			int NumItems = 0;
#ifdef _DEBUG
			// Debug variable to ensure buffer is large enough
			DWORD MaxEntrySizeEncountered{ 0 };
#endif

			DWORD dwEntrySize{ sizeof( Cache ) };
			HANDLE hFind = FindFirstUrlCacheEntryEx( NULL, 0, NORMAL_CACHE_ENTRY | COOKIE_CACHE_ENTRY, 0, &CacheEntry, &dwEntrySize, NULL, NULL, NULL );
			if ( hFind != NULL )
			{
#ifdef _DEBUG
				MaxEntrySizeEncountered = dwEntrySize;
#endif
				do
				{
					++NumItems;

					OutputCacheEntry( CacheEntry );

					// Reset the buffer size
					dwEntrySize = sizeof( Cache );

					const auto bval = FindNextUrlCacheEntryEx( hFind, &CacheEntry, &dwEntrySize, NULL, NULL, NULL );
					if ( !bval )
					{
#ifdef _DEBUG
						// Check if the above API ever fails (because the buffer is too small)
						auto le = GetLastError();
						if ( le != ERROR_NO_MORE_ITEMS )
						{
							le = le;
						}
#endif
						break;
					}
#ifdef _DEBUG
					else
					{
						MaxEntrySizeEncountered = std::max( MaxEntrySizeEncountered, dwEntrySize );
					}
#endif
				}
				while ( true );

				FindCloseUrlCache( hFind );
			}
#ifdef _DEBUG
			cout << "MAX ENCOUNTERED: " << MaxEntrySizeEncountered << "\n";
#endif
			cerr << "There are " << NumItems << " items\n";

			CoUninitialize();

			nRetCode = 0;
		}
		else
		{
			cerr << _T( "Fatal Error: COM initialization failed\n" );
			nRetCode = 2;
		}
	}

	return nRetCode;
}

