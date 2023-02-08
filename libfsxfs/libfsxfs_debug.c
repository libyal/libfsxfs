/*
 * Debug functions
 *
 * Copyright (C) 2020-2023, Joachim Metz <joachim.metz@gmail.com>
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
#include <memory.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#include "libfsxfs_debug.h"
#include "libfsxfs_definitions.h"
#include "libfsxfs_libbfio.h"
#include "libfsxfs_libcerror.h"
#include "libfsxfs_libcnotify.h"
#include "libfsxfs_libfdatetime.h"
#include "libfsxfs_libfguid.h"
#include "libfsxfs_libuna.h"

#if defined( HAVE_DEBUG_OUTPUT )

/* Prints the feature flags
 */
void libfsxfs_debug_print_feature_flags(
      uint16_t feature_flags )
{
	if( ( feature_flags & 0x0010 ) != 0 )
	{
		libcnotify_printf(
		 "\t(XFS_SB_VERSION_ATTRBIT)\n" );
	}
	if( ( feature_flags & 0x0020 ) != 0 )
	{
		libcnotify_printf(
		 "\t(XFS_SB_VERSION_NLINKBIT)\n" );
	}
	if( ( feature_flags & 0x0040 ) != 0 )
	{
		libcnotify_printf(
		 "\t(XFS_SB_VERSION_QUOTABIT)\n" );
	}
	if( ( feature_flags & 0x0080 ) != 0 )
	{
		libcnotify_printf(
		 "\t(XFS_SB_VERSION_ALIGNBIT)\n" );
	}
	if( ( feature_flags & 0x0100 ) != 0 )
	{
		libcnotify_printf(
		 "\t(XFS_SB_VERSION_DALIGNBIT)\n" );
	}
	if( ( feature_flags & 0x0200 ) != 0 )
	{
		libcnotify_printf(
		 "\t(XFS_SB_VERSION_SHAREDBIT)\n" );
	}
	if( ( feature_flags & 0x0400 ) != 0 )
	{
		libcnotify_printf(
		 "\t(XFS_SB_VERSION_LOGV2BIT)\n" );
	}
	if( ( feature_flags & 0x0800 ) != 0 )
	{
		libcnotify_printf(
		 "\t(XFS_SB_VERSION_SECTORBIT)\n" );
	}
	if( ( feature_flags & 0x1000 ) != 0 )
	{
		libcnotify_printf(
		 "\t(XFS_SB_VERSION_EXTFLGBIT)\n" );
	}
	if( ( feature_flags & 0x2000 ) != 0 )
	{
		libcnotify_printf(
		 "\t(XFS_SB_VERSION_DIRV2BIT)\n" );
	}
	if( ( feature_flags & 0x4000 ) != 0 )
	{
		libcnotify_printf(
		 "\t(XFS_SB_VERSION_BORGBIT)\n" );
	}
	if( ( feature_flags & 0x8000 ) != 0 )
	{
		libcnotify_printf(
		 "\t(XFS_SB_VERSION_MOREBITSBIT)\n" );
	}
}

/* Prints the secondary feature flags
 */
void libfsxfs_debug_print_secondary_feature_flags(
      uint32_t secondary_feature_flags )
{
	if( ( secondary_feature_flags & 0x00000001UL ) != 0 )
	{
		libcnotify_printf(
		 "\t(XFS_SB_VERSION2_RESERVED1BIT)\n" );
	}
	if( ( secondary_feature_flags & 0x00000002UL ) != 0 )
	{
		libcnotify_printf(
		 "\t(XFS_SB_VERSION2_LAZYSBCOUNTBIT)\n" );
	}
	if( ( secondary_feature_flags & 0x00000004UL ) != 0 )
	{
		libcnotify_printf(
		 "\t(XFS_SB_VERSION2_RESERVED4BIT)\n" );
	}
	if( ( secondary_feature_flags & 0x00000008UL ) != 0 )
	{
		libcnotify_printf(
		 "\t(XFS_SB_VERSION2_ATTR2BIT)\n" );
	}
	if( ( secondary_feature_flags & 0x00000010UL ) != 0 )
	{
		libcnotify_printf(
		 "\t(XFS_SB_VERSION2_PARENTBIT)\n" );
	}

	if( ( secondary_feature_flags & 0x00000080UL ) != 0 )
	{
		libcnotify_printf(
		 "\t(XFS_SB_VERSION2_PROJID32BIT)\n" );
	}
	if( ( secondary_feature_flags & 0x00000100UL ) != 0 )
	{
		libcnotify_printf(
		 "\t(XFS_SB_VERSION2_CRCBIT)\n" );
	}
	if( ( secondary_feature_flags & 0x00000200UL ) != 0 )
	{
		libcnotify_printf(
		 "\t(XFS_SB_VERSION2_FTYPE)\n" );
	}
}

