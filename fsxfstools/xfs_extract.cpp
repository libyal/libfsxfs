/*
 * fsxfsextract - mounts an XFS filesystem image using fsxfsmount.exe
 * (launched in the background), copies all mounted content to an output
 * directory, then terminates the mount process and cleans up.
 *
 * Usage: fsxfsextract [-v] <xfs_image> <output_dir> [path_to_fsxfsmount.exe]
 *
 * Options:
 *   -v             Enable verbose logging to log.txt (next to the executable)
 *   -h, --help     Show this help message
 *
 * If fsxfsmount.exe is not specified it is looked up next to fsxfsextract.exe.
 */

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <tchar.h>

/* -------------------------------------------------------------------------
 * Logging
 * ---------------------------------------------------------------------- */

static FILE *g_log = NULL;

static void fsxfsextract_log_open(
    void )
{
    wchar_t log_path[ MAX_PATH ];
    DWORD len = GetModuleFileName( NULL, log_path, MAX_PATH );
    if( len == 0 )
    {
        return;
    }
    wchar_t *last_slash = wcsrchr( log_path, L'\\' );
    if( last_slash != NULL )
    {
        *( last_slash + 1 ) = L'\0';
    }
    wcsncat( log_path, L"log.txt", MAX_PATH - (DWORD)wcslen( log_path ) - 1 );
    _tfopen_s( &g_log, log_path, L"a" );
}

static void fsxfsextract_log_close(
    void )
{
    if( g_log != NULL )
    {
        fclose( g_log );
        g_log = NULL;
    }
}

static void fsxfsextract_log(
    const wchar_t *fmt, ... )
{
    if( g_log == NULL )
    {
        return;
    }
    SYSTEMTIME st;
    GetLocalTime( &st );
    _ftprintf( g_log, L"[%04d-%02d-%02d %02d:%02d:%02d] ",
               st.wYear, st.wMonth, st.wDay,
               st.wHour, st.wMinute, st.wSecond );
    va_list args;
    va_start( args, fmt );
    _vftprintf( g_log, fmt, args );
    va_end( args );
    _ftprintf( g_log, L"\n" );
    fflush( g_log );
}

/* -------------------------------------------------------------------------
 * Internal helpers
 * ---------------------------------------------------------------------- */

static int fsxfsextract_ensure_directory(
    const wchar_t *path )
{
    if( CreateDirectory( path, NULL ) )
    {
        return( 0 );
    }
    if( GetLastError() == ERROR_ALREADY_EXISTS )
    {
        return( 0 );
    }
    return( -1 );
}

