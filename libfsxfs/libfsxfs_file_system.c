/*
 * File system functions
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
#include <types.h>

#include "libfsxfs_definitions.h"
#include "libfsxfs_directory.h"
#include "libfsxfs_directory_entry.h"
#include "libfsxfs_file_system.h"
#include "libfsxfs_inode.h"
#include "libfsxfs_inode_btree.h"
#include "libfsxfs_inode_information.h"
#include "libfsxfs_libbfio.h"
#include "libfsxfs_libcerror.h"
#include "libfsxfs_libcnotify.h"
#include "libfsxfs_libcthreads.h"
#include "libfsxfs_libuna.h"

/* Creates a file system
 * Make sure the value file_system is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_file_system_initialize(
     libfsxfs_file_system_t **file_system,
     uint64_t root_directory_inode_number,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_file_system_initialize";

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( *file_system != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file system value already set.",
		 function );

		return( -1 );
	}
	*file_system = memory_allocate_structure(
	                libfsxfs_file_system_t );

	if( *file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file system.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *file_system,
	     0,
	     sizeof( libfsxfs_file_system_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file system.",
		 function );

		memory_free(
		 *file_system );

		*file_system = NULL;

		return( -1 );
	}
	if( libfsxfs_inode_btree_initialize(
	     &( ( *file_system )->inode_btree ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create inode B+ tree.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_initialize(
	     &( ( *file_system )->read_write_lock ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize read/write lock.",
		 function );

		goto on_error;
	}
#endif
	( *file_system )->root_directory_inode_number = root_directory_inode_number;

	return( 1 );

on_error:
	if( *file_system != NULL )
	{
		if( ( *file_system )->inode_btree != NULL )
		{
			libfsxfs_inode_btree_free(
			 &( ( *file_system )->inode_btree ),
			 NULL );
		}
		memory_free(
		 *file_system );

		*file_system = NULL;
	}
	return( -1 );
}

/* Frees a file system
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_file_system_free(
     libfsxfs_file_system_t **file_system,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_file_system_free";
	int result            = 1;

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( *file_system != NULL )
	{
#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
		if( libcthreads_read_write_lock_free(
		     &( ( *file_system )->read_write_lock ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free read/write lock.",
			 function );

			result = -1;
		}
#endif
		if( libfsxfs_inode_btree_free(
		     &( ( *file_system )->inode_btree ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free inode B+ tree.",
			 function );

			result = -1;
		}
		memory_free(
		 *file_system );

		*file_system = NULL;
	}
	return( result );
}

/* Reads the inode information
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_file_system_read_inode_information(
     libfsxfs_file_system_t *file_system,
     libfsxfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_file_system_read_inode_information";

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( libfsxfs_inode_btree_read_inode_information(
	     file_system->inode_btree,
	     io_handle,
	     file_io_handle,
	     file_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read inode information at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( 1 );
	}
	return( 1 );
}

/* Retrieves a specific inode
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libfsxfs_file_system_get_inode_by_number(
     libfsxfs_file_system_t *file_system,
     libfsxfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint64_t inode_number,
     libfsxfs_inode_t **inode,
     libcerror_error_t **error )
{
	libfsxfs_inode_t *safe_inode = NULL;
	static char *function        = "libfsxfs_file_system_get_inode_by_number";
	off64_t file_offset          = 0;
	int result                   = 0;

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( io_handle->inode_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid IO handle - inode size value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( inode_number == 0 )
	 || ( inode_number > (uint64_t) UINT32_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid inode number value out of bounds.",
		 function );

		return( -1 );
	}
	result = libfsxfs_inode_btree_get_inode_by_number(
	          file_system->inode_btree,
	          io_handle,
	          file_io_handle,
	          inode_number,
	          &file_offset,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to retrieve inode: %" PRIu64 "\n",
		 function,
		 inode_number );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libfsxfs_inode_initialize(
		     &safe_inode,
		     io_handle->inode_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create inode.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "Reading inode: %" PRIu64 " at offset: %" PRIi64 " (0x%08" PRIx64 ").\n",
			 inode_number,
			 file_offset,
			 file_offset );
		}
#endif
		if( libfsxfs_inode_read_file_io_handle(
		     safe_inode,
		     io_handle,
		     file_io_handle,
		     file_offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read inode: %" PRIu64 " at offset: %" PRIi64 " (0x%08" PRIx64 ").",
			 function,
			 inode_number,
			 file_offset,
			 file_offset );

			goto on_error;
		}
		*inode = safe_inode;
	}
	return( result );

on_error:
	if( safe_inode != NULL )
	{
		libfsxfs_inode_free(
		 &safe_inode,
		 NULL );
	}
	return( -1 );
}

/* Retrieves a directory entry for a specific UTF-8 formatted path
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfsxfs_file_system_get_directory_entry_by_utf8_path(
     libfsxfs_file_system_t *file_system,
     libfsxfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     uint64_t *inode_number,
     libfsxfs_inode_t **inode,
     libfsxfs_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
	libfsxfs_directory_t *directory                  = NULL;
	libfsxfs_directory_entry_t *safe_directory_entry = NULL;
	libfsxfs_inode_t *safe_inode                     = NULL;
	const uint8_t *utf8_string_segment               = NULL;
	static char *function                            = "libfsxfs_file_system_get_directory_entry_by_utf8_path";
	libuna_unicode_character_t unicode_character     = 0;
	size_t utf8_string_index                         = 0;
	size_t utf8_string_segment_length                = 0;
	uint64_t safe_inode_number                       = 0;
	int result                                       = 0;

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( utf8_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( utf8_string_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 string length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( inode == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode.",
		 function );

		return( -1 );
	}
	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( utf8_string_length > 0 )
	{
		/* Ignore a leading separator
		 */
		if( utf8_string[ utf8_string_index ] == (uint8_t) LIBFSXFS_SEPARATOR )
		{
			utf8_string_index++;
		}
	}
	safe_inode_number = file_system->root_directory_inode_number;

	if( libfsxfs_file_system_get_inode_by_number(
	     file_system,
	     io_handle,
	     file_io_handle,
	     safe_inode_number,
	     &safe_inode,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve inode: %" PRIu64 ".",
		 function,
		 safe_inode_number );

		goto on_error;
	}
	if( ( utf8_string_length == 0 )
	 || ( utf8_string_length == 1 ) )
	{
		result = 1;
	}
	else while( utf8_string_index < utf8_string_length )
	{
		if( directory != NULL )
		{
			if( libfsxfs_directory_free(
			     &directory,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free directory.",
				 function );

				goto on_error;
			}
		}
		if( libfsxfs_directory_initialize(
		     &directory,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create directory.",
			 function );

			goto on_error;
		}
		if( libfsxfs_directory_read_file_io_handle(
		     directory,
		     io_handle,
		     file_io_handle,
		     safe_inode,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read directory from inode: %" PRIu64 ".",
			 function,
			 safe_inode_number );

			goto on_error;
		}
		utf8_string_segment        = &( utf8_string[ utf8_string_index ] );
		utf8_string_segment_length = utf8_string_index;

		while( utf8_string_index < utf8_string_length )
		{
			if( libuna_unicode_character_copy_from_utf8(
			     &unicode_character,
			     utf8_string,
			     utf8_string_length,
			     &utf8_string_index,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy UTF-8 string to Unicode character.",
				 function );

				goto on_error;
			}
			if( ( unicode_character == (libuna_unicode_character_t) LIBFSXFS_SEPARATOR )
			 || ( unicode_character == 0 ) )
			{
				utf8_string_segment_length += 1;

				break;
			}
		}
		utf8_string_segment_length = utf8_string_index - utf8_string_segment_length;

		if( utf8_string_segment_length == 0 )
		{
			result = 0;
		}
		else
		{
			result = libfsxfs_directory_get_entry_by_utf8_name(
			          directory,
			          utf8_string_segment,
			          utf8_string_segment_length,
			          &safe_directory_entry,
			          error );
		}
		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve directory entry by UTF-8 name.",
			 function );

			goto on_error;
		}
		else if( result == 0 )
		{
			break;
		}
		if( libfsxfs_directory_entry_get_inode_number(
		     safe_directory_entry,
		     &safe_inode_number,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve inode number from directory entry.",
			 function );

			goto on_error;
		}
		if( safe_inode != NULL )
		{
			if( libfsxfs_inode_free(
			     &safe_inode,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free inode.",
				 function );

				goto on_error;
			}
		}
		if( libfsxfs_file_system_get_inode_by_number(
		     file_system,
		     io_handle,
		     file_io_handle,
		     safe_inode_number,
		     &safe_inode,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve inode: %" PRIu32 ".",
			 function,
			 safe_inode_number );

			goto on_error;
		}
	}
	if( result == 0 )
	{
		if( safe_inode != NULL )
		{
			if( libfsxfs_inode_free(
			     &safe_inode,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free inode.",
				 function );

				goto on_error;
			}
		}
	}
	else
	{
		if( libfsxfs_directory_entry_clone(
		     directory_entry,
		     safe_directory_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create directory entry.",
			 function );

			goto on_error;
		}
	}
	/* Directory needs to be freed after making a clone of directory_entry
	 */
	if( directory != NULL )
	{
		if( libfsxfs_directory_free(
		     &directory,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free directory.",
			 function );

			goto on_error;
		}
	}
	*inode_number = safe_inode_number;
	*inode        = safe_inode;

	return( result );