/* Prints the file mode
 */
void libfsxfs_debug_print_file_mode(
      uint16_t file_mode )
{
	if( ( file_mode & 0x0007 ) != 0 )
	{
		libcnotify_printf(
		 "\tAccess other: " );

		if( ( file_mode & 0x0004 ) != 0 )
		{
			libcnotify_printf(
			 "R" );
		}
		if( ( file_mode & 0x0002 ) != 0 )
		{
			libcnotify_printf(
			 "W" );
		}
		if( ( file_mode & 0x0001 ) != 0 )
		{
			libcnotify_printf(
			 "X" );
		}
		libcnotify_printf(
		 "\n" );
	}
	if( ( file_mode & 0x0038 ) != 0 )
	{
		libcnotify_printf(
		 "\tAccess group: " );

		if( ( file_mode & 0x0020 ) != 0 )
		{
			libcnotify_printf(
			 "R" );
		}
		if( ( file_mode & 0x0010 ) != 0 )
		{
			libcnotify_printf(
			 "W" );
		}
		if( ( file_mode & 0x0008 ) != 0 )
		{
			libcnotify_printf(
			 "X" );
		}
		libcnotify_printf(
		 "\n" );
	}
	if( ( file_mode & 0x01c0 ) != 0 )
	{
		libcnotify_printf(
		 "\tAccess user: " );

		if( ( file_mode & 0x0100 ) != 0 )
		{
			libcnotify_printf(
			 "R" );
		}
		if( ( file_mode & 0x0080 ) != 0 )
		{
			libcnotify_printf(
			 "W" );
		}
		if( ( file_mode & 0x0040 ) != 0 )
		{
			libcnotify_printf(
			 "X" );
		}
		libcnotify_printf(
		 "\n" );
	}
	if( ( file_mode & 0x0200 ) != 0 )
	{
		libcnotify_printf(
		 "\tSticky bit (S_ISTXT)" );
	}
	if( ( file_mode & 0x0400 ) != 0 )
	{
		libcnotify_printf(
		 "\tSet group identifer (GID) on execution (S_ISGID)\n" );
	}
	if( ( file_mode & 0x0800 ) != 0 )
	{
		libcnotify_printf(
		 "\tSet user identifer (UID) on execution (S_ISUID)\n" );
	}
	switch( file_mode & 0xf000 )
	{
		case 0x1000:
			libcnotify_printf(
			 "\tNamed pipe (FIFO) (S_IFIFO)\n" );
			break;

		case 0x2000:
			libcnotify_printf(
			 "\tCharacter device (S_IFCHR)\n" );
			break;

		case 0x4000:
			libcnotify_printf(
			 "\tDirectory (S_IFDIR)\n" );
			break;

		case 0x6000:
			libcnotify_printf(
			 "\tBlock device (S_IFBLK)\n" );
			break;

		case 0x8000:
			libcnotify_printf(
			 "\tRegular file (S_IFREG)\n" );
			break;

		case 0xa000:
			libcnotify_printf(
			 "\tSymbolic link (S_IFLNK)\n" );
			break;

		case 0xc000:
			libcnotify_printf(
			 "\tSocket (S_IFSOCK)\n" );
			break;

		default:
			break;
	}
	libcnotify_printf(
	 "\n" );
}

/* Prints the fork type
 */
const char *libfsxfs_debug_print_fork_type(
             uint8_t fork_type )
{
	switch( fork_type )
	{
		case 0:
			return( "XFS_DINODE_FMT_DEV" );

		case 1:
			return( "XFS_DINODE_FMT_LOCAL" );

		case 2:
			return( "XFS_DINODE_FMT_EXTENTS" );

		case 3:
			return( "XFS_DINODE_FMT_BTREE" );

		case 4:
			return( "XFS_DINODE_FMT_UUID" );

		case 5:
			return( "XFS_DINODE_FMT_RMAP" );
	}
	return( "Unknown" );
}

