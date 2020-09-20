/*
 * Shows information obtained from an Extended File System (ext) volume
 *
 * Copyright (C) 2020, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <file_stream.h>
#include <memory.h>
#include <system_string.h>
#include <types.h>

#include <stdio.h>

#if defined( HAVE_IO_H ) || defined( WINAPI )
#include <io.h>
#endif

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

#include "fsxfstools_getopt.h"
#include "fsxfstools_libcerror.h"
#include "fsxfstools_libclocale.h"
#include "fsxfstools_libcnotify.h"
#include "fsxfstools_libfsxfs.h"
#include "fsxfstools_output.h"
#include "fsxfstools_signal.h"
#include "fsxfstools_unused.h"
#include "info_handle.h"

enum FSXFSINFO_MODES
{
	FSXFSINFO_MODE_FILE_ENTRY,
	FSXFSINFO_MODE_FILE_SYSTEM_HIERARCHY,
	FSXFSINFO_MODE_INODE,
	FSXFSINFO_MODE_VOLUME
};

info_handle_t *fsxfsinfo_info_handle = NULL;
int fsxfsinfo_abort                  = 0;

/* Prints the executable usage information
 */
void usage_fprint(
      FILE *stream )
{
	if( stream == NULL )
	{
		return;
	}
	fprintf( stream, "Use fsxfsinfo to determine information about an Extended\n"
	                 " File System (ext) volume.\n\n" );

	fprintf( stream, "Usage: fsxfsinfo [ -B bodyfile ] [ -E inode_number ] [ -F file_entry ]\n"
	                 "                 [ -o offset ] [ -hHvV ] source\n\n" );

	fprintf( stream, "\tsource: the source file or device\n\n" );

	fprintf( stream, "\t-B:     output file system information as a bodyfile\n" );
	fprintf( stream, "\t-E:     show information about a specific inode or \"all\".\n" );
	fprintf( stream, "\t-F:     show information about a specific file entry path.\n" );
	fprintf( stream, "\t-h:     shows this help\n" );
	fprintf( stream, "\t-H:     shows the file system hierarchy\n" );
	fprintf( stream, "\t-o:     specify the volume offset\n" );
	fprintf( stream, "\t-v:     verbose output to stderr\n" );
	fprintf( stream, "\t-V:     print version\n" );
}

/* Signal handler for fsxfsinfo
 */