/* Recursively copy src_dir into dst_dir (dst_dir is created if needed). */
static int fsxfsextract_copy_directory(
    const wchar_t *src_dir,
    const wchar_t *dst_dir )
{
    wchar_t search_path[ MAX_PATH ];
    wchar_t src_path[ MAX_PATH ];
    wchar_t dst_path[ MAX_PATH ];
    WIN32_FIND_DATA fd;
    HANDLE hFind;
    int result = 0;

    if( fsxfsextract_ensure_directory( dst_dir ) != 0 )
    {
        return( -1 );
    }

    _sntprintf( search_path, MAX_PATH, L"%s\\*", src_dir );
    hFind = FindFirstFile( search_path, &fd );
    if( hFind == INVALID_HANDLE_VALUE )
    {
        /* Empty or inaccessible directory – not a fatal error. */
        return( 0 );
    }

    do
    {
        if( wcscmp( fd.cFileName, L"." ) == 0
         || wcscmp( fd.cFileName, L".." ) == 0 )
        {
            continue;
        }

        _sntprintf( src_path, MAX_PATH, L"%s\\%s", src_dir, fd.cFileName );
        _sntprintf( dst_path, MAX_PATH, L"%s\\%s", dst_dir, fd.cFileName );

        if( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
        {
            if( fsxfsextract_copy_directory( src_path, dst_path ) != 0 )
            {
                result = -1;
            }
        }
        else
        {
            if( !CopyFile( src_path, dst_path, FALSE ) )
            {
                result = -1;
            }
        }
    }
    while( FindNextFile( hFind, &fd ) );

    FindClose( hFind );
    return( result );
}

/* Best-effort recursive directory removal (temp mount point cleanup). */
static void fsxfsextract_remove_directory(
    const wchar_t *path )
{
    wchar_t search_path[ MAX_PATH ];
    wchar_t child_path[ MAX_PATH ];
    WIN32_FIND_DATA fd;
    HANDLE hFind;

    _sntprintf( search_path, MAX_PATH, L"%s\\*", path );
    hFind = FindFirstFile( search_path, &fd );
    if( hFind != INVALID_HANDLE_VALUE )
    {
        do
        {
            if( wcscmp( fd.cFileName, L"." ) == 0
             || wcscmp( fd.cFileName, L".." ) == 0 )
            {
                continue;
            }

            _sntprintf( child_path, MAX_PATH, L"%s\\%s", path, fd.cFileName );

            if( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
            {
                fsxfsextract_remove_directory( child_path );
            }
            else
            {
                SetFileAttributes( child_path, FILE_ATTRIBUTE_NORMAL );
                DeleteFile( child_path );
            }
        }
        while( FindNextFile( hFind, &fd ) );

        FindClose( hFind );
    }

    RemoveDirectory( path );
}

/* -------------------------------------------------------------------------
 * Entry point
 * ---------------------------------------------------------------------- */

int wmain(
    int argc,
    wchar_t *argv[] )
{
    wchar_t mount_exe[ MAX_PATH ];
    wchar_t mount_point[ MAX_PATH ];
    wchar_t temp_dir[ MAX_PATH ];
    wchar_t cmdline[ 32768 ];
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    int result  = 0;
    int verbose = 0;

    /* ------------------------------------------------------------------ */
    /* Parse flags                                                         */
    /* ------------------------------------------------------------------ */
    int first_pos = 1; /* index of first positional argument */
    for( ; first_pos < argc; first_pos++ )
    {
        if( wcscmp( argv[ first_pos ], L"-v" ) == 0 )
        {
            verbose = 1;
        }
        else if( wcscmp( argv[ first_pos ], L"-h" ) == 0
              || wcscmp( argv[ first_pos ], L"--help" ) == 0 )
        {
            _tprintf(
                L"Usage: fsxfsextract [-v] <xfs_image> <output_dir>"
                L" [path_to_fsxfsmount.exe]\n"
                L"\n"
                L"Options:\n"
                L"  -v             Enable verbose logging to log.txt\n"
                L"  -h, --help     Show this help\n"
                L"\n"
                L"Mounts the XFS image via fsxfsmount.exe (Dokan), copies all\n"
                L"content to output_dir, then unmounts and cleans up.\n"
                L"If fsxfsmount.exe is not specified it is looked up next to\n"
                L"fsxfsextract.exe.\n" );
            return( 0 );
        }
        else
        {
            break; /* first non-flag argument */
        }
    }

    /* Positional: <xfs_image> <output_dir> [fsxfsmount_path] */
    int pos_argc       = argc - first_pos;
    wchar_t **pos_argv = argv + first_pos;

    if( pos_argc < 2 )
    {
        _ftprintf( stderr,
                   L"Error: insufficient arguments.\n"
                   L"Run with -h for help.\n" );
        return( 1 );
    }

    const wchar_t *xfs_image  = pos_argv[ 0 ];
    const wchar_t *output_dir = pos_argv[ 1 ];

    if( verbose )
    {
        fsxfsextract_log_open();
    }

    fsxfsextract_log( L"Image : %s", xfs_image );
    fsxfsextract_log( L"Output: %s", output_dir );

    /* ------------------------------------------------------------------ */
    /* Resolve fsxfsmount.exe path                                         */
    /* ------------------------------------------------------------------ */
    if( pos_argc >= 3 )
    {
        _sntprintf( mount_exe, MAX_PATH, L"%s", pos_argv[ 2 ] );
    }
    else
    {
        /* Default: same directory as this executable. */
        DWORD len = GetModuleFileName( NULL, mount_exe, MAX_PATH );
        if( len == 0 )
        {
            fsxfsextract_log( L"Error: GetModuleFileName failed (%u)", GetLastError() );
            fsxfsextract_log_close();
            return( 1 );
        }

        wchar_t *last_slash = wcsrchr( mount_exe, L'\\' );
        if( last_slash != NULL )
        {
            *( last_slash + 1 ) = L'\0';
        }
        wcsncat( mount_exe, L"fsxfsmount.exe",
                 MAX_PATH - (DWORD)wcslen( mount_exe ) - 1 );
    }

    fsxfsextract_log( L"fsxfsmount: %s", mount_exe );

    /* ------------------------------------------------------------------ */
    /* Create a unique temporary mount point                               */
    /* ------------------------------------------------------------------ */
    if( GetTempPath( MAX_PATH, temp_dir ) == 0 )
    {
        fsxfsextract_log( L"Error: GetTempPath failed (%u)", GetLastError() );
        fsxfsextract_log_close();
        return( 1 );
    }

    _sntprintf( mount_point, MAX_PATH, L"%sfsxfs_mount_%u_%u",
                temp_dir, GetCurrentProcessId(), GetTickCount() );

    if( !CreateDirectory( mount_point, NULL ) )
    {
        fsxfsextract_log( L"Error: cannot create mount point: %s (%u)", mount_point, GetLastError() );
        fsxfsextract_log_close();
        return( 1 );
    }

    fsxfsextract_log( L"Mount point: %s", mount_point );

    /* ------------------------------------------------------------------ */
    /* Launch fsxfsmount in the background                                 */
    /* ------------------------------------------------------------------ */
    _sntprintf( cmdline, 32768, L"\"%s\" \"%s\" \"%s\"",
                mount_exe, xfs_image, mount_point );

    ZeroMemory( &si, sizeof( si ) );
    si.cb = sizeof( si );
    ZeroMemory( &pi, sizeof( pi ) );

    si.dwFlags    |= STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    if( !CreateProcess( NULL, cmdline,
                       NULL, NULL,
                       FALSE,
                       CREATE_NO_WINDOW,
                       NULL, NULL,
                       &si, &pi ) )
    {
        fsxfsextract_log( L"Error: failed to launch fsxfsmount (%u)", GetLastError() );
        fsxfsextract_remove_directory( mount_point );
        fsxfsextract_log_close();
        return( 1 );
    }

    CloseHandle( pi.hThread );
    fsxfsextract_log( L"Mounted (PID %u) - waiting for filesystem to become ready...", pi.dwProcessId );

    /* Give Dokan time to initialise the mount. */
    Sleep( 2000 );

    /* ------------------------------------------------------------------ */
    /* Copy files                                                          */
    /* ------------------------------------------------------------------ */
    fsxfsextract_log( L"Copying files..." );
    result = fsxfsextract_copy_directory( mount_point, output_dir );
    fsxfsextract_log( L"Copy %s", result == 0 ? L"complete" : L"completed with warnings" );

    /* ------------------------------------------------------------------ */
    /* Unmount: terminate fsxfsmount                                       */
    /* ------------------------------------------------------------------ */
    fsxfsextract_log( L"Unmounting (PID %u)...", pi.dwProcessId );
    TerminateProcess( pi.hProcess, 0 );
    WaitForSingleObject( pi.hProcess, 5000 );
    CloseHandle( pi.hProcess );

    /* Brief pause to allow the driver to release the mount point. */
    Sleep( 2000 );

    /* ------------------------------------------------------------------ */
    /* Clean up temporary mount point                                      */
    /* ------------------------------------------------------------------ */
    fsxfsextract_remove_directory( mount_point );
    fsxfsextract_log( L"Done (exit code 0)" );
    fsxfsextract_log_close();

    return( 0 );
}