on_error:
	if( directory != NULL )
	{
		libfsxfs_directory_free(
		 &directory,
		 NULL );
	}
	if( safe_inode != NULL )
	{
		libfsxfs_inode_free(
		 &safe_inode,
		 NULL );
	}
	if( *directory_entry != NULL )
	{
		libfsxfs_directory_entry_free(
		 directory_entry,
		 NULL );
	}
	return( -1 );
}

/* Retrieves a directory entry for a specific UTF-16 formatted path
 * Returns 1 if successful, 0 if not found or -1 on error
 */
int libfsxfs_file_system_get_directory_entry_by_utf16_path(
     libfsxfs_file_system_t *file_system,
     libfsxfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     uint64_t *inode_number,
     libfsxfs_inode_t **inode,
     libfsxfs_directory_entry_t **directory_entry,
     libcerror_error_t **error )
{
	libfsxfs_directory_t *directory                  = NULL;
	libfsxfs_directory_entry_t *safe_directory_entry = NULL;
	libfsxfs_inode_t *safe_inode                     = NULL;
	const uint16_t *utf16_string_segment             = NULL;
	static char *function                            = "libfsxfs_file_system_get_directory_entry_by_utf16_path";
	libuna_unicode_character_t unicode_character     = 0;
	size_t utf16_string_index                        = 0;
	size_t utf16_string_segment_length               = 0;
	uint64_t safe_inode_number                       = 0;
	int result                                       = 0;

	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( utf16_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string.",
		 function );

		return( -1 );
	}
	if( utf16_string_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-16 string length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( inode == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode.",
		 function );

		return( -1 );
	}
	if( directory_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid directory entry.",
		 function );

		return( -1 );
	}
	if( utf16_string_length > 0 )
	{
		/* Ignore a leading separator
		 */
		if( utf16_string[ utf16_string_index ] == (uint16_t) LIBFSXFS_SEPARATOR )
		{
			utf16_string_index++;
		}
	}
	safe_inode_number = file_system->root_directory_inode_number;

	if( libfsxfs_file_system_get_inode_by_number(
	     file_system,
	     io_handle,
	     file_io_handle,
	     safe_inode_number,
	     &safe_inode,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve inode: %" PRIu64 ".",
		 function,
		 safe_inode_number );

		goto on_error;
	}
	if( ( utf16_string_length == 0 )
	 || ( utf16_string_length == 1 ) )
	{
		result = 1;
	}
	else while( utf16_string_index < utf16_string_length )
	{
		if( directory != NULL )
		{
			if( libfsxfs_directory_free(
			     &directory,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free directory.",
				 function );

				goto on_error;
			}
		}
		if( libfsxfs_directory_initialize(
		     &directory,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create directory.",
			 function );

			goto on_error;
		}
		if( libfsxfs_directory_read_file_io_handle(
		     directory,
		     io_handle,
		     file_io_handle,
		     safe_inode,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read directory from inode: %" PRIu64 ".",
			 function,
			 safe_inode_number );

			goto on_error;
		}
		utf16_string_segment        = &( utf16_string[ utf16_string_index ] );
		utf16_string_segment_length = utf16_string_index;

		while( utf16_string_index < utf16_string_length )
		{
			if( libuna_unicode_character_copy_from_utf16(
			     &unicode_character,
			     utf16_string,
			     utf16_string_length,
			     &utf16_string_index,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy UTF-16 string to Unicode character.",
				 function );

				goto on_error;
			}
			if( ( unicode_character == (libuna_unicode_character_t) LIBFSXFS_SEPARATOR )
			 || ( unicode_character == 0 ) )
			{
				utf16_string_segment_length += 1;

				break;
			}
		}
		utf16_string_segment_length = utf16_string_index - utf16_string_segment_length;

		if( utf16_string_segment_length == 0 )
		{
			result = 0;
		}
		else
		{
			result = libfsxfs_directory_get_entry_by_utf16_name(
			          directory,
			          utf16_string_segment,
			          utf16_string_segment_length,
			          &safe_directory_entry,
			          error );
		}
		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve directory entry by UTF-16 name.",
			 function );

			goto on_error;
		}
		else if( result == 0 )
		{
			break;
		}
		if( libfsxfs_directory_entry_get_inode_number(
		     safe_directory_entry,
		     &safe_inode_number,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve inode number from directory entry.",
			 function );

			goto on_error;
		}
		if( safe_inode != NULL )
		{
			if( libfsxfs_inode_free(
			     &safe_inode,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free inode.",
				 function );

				goto on_error;
			}
		}
		if( libfsxfs_file_system_get_inode_by_number(
		     file_system,
		     io_handle,
		     file_io_handle,
		     safe_inode_number,
		     &safe_inode,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve inode: %" PRIu32 ".",
			 function,
			 safe_inode_number );

			goto on_error;
		}
	}
	if( result == 0 )
	{
		if( safe_inode != NULL )
		{
			if( libfsxfs_inode_free(
			     &safe_inode,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free inode.",
				 function );

				goto on_error;
			}
		}
	}
	else
	{
		if( libfsxfs_directory_entry_clone(
		     directory_entry,
		     safe_directory_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create directory entry.",
			 function );

			goto on_error;
		}
	}
	/* Directory needs to be freed after making a clone of directory_entry
	 */
	if( directory != NULL )
	{
		if( libfsxfs_directory_free(
		     &directory,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free directory.",
			 function );

			goto on_error;
		}
	}
	*inode_number = safe_inode_number;
	*inode        = safe_inode;

	return( result );

on_error:
	if( directory != NULL )
	{
		libfsxfs_directory_free(
		 &directory,
		 NULL );
	}
	if( safe_inode != NULL )
	{
		libfsxfs_inode_free(
		 &safe_inode,
		 NULL );
	}
	if( *directory_entry != NULL )
	{
		libfsxfs_directory_entry_free(
		 directory_entry,
		 NULL );
	}
	return( -1 );
}