void fsxfsinfo_signal_handler(
      fsxfstools_signal_t signal FSXFSTOOLS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function   = "fsxfsinfo_signal_handler";

	FSXFSTOOLS_UNREFERENCED_PARAMETER( signal )

	fsxfsinfo_abort = 1;

	if( fsxfsinfo_info_handle != NULL )
	{
		if( info_handle_signal_abort(
		     fsxfsinfo_info_handle,
		     &error ) != 1 )
		{
			libcnotify_printf(
			 "%s: unable to signal info handle to abort.\n",
			 function );

			libcnotify_print_error_backtrace(
			 error );
			libcerror_error_free(
			 &error );
		}
	}
	/* Force stdin to close otherwise any function reading it will remain blocked
	 */
#if defined( WINAPI ) && !defined( __CYGWIN__ )
	if( _close(
	     0 ) != 0 )
#else
	if( close(
	     0 ) != 0 )
#endif
	{
		libcnotify_printf(
		 "%s: unable to close stdin.\n",
		 function );
	}
}

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	libfsxfs_error_t *error                  = NULL;
	system_character_t *option_bodyfile      = NULL;
	system_character_t *option_file_entry    = NULL;
	system_character_t *option_inode_number  = NULL;
	system_character_t *option_volume_offset = NULL;
	system_character_t *source               = NULL;
	char *program                            = "fsxfsinfo";
	system_integer_t option                  = 0;
	size_t string_length                     = 0;
	uint64_t inode_number                    = 0;
	int option_mode                          = FSXFSINFO_MODE_VOLUME;
	int verbose                              = 0;

	libcnotify_stream_set(
	 stderr,
	 NULL );
	libcnotify_verbose_set(
	 1 );

	if( libclocale_initialize(
             "fsxfstools",
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize locale values.\n" );

		goto on_error;
	}
        if( fsxfstools_output_initialize(
             _IONBF,
             &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize output settings.\n" );

		goto on_error;
	}
	fsxfstools_output_version_fprint(
	 stdout,
	 program );

	while( ( option = fsxfstools_getopt(
	                   argc,
	                   argv,
	                   _SYSTEM_STRING( "B:E:F:hHo:vV" ) ) ) != (system_integer_t) -1 )
	{
		switch( option )
		{
			case (system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_SYSTEM "\n",
				 argv[ optind - 1 ] );

				usage_fprint(
				 stdout );

				return( EXIT_FAILURE );

			case (system_integer_t) 'B':
				option_bodyfile = optarg;

				break;

			case (system_integer_t) 'E':
				option_mode         = FSXFSINFO_MODE_INODE;
				option_inode_number = optarg;

				break;

			case (system_integer_t) 'F':
				option_mode       = FSXFSINFO_MODE_FILE_ENTRY;
				option_file_entry = optarg;

				break;

			case (system_integer_t) 'h':
				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (system_integer_t) 'H':
				option_mode = FSXFSINFO_MODE_FILE_SYSTEM_HIERARCHY;

				break;

			case (system_integer_t) 'o':
				option_volume_offset = optarg;

				break;

			case (system_integer_t) 'v':
				verbose = 1;

				break;

			case (system_integer_t) 'V':
				fsxfstools_output_copyright_fprint(
				 stdout );

				return( EXIT_SUCCESS );
		}
	}
	if( optind == argc )
	{
		fprintf(
		 stderr,
		 "Missing source file or device.\n" );

		usage_fprint(
		 stdout );

		return( EXIT_FAILURE );
	}
	source = argv[ optind ];

	libcnotify_verbose_set(
	 verbose );
	libfsxfs_notify_set_stream(
	 stderr,
	 NULL );
	libfsxfs_notify_set_verbose(
	 verbose );

	if( info_handle_initialize(
	     &fsxfsinfo_info_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize info handle.\n" );

		goto on_error;
	}
	if( option_bodyfile != NULL )
	{
		if( info_handle_set_bodyfile(
		     fsxfsinfo_info_handle,
		     option_bodyfile,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to set bodyfile.\n" );

			goto on_error;
		}
	}
	if( option_volume_offset != NULL )
	{
		if( info_handle_set_volume_offset(
		     fsxfsinfo_info_handle,
		     option_volume_offset,
		     &error ) != 1 )
		{
			libcnotify_print_error_backtrace(
			 error );
			libcerror_error_free(
			 &error );

			fprintf(
			 stderr,
			 "Unsupported volume offset defaulting to: %" PRIi64 ".\n",
			 fsxfsinfo_info_handle->volume_offset );
		}
	}
	if( info_handle_open_input(
	     fsxfsinfo_info_handle,
	     source,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to open: %" PRIs_SYSTEM ".\n",
		 source );

		goto on_error;
	}
	switch( option_mode )
	{
/* TODO
		case FSXFSINFO_MODE_FILE_ENTRY:
			if( info_handle_file_entry_fprint_by_path(
			     fsxfsinfo_info_handle,
			     option_file_entry,
			     &error ) != 1 )
			{
				fprintf(
				 stderr,
				 "Unable to print file entry information.\n" );

				goto on_error;
			}
			break;

		case FSXFSINFO_MODE_FILE_SYSTEM_HIERARCHY:
			if( info_handle_file_system_hierarchy_fprint(
			     fsxfsinfo_info_handle,
			     &error ) != 1 )
			{
				fprintf(
				 stderr,
				 "Unable to print file system hierarchy.\n" );

				goto on_error;
			}
			break;

		case FSXFSINFO_MODE_INODE:
			if( option_inode_number == NULL )
			{
				fprintf(
				 stderr,
				 "Mising inode number string.\n" );

				goto on_error;
			}
			string_length = system_string_length(
					 option_inode_number );

			if( ( string_length == 3 )
			 && ( system_string_compare(
			       option_inode_number,
			       _SYSTEM_STRING( "all" ),
			       3 ) == 0 ) )
			{
				if( info_handle_inodes_fprint(
				     fsxfsinfo_info_handle,
				     &error ) != 1 )
				{
					fprintf(
					 stderr,
					 "Unable to print inodes.\n" );

					goto on_error;
				}
			}
			else if( info_handle_system_string_copy_from_64_bit_in_decimal(
			          option_inode_number,
			          string_length + 1,
			          &inode_number,
			          &error ) == 1 )
			{
				if( inode_number > (uint64_t) UINT32_MAX )
				{
					fprintf(
					 stderr,
					 "Invalid inode number value out of bounds." );

					goto on_error;
				}
				if( info_handle_file_entry_fprint_by_inode(
				     fsxfsinfo_info_handle,
				     (uint32_t) inode_number,
				     &error ) != 1 )
				{
					fprintf(
					 stderr,
					 "Unable to print inode: %" PRIu64 ".\n",
					 inode_number );

					goto on_error;
				}
			}
			else
			{
				fprintf(
				 stderr,
				 "Unable to copy inode number string to 64-bit decimal.\n" );

				goto on_error;
			}
			break;
*/

		case FSXFSINFO_MODE_VOLUME:
		default:
			if( info_handle_volume_fprint(
			     fsxfsinfo_info_handle,
			     &error ) != 1 )
			{
				fprintf(
				 stderr,
				 "Unable to print volume information.\n" );

				goto on_error;
			}
			break;
	}
	if( info_handle_close_input(
	     fsxfsinfo_info_handle,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close info handle.\n" );

		goto on_error;
	}
	if( info_handle_free(
	     &fsxfsinfo_info_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free info handle.\n" );

		goto on_error;
	}
	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	if( fsxfsinfo_info_handle != NULL )
	{
		info_handle_free(
		 &fsxfsinfo_info_handle,
		 NULL );
	}
	return( EXIT_FAILURE );
}

