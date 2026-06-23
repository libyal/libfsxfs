/*
 * xfs_extract - mounts an XFS filesystem image using fsxfsmount
 * (launched in the background), copies all mounted content to an
 * output directory, then terminates / unmounts the mount process
 * and cleans up.
 *
 * Cross-platform: Windows (uses Dokan via fsxfsmount.exe) and
 *                 Linux  (uses FUSE  via fsxfsmount).
 *
 * POSIX usage: xfs_extract <xfs_image> <output_dir> [-v]
 *              [-l log_file] [-t temp_folder] [-m path_to_fsxfsmount]
 *
 * Options:
 *   -v             Enable verbose logging to log.txt (next to the executable)
 *   -h, --help     Show this help message
 *
 * If fsxfsmount(.exe) is not specified it is looked up next to xfs_extract,
 * then on $PATH (POSIX only).
 */

/* ======================================================================
 *                              WINDOWS
 * ====================================================================== */
#if defined( _WIN32 ) || defined( WIN32 )

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <tchar.h>

/* ------------------------------ Logging ------------------------------ */

static FILE *g_log = NULL;

static void xfs_extract_log_open(
    void )
{
    wchar_t log_path[ MAX_PATH ];
    DWORD len = GetModuleFileNameW( NULL, log_path, MAX_PATH );
    if( len == 0 )
    {
        return;
    }
    wchar_t *last_slash = wcsrchr( log_path, L'\\' );
    if( last_slash != NULL )
    {
        *( last_slash + 1 ) = L'\0';
    }
    wcsncat( log_path, L"log.txt", MAX_PATH - (DWORD) wcslen( log_path ) - 1 );
    _wfopen_s( &g_log, log_path, L"a" );
}

static void xfs_extract_log_close(
    void )
{
    if( g_log != NULL )
    {
        fclose( g_log );
        g_log = NULL;
    }
}

static void xfs_extract_log(
    const wchar_t *fmt, ... )
{
    if( g_log == NULL )
    {
        return;
    }
    SYSTEMTIME st;
    GetLocalTime( &st );
    fwprintf( g_log, L"[%04d-%02d-%02d %02d:%02d:%02d] ",
              st.wYear, st.wMonth, st.wDay,
              st.wHour, st.wMinute, st.wSecond );
    va_list args;
    va_start( args, fmt );
    vfwprintf( g_log, fmt, args );
    va_end( args );
    fputwc( L'\n', g_log );
    fflush( g_log );
}

/* ------------------------------ Helpers ------------------------------ */

static int xfs_extract_ensure_directory(
    const wchar_t *path )
{
    if( CreateDirectoryW( path, NULL ) )
    {
        return( 0 );
    }
    if( GetLastError() == ERROR_ALREADY_EXISTS )
    {
        return( 0 );
    }
    return( -1 );
}