/* Prints a POSIX value
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_debug_print_posix_time_value(
     const char *function_name,
     const char *value_name,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     int byte_order,
     uint8_t value_type,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
	char date_time_string[ 32 ];

	libfdatetime_posix_time_t *posix_time = NULL;
	static char *function                 = "libfsxfs_debug_print_posix_time_value";

	if( libfdatetime_posix_time_initialize(
	     &posix_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create posix time.",
		 function );

		goto on_error;
	}
	if( libfdatetime_posix_time_copy_from_byte_stream(
	     posix_time,
	     byte_stream,
	     byte_stream_size,
	     byte_order,
	     value_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy byte stream to posix time.",
		 function );

		goto on_error;
	}
	if( libfdatetime_posix_time_copy_to_utf8_string(
	     posix_time,
	     (uint8_t *) date_time_string,
	     32,
	     string_format_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy posix_time to string.",
		 function );

		goto on_error;
	}
	libcnotify_printf(
	 "%s: %s: %s UTC\n",
	 function_name,
	 value_name,
	 date_time_string );

	if( libfdatetime_posix_time_free(
	     &posix_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free posix time.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( posix_time != NULL )
	{
		libfdatetime_posix_time_free(
		 &posix_time,
		 NULL );
	}
	return( -1 );
}

/* Prints a GUID/UUID value
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_debug_print_guid_value(
     const char *function_name,
     const char *value_name,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     int byte_order,
     uint32_t string_format_flags,
     libcerror_error_t **error )
{
        system_character_t guid_string[ 48 ];

        libfguid_identifier_t *guid = NULL;
	static char *function       = "libfsxfs_debug_print_guid_value";

	if( libfguid_identifier_initialize(
	     &guid,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create GUID.",
		 function );

		goto on_error;
	}
	if( libfguid_identifier_copy_from_byte_stream(
	     guid,
	     byte_stream,
	     byte_stream_size,
	     byte_order,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy byte stream to GUID.",
		 function );

		goto on_error;
	}
	if( libfguid_identifier_copy_to_utf8_string(
	     guid,
	     (uint8_t *) guid_string,
	     48,
	     string_format_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy GUID to string.",
		 function );

		goto on_error;
	}
	libcnotify_printf(
	 "%s: %s: %s\n",
	 function_name,
	 value_name,
	 guid_string );

	if( libfguid_identifier_free(
	     &guid,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free GUID.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( guid != NULL )
	{
		libfguid_identifier_free(
		 &guid,
		 NULL );
	}
	return( -1 );
}

/* Prints an UTF-8 string value
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_debug_print_utf8_string_value(
     const char *function_name,
     const char *value_name,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     libcerror_error_t **error )
{
	system_character_t *string = NULL;
	static char *function      = "libfsxfs_debug_print_utf8_string_value";
	size_t string_size         = 0;
	int result                 = 0;

	if( ( byte_stream == NULL )
	 || ( byte_stream_size == 0 ) )
	{
		libcnotify_printf(
		 "%s: %s: \n",
		 function_name,
		 value_name );

		return( 1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libuna_utf16_string_size_from_utf8_stream(
		  byte_stream,
		  byte_stream_size,
		  &string_size,
		  error );
#else
	result = libuna_utf8_string_size_from_utf8_stream(
		  byte_stream,
		  byte_stream_size,
		  &string_size,
		  error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine size of string.",
		 function );

		goto on_error;
	}
	if( string_size > (size_t) ( SSIZE_MAX / sizeof( system_character_t ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid string size value exceeds maximum.",
		 function );

		goto on_error;
	}
	string = system_string_allocate(
	          string_size );

	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create string.",
		 function );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libuna_utf16_string_copy_from_utf8_stream(
		  (libuna_utf16_character_t *) string,
		  string_size,
		  byte_stream,
		  byte_stream_size,
		  error );
#else
	result = libuna_utf8_string_copy_from_utf8_stream(
		  (libuna_utf8_character_t *) string,
		  string_size,
		  byte_stream,
		  byte_stream_size,
		  error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set string.",
		 function );

		goto on_error;
	}
	libcnotify_printf(
	 "%s: %s: %" PRIs_SYSTEM "\n",
	 function_name,
	 value_name,
	 string );

	memory_free(
	 string );

	return( 1 );

on_error:
	if( string != NULL )
	{
		memory_free(
		 string );
	}
	return( -1 );
}

/* Prints the read offsets
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_debug_print_read_offsets(
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_debug_print_read_offsets";
	off64_t offset        = 0;
	size64_t size         = 0;
	int number_of_offsets = 0;
	int offset_iterator   = 0;

	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( libbfio_handle_get_number_of_offsets_read(
	     file_io_handle,
	     &number_of_offsets,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of offsets read.",
		 function );

		return( -1 );
	}
	libcnotify_printf(
	 "Offsets read:\n" );

	for( offset_iterator = 0;
	     offset_iterator < number_of_offsets;
	     offset_iterator++ )
	{
		if( libbfio_handle_get_offset_read(
		     file_io_handle,
		     offset_iterator,
		     &offset,
		     &size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve offset: %d.",
			 function,
			 ( offset_iterator + 1 ) );

			return( -1 );
		}
		libcnotify_printf(
		 "%08" PRIi64 " ( 0x%08" PRIx64 " ) - %08" PRIi64 " ( 0x%08" PRIx64 " ) size: %" PRIu64 "\n",
		 offset,
		 offset,
		 offset + (off64_t) size,
		 offset + (off64_t) size,
		 size );
	}
	libcnotify_printf(
	 "\n" );

	return( 1 );
}

#endif /* defined( HAVE_DEBUG_OUTPUT ) */