static int xfs_extract_copy_directory(
    const wchar_t *src_dir,
    const wchar_t *dst_dir )
{
    wchar_t          search_path[ MAX_PATH ];
    wchar_t          src_path[ MAX_PATH ];
    wchar_t          dst_path[ MAX_PATH ];
    WIN32_FIND_DATAW fd;
    HANDLE           hFind;
    int              result = 0;

    if( xfs_extract_ensure_directory( dst_dir ) != 0 )
    {
        return( -1 );
    }
    _snwprintf( search_path, MAX_PATH, L"%s\\*", src_dir );
    hFind = FindFirstFileW( search_path, &fd );
    if( hFind == INVALID_HANDLE_VALUE )
    {
        return( 0 );
    }
    do
    {
        if( wcscmp( fd.cFileName, L"." ) == 0
         || wcscmp( fd.cFileName, L".." ) == 0 )
        {
            continue;
        }
        _snwprintf( src_path, MAX_PATH, L"%s\\%s", src_dir, fd.cFileName );
        _snwprintf( dst_path, MAX_PATH, L"%s\\%s", dst_dir, fd.cFileName );

        if( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
        {
            if( xfs_extract_copy_directory( src_path, dst_path ) != 0 )
            {
                result = -1;
            }
        }
        else
        {
            if( !CopyFileW( src_path, dst_path, FALSE ) )
            {
                result = -1;
            }
        }
    }
    while( FindNextFileW( hFind, &fd ) );

    FindClose( hFind );
    return( result );
}

static void xfs_extract_remove_directory(
    const wchar_t *path )
{
    wchar_t          search_path[ MAX_PATH ];
    wchar_t          child_path[ MAX_PATH ];
    WIN32_FIND_DATAW fd;
    HANDLE           hFind;

    _snwprintf( search_path, MAX_PATH, L"%s\\*", path );
    hFind = FindFirstFileW( search_path, &fd );
    if( hFind != INVALID_HANDLE_VALUE )
    {
        do
        {
            if( wcscmp( fd.cFileName, L"." ) == 0
             || wcscmp( fd.cFileName, L".." ) == 0 )
            {
                continue;
            }
            _snwprintf( child_path, MAX_PATH, L"%s\\%s", path, fd.cFileName );
            if( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
            {
                xfs_extract_remove_directory( child_path );
            }
            else
            {
                SetFileAttributesW( child_path, FILE_ATTRIBUTE_NORMAL );
                DeleteFileW( child_path );
            }
        }
        while( FindNextFileW( hFind, &fd ) );
        FindClose( hFind );
    }
    RemoveDirectoryW( path );
}

/* ------------------------------ Entry point -------------------------- */

int wmain(
    int argc,
    wchar_t *argv[] )
{
    wchar_t             mount_exe[ MAX_PATH ];
    wchar_t             mount_point[ MAX_PATH ];
    wchar_t             temp_dir[ MAX_PATH ];
    wchar_t             cmdline[ 32768 ];
    STARTUPINFOW        si;
    PROCESS_INFORMATION pi;
    int                 result    = 0;
    int                 verbose   = 0;
    int                 first_pos = 1;

    for( ; first_pos < argc; first_pos++ )
    {
        if( wcscmp( argv[ first_pos ], L"-v" ) == 0 )
        {
            verbose = 1;
        }
        else if( wcscmp( argv[ first_pos ], L"-h" ) == 0
              || wcscmp( argv[ first_pos ], L"--help" ) == 0 )
        {
            wprintf(
                L"Usage: xfs_extract [-v] <xfs_image> <output_dir>"
                L" [path_to_fsxfsmount.exe]\n"
                L"\n"
                L"Options:\n"
                L"  -v             Enable verbose logging to log.txt\n"
                L"  -h, --help     Show this help\n"
                L"\n"
                L"Mounts the XFS image via fsxfsmount.exe (Dokan), copies all\n"
                L"content to output_dir, then unmounts and cleans up.\n"
                L"If fsxfsmount.exe is not specified it is looked up next to\n"
                L"xfs_extract.exe.\n" );
            return( 0 );
        }
        else
        {
            break;
        }
    }

    int       pos_argc = argc - first_pos;
    wchar_t **pos_argv = argv + first_pos;

    if( pos_argc < 2 )
    {
        fwprintf( stderr,
                  L"Error: insufficient arguments.\n"
                  L"Run with -h for help.\n" );
        return( 1 );
    }
    const wchar_t *xfs_image  = pos_argv[ 0 ];
    const wchar_t *output_dir = pos_argv[ 1 ];

    if( verbose )
    {
        xfs_extract_log_open();
    }
    xfs_extract_log( L"Image : %s", xfs_image );
    xfs_extract_log( L"Output: %s", output_dir );

    /* Resolve fsxfsmount.exe path. */
    if( pos_argc >= 3 )
    {
        _snwprintf( mount_exe, MAX_PATH, L"%s", pos_argv[ 2 ] );
    }
    else
    {
        DWORD len = GetModuleFileNameW( NULL, mount_exe, MAX_PATH );
        if( len == 0 )
        {
            xfs_extract_log( L"Error: GetModuleFileName failed (%u)", GetLastError() );
            xfs_extract_log_close();
            return( 1 );
        }
        wchar_t *last_slash = wcsrchr( mount_exe, L'\\' );
        if( last_slash != NULL )
        {
            *( last_slash + 1 ) = L'\0';
        }
        wcsncat( mount_exe, L"fsxfsmount.exe",
                 MAX_PATH - (DWORD) wcslen( mount_exe ) - 1 );
    }
    xfs_extract_log( L"fsxfsmount: %s", mount_exe );

    /* Create a unique temporary mount point. */
    if( GetTempPathW( MAX_PATH, temp_dir ) == 0 )
    {
        xfs_extract_log( L"Error: GetTempPath failed (%u)", GetLastError() );
        xfs_extract_log_close();
        return( 1 );
    }
    _snwprintf( mount_point, MAX_PATH, L"%sfsxfs_mount_%u_%u",
                temp_dir, GetCurrentProcessId(), GetTickCount() );

    if( !CreateDirectoryW( mount_point, NULL ) )
    {
        xfs_extract_log( L"Error: cannot create mount point: %s (%u)",
                         mount_point, GetLastError() );
        xfs_extract_log_close();
        return( 1 );
    }
    xfs_extract_log( L"Mount point: %s", mount_point );

    /* Launch fsxfsmount in the background. */
    _snwprintf( cmdline, 32768, L"\"%s\" \"%s\" \"%s\"",
                mount_exe, xfs_image, mount_point );

    ZeroMemory( &si, sizeof( si ) );
    si.cb = sizeof( si );
    ZeroMemory( &pi, sizeof( pi ) );
    si.dwFlags    |= STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    if( !CreateProcessW( NULL, cmdline,
                         NULL, NULL,
                         FALSE,
                         CREATE_NO_WINDOW,
                         NULL, NULL,
                         &si, &pi ) )
    {
        xfs_extract_log( L"Error: failed to launch fsxfsmount (%u)", GetLastError() );
        xfs_extract_remove_directory( mount_point );
        xfs_extract_log_close();
        return( 1 );
    }
    CloseHandle( pi.hThread );
    xfs_extract_log( L"Mounted (PID %u) - waiting for filesystem to become ready...",
                     pi.dwProcessId );

    Sleep( 2000 );

    /* Copy. */
    xfs_extract_log( L"Copying files..." );
    result = xfs_extract_copy_directory( mount_point, output_dir );
    xfs_extract_log( L"Copy %s", result == 0 ? L"complete" : L"completed with warnings" );

    Sleep( 200000000 );
    /* Unmount. */
    xfs_extract_log( L"Unmounting (PID %u)...", pi.dwProcessId );
    TerminateProcess( pi.hProcess, 0 );
    WaitForSingleObject( pi.hProcess, 5000 );
    CloseHandle( pi.hProcess );

    Sleep( 2000000000 );

    xfs_extract_remove_directory( mount_point );
    xfs_extract_log( L"Done (exit code 0)" );
    xfs_extract_log_close();

    return( 0 );
}

/* ======================================================================
 *                               POSIX
 * ====================================================================== */
#else /* !_WIN32 */

#define _GNU_SOURCE
#define _XOPEN_SOURCE 700
#define _FILE_OFFSET_BITS 64

#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <ftw.h>
#include <libgen.h>
#include <limits.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

/* ------------------------------ Logging ------------------------------ */

static FILE *g_log = NULL;

static void xfs_extract_exe_dir(
    char *out,
    size_t out_size )
{
    ssize_t len = readlink( "/proc/self/exe", out, out_size - 1 );
    if( len <= 0 )
    {
        out[ 0 ] = '\0';
        return;
    }
    out[ len ] = '\0';
    char *last_slash = strrchr( out, '/' );
    if( last_slash != NULL )
    {
        *( last_slash + 1 ) = '\0';
    }
}

static void xfs_extract_log_open(
    const char *log_file_path )
{
    char log_path[ PATH_MAX ];

    if( log_file_path != NULL && log_file_path[ 0 ] != '\0' )
    {
        snprintf( log_path, sizeof( log_path ), "%s", log_file_path );
    }
    else
    {
        xfs_extract_exe_dir( log_path, sizeof( log_path ) );
        if( log_path[ 0 ] == '\0' )
        {
            snprintf( log_path, sizeof( log_path ), "./" );
        }
        strncat( log_path, "log.txt", sizeof( log_path ) - strlen( log_path ) - 1 );
    }
    g_log = fopen( log_path, "a" );
}

static void xfs_extract_log_close(
    void )
{
    if( g_log != NULL )
    {
        fclose( g_log );
        g_log = NULL;
    }
}

static void xfs_extract_log(
    const char *fmt, ... )
{
    if( g_log == NULL )
    {
        return;
    }
    time_t    now = time( NULL );
    struct tm tm_st;
    localtime_r( &now, &tm_st );
    fprintf( g_log, "[%04d-%02d-%02d %02d:%02d:%02d] ",
             tm_st.tm_year + 1900, tm_st.tm_mon + 1, tm_st.tm_mday,
             tm_st.tm_hour, tm_st.tm_min, tm_st.tm_sec );

    va_list args;
    va_start( args, fmt );
    vfprintf( g_log, fmt, args );
    va_end( args );

    fputc( '\n', g_log );
    fflush( g_log );
}

/* ------------------------------ Helpers ------------------------------ */

static int xfs_extract_ensure_directory(
    const char *path )
{
    if( mkdir( path, 0755 ) == 0 )
    {
        return( 0 );
    }
    return( errno == EEXIST ? 0 : -1 );
}

static int xfs_extract_file_exists(
    const char *path )
{
    struct stat st;
    return( stat( path, &st ) == 0 );
}

static int xfs_extract_is_executable(
    const char *path )
{
    return( access( path, X_OK ) == 0 );
}

static int xfs_extract_create_mount_point(
    const char *temp_base,
    char *path,
    size_t path_size )
{
    if( snprintf( path, path_size, "%s/%s", temp_base, "fsxfs_mount_XXXXXX" )
     >= (int) path_size )
    {
        errno = ENAMETOOLONG;
        return( -1 );
    }
    if( mkdtemp( path ) == NULL )
    {
        return( -1 );
    }
    return( 0 );
}

static int xfs_extract_copy_file(
    const char *src,
    const char *dst )
{
    int     in_fd  = -1;
    int     out_fd = -1;
    char    buffer[ 64 * 1024 ];
    ssize_t r;
    int     result = 0;

    in_fd = open( src, O_RDONLY );
    if( in_fd < 0 )
    {
        xfs_extract_log( "Error: unable to open source file %s: %s",
                         src, strerror( errno ) );
        return( -1 );
    }
    out_fd = open( dst, O_WRONLY | O_CREAT | O_TRUNC, 0644 );
    if( out_fd < 0 )
    {
        xfs_extract_log( "Error: unable to open destination file %s: %s",
                         dst, strerror( errno ) );
        close( in_fd );
        return( -1 );
    }
    while( ( r = read( in_fd, buffer, sizeof( buffer ) ) ) > 0 )
    {
        ssize_t off = 0;
        while( off < r )
        {
            ssize_t w = write( out_fd, buffer + off, (size_t) ( r - off ) );
            if( w < 0 )
            {
                if( errno == EINTR )
                {
                    continue;
                }
                xfs_extract_log( "Error: unable to write destination file %s: %s",
                                 dst, strerror( errno ) );
                result = -1;
                goto done;
            }
            off += w;
        }
    }
    if( r < 0 )
    {
        xfs_extract_log( "Error: unable to read source file %s: %s",
                         src, strerror( errno ) );
        result = -1;
    }
done:
    close( in_fd );
    close( out_fd );
    return( result );
}

static int xfs_extract_copy_directory(
    const char *src_dir,
    const char *dst_dir )
{
    DIR           *dir;
    struct dirent *entry;
    char           src_path[ PATH_MAX ];
    char           dst_path[ PATH_MAX ];
    int            result = 0;

    if( xfs_extract_ensure_directory( dst_dir ) != 0 )
    {
        xfs_extract_log( "Error: unable to prepare destination directory %s: %s",
                         dst_dir, strerror( errno ) );
        return( -1 );
    }
    dir = opendir( src_dir );
    if( dir == NULL )
    {
        xfs_extract_log( "Error: unable to open source directory %s: %s",
                         src_dir, strerror( errno ) );
        return( -1 );
    }
    for( ;; )
    {
        struct stat st;

        errno = 0;
        entry = readdir( dir );

        if( entry == NULL )
        {
            break;
        }

        if( strcmp( entry->d_name, "." ) == 0
         || strcmp( entry->d_name, ".." ) == 0 )
        {
            continue;
        }
        snprintf( src_path, sizeof( src_path ), "%s/%s", src_dir, entry->d_name );
        snprintf( dst_path, sizeof( dst_path ), "%s/%s", dst_dir, entry->d_name );

        if( lstat( src_path, &st ) != 0 )
        {
            xfs_extract_log( "Error: unable to stat source path %s: %s",
                             src_path, strerror( errno ) );
            result = -1;
            continue;
        }
        if( S_ISDIR( st.st_mode ) )
        {
            if( xfs_extract_copy_directory( src_path, dst_path ) != 0 )
            {
                result = -1;
            }
        }
        else if( S_ISLNK( st.st_mode ) )
        {
            char    link_target[ PATH_MAX ];
            ssize_t len = readlink( src_path, link_target, sizeof( link_target ) - 1 );
            if( len < 0 )
            {
                xfs_extract_log( "Error: unable to read source symlink %s: %s",
                                 src_path, strerror( errno ) );
                result = -1;
                continue;
            }
            link_target[ len ] = '\0';
            if( symlink( link_target, dst_path ) != 0 && errno != EEXIST )
            {
                xfs_extract_log( "Error: unable to create destination symlink %s: %s",
                                 dst_path, strerror( errno ) );
                result = -1;
            }
        }
        else if( S_ISREG( st.st_mode ) )
        {
            if( xfs_extract_copy_file( src_path, dst_path ) != 0 )
            {
                result = -1;
            }
        }
    }
    if( errno != 0 )
    {
        xfs_extract_log( "Error: directory enumeration failed for %s: %s",
                         src_dir, strerror( errno ) );
        result = -1;
    }
    closedir( dir );
    return( result );
}

static int xfs_extract_unlink_cb(
    const char *fpath,
    const struct stat *sb,
    int typeflag,
    struct FTW *ftwbuf )
{
    (void) sb;
    (void) typeflag;
    (void) ftwbuf;
    remove( fpath );
    return( 0 );
}

static void xfs_extract_remove_directory(
    const char *path )
{
    nftw( path, xfs_extract_unlink_cb, 16, FTW_DEPTH | FTW_PHYS );
}

/* ---------------------------- Mount control -------------------------- */

static pid_t xfs_extract_launch_mount(
    const char *mount_exe,
    const char *image,
    const char *mount_point,
    int verbose )
{
    pid_t pid = fork();
    if( pid < 0 )
    {
        return( -1 );
    }
    if( pid == 0 )
    {
        if( verbose == 0 )
        {
            int devnull = open( "/dev/null", O_RDWR );
            if( devnull >= 0 )
            {
                dup2( devnull, STDOUT_FILENO );
                dup2( devnull, STDERR_FILENO );
                if( devnull > 2 )
                {
                    close( devnull );
                }
            }
        }
        /* -v keeps FUSE in the foreground so we keep ownership of the PID. */
        execl( mount_exe, "fsxfsmount", "-v", image, mount_point, (char *) NULL );
        _exit( 127 );
    }
    return( pid );
}

enum
{
    XFS_EXTRACT_WAIT_FOR_MOUNT_SUCCESS = 0,
    XFS_EXTRACT_WAIT_FOR_MOUNT_TIMEOUT = 1,
    XFS_EXTRACT_WAIT_FOR_MOUNT_EXITED  = 2,
    XFS_EXTRACT_WAIT_FOR_MOUNT_ERROR   = 3
};

static int xfs_extract_wait_for_mount(
    const char *mount_point,
    pid_t mount_pid,
    int timeout_seconds,
    int *mount_status )
{
    struct stat parent_st;
    struct stat mp_st;
    char        parent[ PATH_MAX ];

    snprintf( parent, sizeof( parent ), "%s/..", mount_point );
    if( stat( parent, &parent_st ) != 0 )
    {
        return( XFS_EXTRACT_WAIT_FOR_MOUNT_ERROR );
    }
    for( int i = 0; i < timeout_seconds * 10; i++ )
    {
        int   status = 0;
        pid_t result = waitpid( mount_pid, &status, WNOHANG );

        if( result == mount_pid )
        {
            if( mount_status != NULL )
            {
                *mount_status = status;
            }
            return( XFS_EXTRACT_WAIT_FOR_MOUNT_EXITED );
        }
        if( result < 0 )
        {
            return( XFS_EXTRACT_WAIT_FOR_MOUNT_ERROR );
        }
        if( stat( mount_point, &mp_st ) == 0 )
        {
            if( mp_st.st_dev != parent_st.st_dev )
            {
                return( XFS_EXTRACT_WAIT_FOR_MOUNT_SUCCESS );
            }
        }
        usleep( 100 * 1000 );
    }
    return( XFS_EXTRACT_WAIT_FOR_MOUNT_TIMEOUT );
}

static void xfs_extract_unmount(
    const char *mount_point,
    pid_t mount_pid )
{
    pid_t pid = fork();
    if( pid == 0 )
    {
        execlp( "fusermount", "fusermount", "-u", mount_point, (char *) NULL );
        execlp( "fusermount3", "fusermount3", "-u", mount_point, (char *) NULL );
        execlp( "umount", "umount", mount_point, (char *) NULL );
        _exit( 127 );
    }
    else if( pid > 0 )
    {
        int status = 0;
        waitpid( pid, &status, 0 );
    }
    for( int i = 0; i < 50; i++ )
    {
        int   status = 0;
        pid_t r      = waitpid( mount_pid, &status, WNOHANG );
        if( r == mount_pid || r < 0 )
        {
            return;
        }
        usleep( 100 * 1000 );
    }
    kill( mount_pid, SIGTERM );
    waitpid( mount_pid, NULL, 0 );
}

static int xfs_extract_resolve_mount_exe(
    const char *explicit_path,
    char *out,
    size_t out_size )
{
    if( explicit_path != NULL && explicit_path[ 0 ] != '\0' )
    {
        snprintf( out, out_size, "%s", explicit_path );
        return( xfs_extract_is_executable( out ) ? 0 : -1 );
    }
    char exe_dir[ PATH_MAX ];
    xfs_extract_exe_dir( exe_dir, sizeof( exe_dir ) );
    if( exe_dir[ 0 ] != '\0' )
    {
        snprintf( out, out_size, "%sfsxfsmount", exe_dir );
        if( xfs_extract_is_executable( out ) )
        {
            return( 0 );
        }
    }
    const char *path_env = getenv( "PATH" );
    if( path_env != NULL )
    {
        const char *p = path_env;
        while( *p != '\0' )
        {
            const char *sep = strchr( p, ':' );
            size_t      len = ( sep != NULL ) ? (size_t) ( sep - p ) : strlen( p );
            if( len > 0 && len < out_size - 16 )
            {
                memcpy( out, p, len );
                out[ len ] = '\0';
                strncat( out, "/fsxfsmount", out_size - strlen( out ) - 1 );
                if( xfs_extract_is_executable( out ) )
                {
                    return( 0 );
                }
            }
            if( sep == NULL )
            {
                break;
            }
            p = sep + 1;
        }
    }
    snprintf( out, out_size, "fsxfsmount" );
    return( -1 );
}

/* ------------------------------ Entry point -------------------------- */

static void xfs_extract_usage(
    FILE *stream )
{
    fprintf( stream,
        "Usage: xfs_extract <xfs_image> <output_dir> [-v] [-l log_file] [-t temp_folder] [-m path_to_fsxfsmount]\n"
        "\n"
        "Options:\n"
        "  -v             Enable verbose logging\n"
        "  -h, --help     Show this help\n"
        "  -l log_file    Write logs to the specified file\n"
        "  --log-file log_file\n"
        "                 Same as -l\n"
        "  -t temp_folder Override the temporary mount base folder\n"
        "  --temp-folder temp_folder\n"
        "                 Same as -t\n"
        "  -m mount_path  Use the specified fsxfsmount executable\n"
        "  --mount mount_path\n"
        "                 Same as -m\n"
        "\n"
        "Mounts the XFS image via fsxfsmount (FUSE), copies all content to\n"
        "output_dir, then unmounts and cleans up.\n"
        "If fsxfsmount is not specified it is looked up next to xfs_extract\n"
        "and then on $PATH.\n" );
}

int main(
    int argc,
    char *argv[] )
{
    char       mount_exe[ PATH_MAX ];
    char       mount_point[ PATH_MAX ];
    const char *explicit_mount = NULL;
    const char *log_file_path  = NULL;
    const char *temp_base      = NULL;
    int        mount_status    = 0;
    int        mount_wait      = XFS_EXTRACT_WAIT_FOR_MOUNT_ERROR;
    int        mounted         = 0;
    int        result          = 0;
    int        verbose         = 0;
    pid_t      mount_pid       = -1;

    if( argc == 2
     && ( strcmp( argv[ 1 ], "-h" ) == 0
       || strcmp( argv[ 1 ], "--help" ) == 0 ) )
    {
        xfs_extract_usage( stdout );
        return( 0 );
    }
    if( argc < 3 )
    {
        fprintf( stderr,
                 "Error: insufficient arguments.\n"
                 "Run with -h for help.\n" );
        return( 1 );
    }

    const char *xfs_image  = argv[ 1 ];
    const char *output_dir = argv[ 2 ];

    for( int argument_index = 3; argument_index < argc; argument_index++ )
    {
        if( strcmp( argv[ argument_index ], "-v" ) == 0 )
        {
            verbose = 1;
        }
        else if( strcmp( argv[ argument_index ], "-h" ) == 0
              || strcmp( argv[ argument_index ], "--help" ) == 0 )
        {
            xfs_extract_usage( stdout );
            return( 0 );
        }
        else if( strcmp( argv[ argument_index ], "-l" ) == 0
              || strcmp( argv[ argument_index ], "--log-file" ) == 0 )
        {
            const char *option_name = argv[ argument_index ];

            argument_index++;

            if( argument_index >= argc )
            {
                fprintf( stderr, "Error: missing value for %s.\n", option_name );
                return( 1 );
            }
            log_file_path = argv[ argument_index ];
            verbose       = 1;
        }
        else if( strcmp( argv[ argument_index ], "-t" ) == 0
              || strcmp( argv[ argument_index ], "--temp-folder" ) == 0
              || strcmp( argv[ argument_index ], "--temp-path" ) == 0 )
        {
            const char *option_name = argv[ argument_index ];

            argument_index++;

            if( argument_index >= argc )
            {
                fprintf( stderr, "Error: missing value for %s.\n", option_name );
                return( 1 );
            }
            temp_base = argv[ argument_index ];
        }
        else if( strcmp( argv[ argument_index ], "-m" ) == 0
              || strcmp( argv[ argument_index ], "--mount" ) == 0 )
        {
            const char *option_name = argv[ argument_index ];

            argument_index++;

            if( argument_index >= argc )
            {
                fprintf( stderr, "Error: missing value for %s.\n", option_name );
                return( 1 );
            }
            explicit_mount = argv[ argument_index ];
        }
        else if( explicit_mount == NULL )
        {
            explicit_mount = argv[ argument_index ];
        }
        else
        {
            fprintf( stderr, "Error: invalid argument: %s\n", argv[ argument_index ] );
            return( 1 );
        }
    }

    if( verbose != 0 )
    {
        xfs_extract_log_open( log_file_path );
    }
    xfs_extract_log( "Image : %s", xfs_image );
    xfs_extract_log( "Output: %s", output_dir );
    if( log_file_path != NULL )
    {
        xfs_extract_log( "Log file: %s", log_file_path );
    }

    if( !xfs_extract_file_exists( xfs_image ) )
    {
        fprintf( stderr, "Error: image not found: %s\n", xfs_image );
        xfs_extract_log( "Error: image not found" );
        xfs_extract_log_close();
        return( 1 );
    }
    if( xfs_extract_resolve_mount_exe( explicit_mount, mount_exe, sizeof( mount_exe ) ) != 0 )
    {
        fprintf( stderr, "Error: fsxfsmount not found (looked next to xfs_extract"
                         " and on $PATH). Pass the path with -m/--mount or as the optional final argument.\n" );
        xfs_extract_log( "Error: fsxfsmount not found, tried: %s", mount_exe );
        xfs_extract_log_close();
        return( 1 );
    }
    xfs_extract_log( "fsxfsmount: %s", mount_exe );

    const char *tmp = temp_base;
    if( tmp == NULL || tmp[ 0 ] == '\0' )
    {
        tmp = getenv( "TMPDIR" );
        if( tmp == NULL || tmp[ 0 ] == '\0' )
        {
            tmp = "/tmp";
        }
    }
    xfs_extract_log( "Temp base: %s", tmp );
    if( xfs_extract_create_mount_point( tmp, mount_point, sizeof( mount_point ) ) != 0 )
    {
        fprintf( stderr, "Error: cannot create mount point %s: %s\n",
                 tmp, strerror( errno ) );
        xfs_extract_log( "Error: create mount point failed in %s: %s",
                         tmp, strerror( errno ) );
        xfs_extract_log_close();
        return( 1 );
    }
    xfs_extract_log( "Mount point: %s", mount_point );

    if( xfs_extract_ensure_directory( output_dir ) != 0 )
    {
        fprintf( stderr, "Error: cannot create output dir %s: %s\n",
                 output_dir, strerror( errno ) );
        rmdir( mount_point );
        xfs_extract_log_close();
        return( 1 );
    }

    mount_pid = xfs_extract_launch_mount( mount_exe, xfs_image, mount_point, verbose );
    if( mount_pid < 0 )
    {
        fprintf( stderr, "Error: failed to fork fsxfsmount: %s\n", strerror( errno ) );
        xfs_extract_log( "Error: fork failed" );
        rmdir( mount_point );
        xfs_extract_log_close();
        return( 1 );
    }
    xfs_extract_log( "Mounted (PID %d) - waiting for filesystem...", (int) mount_pid );

    mount_wait = xfs_extract_wait_for_mount(
                  mount_point,
                  mount_pid,
                  10,
                  &mount_status );

    if( mount_wait != XFS_EXTRACT_WAIT_FOR_MOUNT_SUCCESS )
    {
        if( mount_wait == XFS_EXTRACT_WAIT_FOR_MOUNT_EXITED )
        {
            if( WIFEXITED( mount_status ) )
            {
                fprintf( stderr,
                         "Error: fsxfsmount exited with code %d before filesystem became ready\n",
                         WEXITSTATUS( mount_status ) );
                xfs_extract_log( "Error: fsxfsmount exited with code %d before mount was ready",
                                 WEXITSTATUS( mount_status ) );
            }
            else if( WIFSIGNALED( mount_status ) )
            {
                fprintf( stderr,
                         "Error: fsxfsmount terminated with signal %d before filesystem became ready\n",
                         WTERMSIG( mount_status ) );
                xfs_extract_log( "Error: fsxfsmount terminated with signal %d before mount was ready",
                                 WTERMSIG( mount_status ) );
            }
            else
            {
                fprintf( stderr,
                         "Error: fsxfsmount exited before filesystem became ready\n" );
                xfs_extract_log( "Error: fsxfsmount exited before mount was ready" );
            }
        }
        else if( mount_wait == XFS_EXTRACT_WAIT_FOR_MOUNT_TIMEOUT )
        {
            fprintf( stderr, "Error: filesystem did not become ready within 10s\n" );
            xfs_extract_log( "Error: mount not ready" );
            xfs_extract_unmount( mount_point, mount_pid );
        }
        else
        {
            fprintf( stderr, "Error: unable to monitor mount startup\n" );
            xfs_extract_log( "Error: unable to monitor mount startup" );
        }
        rmdir( mount_point );
        xfs_extract_log_close();
        return( 1 );
    }
    mounted = 1;

    xfs_extract_log( "Copying files..." );
    result = xfs_extract_copy_directory( mount_point, output_dir );
    xfs_extract_log( "Copy %s", result == 0 ? "complete" : "completed with warnings" );

    if( mounted != 0 )
    {
        xfs_extract_log( "Unmounting (PID %d)...", (int) mount_pid );
        xfs_extract_unmount( mount_point, mount_pid );
    }

    xfs_extract_remove_directory( mount_point );
    xfs_extract_log( "Done (exit code 0)" );
    xfs_extract_log_close();

    return( 0 );
}

#endif /* _WIN32 */
