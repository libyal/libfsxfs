/*
 * File entry functions
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

#include "libfsxfs_attributes.h"
#include "libfsxfs_data_stream.h"
#include "libfsxfs_definitions.h"
#include "libfsxfs_directory.h"
#include "libfsxfs_directory_entry.h"
#include "libfsxfs_extended_attribute.h"
#include "libfsxfs_file_entry.h"
#include "libfsxfs_inode.h"
#include "libfsxfs_libbfio.h"
#include "libfsxfs_libcerror.h"
#include "libfsxfs_libcnotify.h"
#include "libfsxfs_libcthreads.h"
#include "libfsxfs_libfdata.h"
#include "libfsxfs_libfdatetime.h"
#include "libfsxfs_libuna.h"
#include "libfsxfs_types.h"

/* Creates a file entry
 * Make sure the value file_entry is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_file_entry_initialize(
     libfsxfs_file_entry_t **file_entry,
     libfsxfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfsxfs_file_system_t *file_system,
     uint64_t inode_number,
     libfsxfs_inode_t *inode,
     libfsxfs_directory_entry_t *directory_entry,
     libcerror_error_t **error )
{
	libfsxfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsxfs_file_entry_initialize";

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( *file_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file entry value already set.",
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
	internal_file_entry = memory_allocate_structure(
	                       libfsxfs_internal_file_entry_t );

	if( internal_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file entry.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_file_entry,
	     0,
	     sizeof( libfsxfs_internal_file_entry_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file entry.",
		 function );

		memory_free(
		 internal_file_entry );

		return( -1 );
	}
	if( ( ( inode->file_mode & 0xf000 ) == LIBFSXFS_FILE_TYPE_REGULAR_FILE )
	 || ( ( inode->file_mode & 0xf000 ) == LIBFSXFS_FILE_TYPE_SYMBOLIC_LINK ) )
	{
		if( libfsxfs_inode_get_data_size(
		     inode,
		     &( internal_file_entry->data_size ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data size from inode.",
			 function );

			goto on_error;
		}
		if( libfsxfs_data_stream_initialize(
		     &( internal_file_entry->data_stream ),
		     io_handle,
		     inode,
		     internal_file_entry->data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create data stream.",
			 function );

			goto on_error;
		}
	}
#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_initialize(
	     &( internal_file_entry->read_write_lock ),
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
	internal_file_entry->io_handle       = io_handle;
	internal_file_entry->file_io_handle  = file_io_handle;
	internal_file_entry->file_system     = file_system;
	internal_file_entry->inode_number    = inode_number;
	internal_file_entry->inode           = inode;
	internal_file_entry->directory_entry = directory_entry;

	*file_entry = (libfsxfs_file_entry_t *) internal_file_entry;

	return( 1 );

on_error:
	if( internal_file_entry != NULL )
	{
		memory_free(
		 internal_file_entry );
	}
	return( -1 );
}

/* Frees a file entry
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_file_entry_free(
     libfsxfs_file_entry_t **file_entry,
     libcerror_error_t **error )
{
	libfsxfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsxfs_file_entry_free";
	int result                                          = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( *file_entry != NULL )
	{
		internal_file_entry = (libfsxfs_internal_file_entry_t *) *file_entry;
		*file_entry         = NULL;

#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
		if( libcthreads_read_write_lock_free(
		     &( internal_file_entry->read_write_lock ),
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
		/* The io_handle and file_io_handle references are freed elsewhere
		 */
		if( libfsxfs_inode_free(
		     &( internal_file_entry->inode ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free inode.",
			 function );

			result = -1;
		}
		if( internal_file_entry->directory_entry != NULL )
		{
			if( libfsxfs_directory_entry_free(
			     &( internal_file_entry->directory_entry ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free directory entry.",
				 function );

				result = -1;
			}
		}
		if( internal_file_entry->extended_attributes_array != NULL )
		{
			if( libcdata_array_free(
			     &( internal_file_entry->extended_attributes_array ),
			     (int (*)(intptr_t **, libcerror_error_t **)) &libfsxfs_attribute_values_free,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free extended attributes array.",
				 function );

				result = -1;
			}
		}
		if( internal_file_entry->directory != NULL )
		{
			if( libfsxfs_directory_free(
			     &( internal_file_entry->directory ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free directory.",
				 function );

				result = -1;
			}
		}
		if( internal_file_entry->data_stream != NULL )
		{
			if( libfdata_stream_free(
			     &( internal_file_entry->data_stream ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free data stream.",
				 function );

				result = -1;
			}
		}
		if( internal_file_entry->symbolic_link_data != NULL )
		{
			memory_free(
			 internal_file_entry->symbolic_link_data );
		}
		memory_free(
		 internal_file_entry );
	}
	return( result );
}

/* Determines the directory
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_internal_file_entry_get_directory(
     libfsxfs_internal_file_entry_t *internal_file_entry,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_internal_file_entry_get_directory";

	if( internal_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( internal_file_entry->inode == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file entry - missing inode.",
		 function );

		return( -1 );
	}
	if( internal_file_entry->directory != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file entry - directory value already set.",
		 function );

		return( -1 );
	}
	if( ( internal_file_entry->inode->file_mode & 0xf000 ) == LIBFSXFS_FILE_TYPE_DIRECTORY )
	{
		if( libfsxfs_directory_initialize(
		     &( internal_file_entry->directory ),
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
		     internal_file_entry->directory,
		     internal_file_entry->io_handle,
		     internal_file_entry->file_io_handle,
		     internal_file_entry->inode,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read directory for inode: %" PRIu32 ".",
			 function,
			 internal_file_entry->inode_number );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( internal_file_entry->directory != NULL )
	{
		libfsxfs_directory_free(
		 &( internal_file_entry->directory ),
		 NULL );
	}
	return( -1 );
}

/* Determines the symbolic link data
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_internal_file_entry_get_symbolic_link_data(
     libfsxfs_internal_file_entry_t *internal_file_entry,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_internal_file_entry_get_symbolic_link_data";
	ssize_t read_count    = 0;

	if( internal_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( internal_file_entry->inode == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file entry - missing inode.",
		 function );

		return( -1 );
	}
	if( internal_file_entry->symbolic_link_data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file entry - symbolic link data value already set.",
		 function );

		return( -1 );
	}
	if( ( internal_file_entry->inode->file_mode & 0xf000 ) == LIBFSXFS_FILE_TYPE_SYMBOLIC_LINK )
	{
		if( ( internal_file_entry->data_size == 0 )
		 || ( internal_file_entry->data_size > (uint64_t) MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid data size value out of bounds.",
			 function );

			goto on_error;
		}
		internal_file_entry->symbolic_link_data = (uint8_t *) memory_allocate(
		                                                       sizeof( uint8_t ) * (size_t) internal_file_entry->data_size );

		if( internal_file_entry->symbolic_link_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create symbolic link data.",
			 function );

			goto on_error;
		}
		internal_file_entry->symbolic_link_data_size = (size_t) internal_file_entry->data_size;

		read_count = libfdata_stream_read_buffer_at_offset(
		              internal_file_entry->data_stream,
		              (intptr_t *) internal_file_entry->file_io_handle,
		              internal_file_entry->symbolic_link_data,
		              (size_t) internal_file_entry->data_size,
		              0,
		              0,
		              error );

		if( read_count != (ssize_t) internal_file_entry->data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read from data stream.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: symbolic link data:\n",
			 function );
			libcnotify_print_data(
			 internal_file_entry->symbolic_link_data,
			 internal_file_entry->symbolic_link_data_size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
#endif
	}
	return( 1 );

on_error:
	if( internal_file_entry->symbolic_link_data != NULL )
	{
		memory_free(
		 internal_file_entry->symbolic_link_data );

		internal_file_entry->symbolic_link_data = NULL;
	}
	internal_file_entry->symbolic_link_data_size = 0;

	return( -1 );
}

/* Retrieves the inode number
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_file_entry_get_inode_number(
     libfsxfs_file_entry_t *file_entry,
     uint64_t *inode_number,
     libcerror_error_t **error )
{
	libfsxfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsxfs_file_entry_get_inode_number";

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsxfs_internal_file_entry_t *) file_entry;

	if( inode_number == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid inode number.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*inode_number = internal_file_entry->inode_number;

#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Retrieves the creation date and time
 * The timestamp is a signed 64-bit POSIX date and time value in number of nano seconds
 * This value is retrieved from the inode
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsxfs_file_entry_get_creation_time(
     libfsxfs_file_entry_t *file_entry,
     int64_t *posix_time,
     libcerror_error_t **error )
{
	libfsxfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsxfs_file_entry_get_creation_time";
	int result                                          = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsxfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libfsxfs_inode_get_creation_time(
	          internal_file_entry->inode,
	          posix_time,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve creation time.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the modification date and time
 * The timestamp is a signed 64-bit POSIX date and time value in number of nano seconds
 * This value is retrieved from the inode
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_file_entry_get_modification_time(
     libfsxfs_file_entry_t *file_entry,
     int64_t *posix_time,
     libcerror_error_t **error )
{
	libfsxfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsxfs_file_entry_get_modification_time";
	int result                                          = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsxfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfsxfs_inode_get_modification_time(
	     internal_file_entry->inode,
	     posix_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve modification time.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the access date and time
 * The timestamp is a signed 64-bit POSIX date and time value in number of nano seconds
 * This value is retrieved from the inode
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_file_entry_get_access_time(
     libfsxfs_file_entry_t *file_entry,
     int64_t *posix_time,
     libcerror_error_t **error )
{
	libfsxfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsxfs_file_entry_get_access_time";
	int result                                          = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsxfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfsxfs_inode_get_access_time(
	     internal_file_entry->inode,
	     posix_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve access time.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the inode change date and time
 * The timestamp is a signed 64-bit POSIX date and time value in number of nano seconds
 * This value is retrieved from the inode
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_file_entry_get_inode_change_time(
     libfsxfs_file_entry_t *file_entry,
     int64_t *posix_time,
     libcerror_error_t **error )
{
	libfsxfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsxfs_file_entry_get_inode_change_time";
	int result                                          = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsxfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfsxfs_inode_get_inode_change_time(
	     internal_file_entry->inode,
	     posix_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve inode change time.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the file mode
 * This value is retrieved from the inode
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_file_entry_get_file_mode(
     libfsxfs_file_entry_t *file_entry,
     uint16_t *file_mode,
     libcerror_error_t **error )
{
	libfsxfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsxfs_file_entry_get_file_mode";
	int result                                          = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsxfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfsxfs_inode_get_file_mode(
	     internal_file_entry->inode,
	     file_mode,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file mode.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the number of (hard) links
 * This value is retrieved from the inode
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_file_entry_get_number_of_links(
     libfsxfs_file_entry_t *file_entry,
     uint32_t *number_of_links,
     libcerror_error_t **error )
{
	libfsxfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsxfs_file_entry_get_number_of_links";
	int result                                          = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsxfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfsxfs_inode_get_number_of_links(
	     internal_file_entry->inode,
	     number_of_links,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of (hard_ links.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the owner (user) identifier
 * This value is retrieved from the inode
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_file_entry_get_owner_identifier(
     libfsxfs_file_entry_t *file_entry,
     uint32_t *owner_identifier,
     libcerror_error_t **error )
{
	libfsxfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsxfs_file_entry_get_owner_identifier";
	int result                                          = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsxfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfsxfs_inode_get_owner_identifier(
	     internal_file_entry->inode,
	     owner_identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve owner identifier.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the group identifier
 * This value is retrieved from the inode
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_file_entry_get_group_identifier(
     libfsxfs_file_entry_t *file_entry,
     uint32_t *group_identifier,
     libcerror_error_t **error )
{
	libfsxfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsxfs_file_entry_get_group_identifier";
	int result                                          = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsxfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfsxfs_inode_get_group_identifier(
	     internal_file_entry->inode,
	     group_identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve group identifier.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the device identifier
 * This value is retrieved from the inode
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsxfs_file_entry_get_device_identifier(
     libfsxfs_file_entry_t *file_entry,
     uint32_t *device_identifier,
     libcerror_error_t **error )
{
	libfsxfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsxfs_file_entry_get_device_identifier";
	int result                                          = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsxfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libfsxfs_inode_get_device_identifier(
	          internal_file_entry->inode,
	          device_identifier,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve device identifier from inode.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the device number
 * This value is retrieved from the inode
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsxfs_file_entry_get_device_number(
     libfsxfs_file_entry_t *file_entry,
     uint32_t *major_device_number,
     uint32_t *minor_device_number,
     libcerror_error_t **error )
{
	libfsxfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsxfs_file_entry_get_device_number";
	int result                                          = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsxfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	result = libfsxfs_inode_get_device_number(
	          internal_file_entry->inode,
	          major_device_number,
	          minor_device_number,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve device number from inode.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * This value is retrieved from the directory entry
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsxfs_file_entry_get_utf8_name_size(
     libfsxfs_file_entry_t *file_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libfsxfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsxfs_file_entry_get_utf8_name_size";
	int result                                          = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsxfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->directory_entry != NULL )
	{
		result = libfsxfs_directory_entry_get_utf8_name_size(
		          internal_file_entry->directory_entry,
		          utf8_string_size,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve size of UTF-8 name from directory entry.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-8 encoded name
 * The size should include the end of string character
 * This value is retrieved from the directory entry
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsxfs_file_entry_get_utf8_name(
     libfsxfs_file_entry_t *file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libfsxfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsxfs_file_entry_get_utf8_name";
	int result                                          = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsxfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->directory_entry != NULL )
	{
		result = libfsxfs_directory_entry_get_utf8_name(
		          internal_file_entry->directory_entry,
		          utf8_string,
		          utf8_string_size,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-8 name from directory entry.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * This value is retrieved from the directory entry
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsxfs_file_entry_get_utf16_name_size(
     libfsxfs_file_entry_t *file_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libfsxfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsxfs_file_entry_get_utf16_name_size";
	int result                                          = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsxfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->directory_entry != NULL )
	{
		result = libfsxfs_directory_entry_get_utf16_name_size(
		          internal_file_entry->directory_entry,
		          utf16_string_size,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve size of UTF-16 name from directory entry.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-16 encoded name
 * The size should include the end of string character
 * This value is retrieved from the directory entry
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsxfs_file_entry_get_utf16_name(
     libfsxfs_file_entry_t *file_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libfsxfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsxfs_file_entry_get_utf16_name";
	int result                                          = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsxfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->directory_entry != NULL )
	{
		result = libfsxfs_directory_entry_get_utf16_name(
		          internal_file_entry->directory_entry,
		          utf16_string,
		          utf16_string_size,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-16 name from directory entry.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the size of the UTF-8 encoded symbolic link target
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsxfs_file_entry_get_utf8_symbolic_link_target_size(
     libfsxfs_file_entry_t *file_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libfsxfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsxfs_file_entry_get_utf8_symbolic_link_target_size";
	int result                                          = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsxfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->symbolic_link_data == NULL )
	{
		if( libfsxfs_internal_file_entry_get_symbolic_link_data(
		     internal_file_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine symbolic link data.",
			 function );

			result = -1;
		}
	}
	if( internal_file_entry->symbolic_link_data != NULL )
	{
		result = libuna_utf8_string_size_from_utf8_stream(
		          internal_file_entry->symbolic_link_data,
		          internal_file_entry->symbolic_link_data_size,
		          utf8_string_size,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-8 string size.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-8 encoded symbolic link target
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfsxfs_file_entry_get_utf8_symbolic_link_target(
     libfsxfs_file_entry_t *file_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libfsxfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsxfs_file_entry_get_utf8_symbolic_link_target";
	int result                                          = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsxfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->symbolic_link_data == NULL )
	{
		if( libfsxfs_internal_file_entry_get_symbolic_link_data(
		     internal_file_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine symbolic link data.",
			 function );

			result = -1;
		}
	}
	if( internal_file_entry->symbolic_link_data != NULL )
	{
		result = libuna_utf8_string_copy_from_utf8_stream(
		          utf8_string,
		          utf8_string_size,
		          internal_file_entry->symbolic_link_data,
		          internal_file_entry->symbolic_link_data_size,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-8 string.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Determines the extended attributes
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_internal_file_entry_get_extended_attributes(
     libfsxfs_internal_file_entry_t *internal_file_entry,
     libcerror_error_t **error )
{
	static char *function = "libfsxfs_internal_file_entry_get_extended_attributes";

	if( internal_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( internal_file_entry->extended_attributes_array != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file entry - extended attributes array value already set.",
		 function );

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( internal_file_entry->extended_attributes_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create extended attributes array.",
		 function );

		goto on_error;
	}
	if( libfsxfs_attributes_get_from_inode(
	     internal_file_entry->io_handle,
	     internal_file_entry->file_io_handle,
	     internal_file_entry->inode,
	     internal_file_entry->extended_attributes_array,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to retrieve extended attributes from inode.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( internal_file_entry->extended_attributes_array != NULL )
	{
		libcdata_array_free(
		 &( internal_file_entry->extended_attributes_array ),
		 (int (*)(intptr_t **, libcerror_error_t **)) &libfsxfs_attribute_values_free,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the number of extended attributes
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_file_entry_get_number_of_extended_attributes(
     libfsxfs_file_entry_t *file_entry,
     int *number_of_extended_attributes,
     libcerror_error_t **error )
{
	libfsxfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsxfs_file_entry_get_number_of_extended_attributes";
	int result                                          = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsxfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->extended_attributes_array == NULL )
	{
		if( libfsxfs_internal_file_entry_get_extended_attributes(
		     internal_file_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine extended attributes.",
			 function );

			result = -1;
		}
	}
	if( internal_file_entry->extended_attributes_array != NULL )
	{
		if( libcdata_array_get_number_of_entries(
		     internal_file_entry->extended_attributes_array,
		     number_of_extended_attributes,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of entries from extended attributes array.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the extended attribute for the specific index
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_file_entry_get_extended_attribute_by_index(
     libfsxfs_file_entry_t *file_entry,
     int extended_attribute_index,
     libfsxfs_extended_attribute_t **extended_attribute,
     libcerror_error_t **error )
{
	libfsxfs_attribute_values_t *attribute_values       = NULL;
	libfsxfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsxfs_file_entry_get_extended_attribute_by_index";
	int result                                          = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsxfs_internal_file_entry_t *) file_entry;

	if( extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extended attribute.",
		 function );

		return( -1 );
	}
	if( *extended_attribute != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid extended attribute value already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->extended_attributes_array == NULL )
	{
		if( libfsxfs_internal_file_entry_get_extended_attributes(
		     internal_file_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine extended attributes.",
			 function );

			result = -1;
		}
	}
	if( internal_file_entry->extended_attributes_array != NULL )
	{
		if( libcdata_array_get_entry_by_index(
		     internal_file_entry->extended_attributes_array,
		     extended_attribute_index,
		     (intptr_t **) &attribute_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attribute: %d values.",
			 function,
			 extended_attribute_index );

			result = -1;
		}
		else
		{
			if( libfsxfs_extended_attribute_initialize(
			     extended_attribute,
			     internal_file_entry->io_handle,
			     internal_file_entry->file_io_handle,
			     internal_file_entry->file_system,
			     internal_file_entry->inode,
			     attribute_values,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create extended attribute.",
				 function );

				result = -1;
			}
		}
	}
#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the attribute values for an UTF-8 encoded name
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
int libfsxfs_internal_file_entry_get_attribute_values_by_utf8_name(
     libfsxfs_internal_file_entry_t *internal_file_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsxfs_attribute_values_t **attribute_values,
     libcerror_error_t **error )
{
	libfsxfs_attribute_values_t *safe_attribute_values = NULL;
	static char *function                              = "libfsxfs_internal_file_entry_get_attribute_values_by_utf8_name";
	int attribute_index                                = 0;
	int number_of_attributes                           = 0;
	int result                                         = 0;

	if( internal_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( internal_file_entry->extended_attributes_array == NULL )
	{
		if( libfsxfs_internal_file_entry_get_extended_attributes(
		     internal_file_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine extended attributes.",
			 function );

			return( -1 );
		}
	}
	if( libcdata_array_get_number_of_entries(
	     internal_file_entry->extended_attributes_array,
	     &number_of_attributes,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from extended attributes array.",
		 function );

		return( -1 );
	}
	for( attribute_index = 0;
	     attribute_index < number_of_attributes;
	     attribute_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     internal_file_entry->extended_attributes_array,
		     attribute_index,
		     (intptr_t **) &safe_attribute_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attribute: %d values.",
			 function,
			 attribute_index );

			return( -1 );
		}
		result = libfsxfs_attribute_values_compare_name_with_utf8_string(
		          safe_attribute_values,
		          utf8_string,
		          utf8_string_length,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to compare UTF-8 string with name of attribute: %d values.",
			 function,
			 attribute_index );

			return( -1 );
		}
		else if( result == LIBUNA_COMPARE_EQUAL )
		{
			*attribute_values = safe_attribute_values;

			return( 1 );
		}
	}
	return( 0 );
}

/* Retrieves the attribute values for an UTF-16 encoded name
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
int libfsxfs_internal_file_entry_get_attribute_values_by_utf16_name(
     libfsxfs_internal_file_entry_t *internal_file_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsxfs_attribute_values_t **attribute_values,
     libcerror_error_t **error )
{
	libfsxfs_attribute_values_t *safe_attribute_values = NULL;
	static char *function                              = "libfsxfs_internal_file_entry_get_attribute_values_by_utf16_name";
	int attribute_index                                = 0;
	int number_of_attributes                           = 0;
	int result                                         = 0;

	if( internal_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( internal_file_entry->extended_attributes_array == NULL )
	{
		if( libfsxfs_internal_file_entry_get_extended_attributes(
		     internal_file_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine extended attributes.",
			 function );

			return( -1 );
		}
	}
	if( libcdata_array_get_number_of_entries(
	     internal_file_entry->extended_attributes_array,
	     &number_of_attributes,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from extended attributes array.",
		 function );

		return( -1 );
	}
	for( attribute_index = 0;
	     attribute_index < number_of_attributes;
	     attribute_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     internal_file_entry->extended_attributes_array,
		     attribute_index,
		     (intptr_t **) &safe_attribute_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve attribute: %d values.",
			 function,
			 attribute_index );

			return( -1 );
		}
		result = libfsxfs_attribute_values_compare_name_with_utf16_string(
		          safe_attribute_values,
		          utf16_string,
		          utf16_string_length,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to compare UTF-16 string with name of attribute: %d values.",
			 function,
			 attribute_index );

			return( -1 );
		}
		else if( result == LIBUNA_COMPARE_EQUAL )
		{
			*attribute_values = safe_attribute_values;

			return( 1 );
		}
	}
	return( 0 );
}

/* Determines if there is an extended attribute for an UTF-8 encoded name
 * Returns 1 if available, 0 if not or -1 on error
 */
int libfsxfs_file_entry_has_extended_attribute_by_utf8_name(
     libfsxfs_file_entry_t *file_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error )
{
	libfsxfs_attribute_values_t *attribute_values       = NULL;
	libfsxfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsxfs_file_entry_has_extended_attribute_by_utf8_name";
	int result                                          = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsxfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	result = libfsxfs_internal_file_entry_get_attribute_values_by_utf8_name(
	          internal_file_entry,
	          utf8_string,
	          utf8_string_length,
	          &attribute_values,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve attribute values for UTF-8 name.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Determines if there is an extended attribute for an UTF-8 encoded name
 * Returns 1 if available, 0 if not or -1 on error
 */
int libfsxfs_file_entry_has_extended_attribute_by_utf16_name(
     libfsxfs_file_entry_t *file_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error )
{
	libfsxfs_attribute_values_t *attribute_values       = NULL;
	libfsxfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsxfs_file_entry_has_extended_attribute_by_utf16_name";
	int result                                          = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsxfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	result = libfsxfs_internal_file_entry_get_attribute_values_by_utf16_name(
	          internal_file_entry,
	          utf16_string,
	          utf16_string_length,
	          &attribute_values,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve attribute values for UTF-16 name.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the extended attribute for an UTF-8 encoded name
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
int libfsxfs_file_entry_get_extended_attribute_by_utf8_name(
     libfsxfs_file_entry_t *file_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsxfs_extended_attribute_t **extended_attribute,
     libcerror_error_t **error )
{
	libfsxfs_attribute_values_t *attribute_values       = NULL;
	libfsxfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsxfs_file_entry_get_extended_attribute_by_utf8_name";
	int result                                          = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsxfs_internal_file_entry_t *) file_entry;

	if( extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extended attribute.",
		 function );

		return( -1 );
	}
	if( *extended_attribute != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid extended attribute value already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	result = libfsxfs_internal_file_entry_get_attribute_values_by_utf8_name(
	          internal_file_entry,
	          utf8_string,
	          utf8_string_length,
	          &attribute_values,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve attribute values for UTF-8 name.",
		 function );

		result = -1;
	}
	else if( result != 0 )
	{
		if( libfsxfs_extended_attribute_initialize(
		     extended_attribute,
		     internal_file_entry->io_handle,
		     internal_file_entry->file_io_handle,
		     internal_file_entry->file_system,
		     internal_file_entry->inode,
		     attribute_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create extended attribute.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the extended attribute for an UTF-16 encoded name
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
int libfsxfs_file_entry_get_extended_attribute_by_utf16_name(
     libfsxfs_file_entry_t *file_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsxfs_extended_attribute_t **extended_attribute,
     libcerror_error_t **error )
{
	libfsxfs_attribute_values_t *attribute_values       = NULL;
	libfsxfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsxfs_file_entry_get_extended_attribute_by_utf16_name";
	int result                                          = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsxfs_internal_file_entry_t *) file_entry;

	if( extended_attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid extended attribute.",
		 function );

		return( -1 );
	}
	if( *extended_attribute != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid extended attribute value already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	result = libfsxfs_internal_file_entry_get_attribute_values_by_utf16_name(
	          internal_file_entry,
	          utf16_string,
	          utf16_string_length,
	          &attribute_values,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve attribute values for UTF-16 name.",
		 function );

		result = -1;
	}
	else if( result != 0 )
	{
		if( libfsxfs_extended_attribute_initialize(
		     extended_attribute,
		     internal_file_entry->io_handle,
		     internal_file_entry->file_io_handle,
		     internal_file_entry->file_system,
		     internal_file_entry->inode,
		     attribute_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create extended attribute.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the number of sub file entries
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_file_entry_get_number_of_sub_file_entries(
     libfsxfs_file_entry_t *file_entry,
     int *number_of_sub_file_entries,
     libcerror_error_t **error )
{
	libfsxfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsxfs_file_entry_get_number_of_sub_file_entries";
	int result                                          = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsxfs_internal_file_entry_t *) file_entry;

	if( number_of_sub_file_entries == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of sub file entries.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->directory == NULL )
	{
		if( libfsxfs_internal_file_entry_get_directory(
		     internal_file_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve directory for inode: %" PRIu32 ".",
			 function,
			 internal_file_entry->inode_number );

			result = -1;
		}
	}
	if( internal_file_entry->directory != NULL )
	{
		if( libfsxfs_directory_get_number_of_entries(
		     internal_file_entry->directory,
		     number_of_sub_file_entries,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of sub directory entries.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the sub file entry for the specific index
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_file_entry_get_sub_file_entry_by_index(
     libfsxfs_file_entry_t *file_entry,
     int sub_file_entry_index,
     libfsxfs_file_entry_t **sub_file_entry,
     libcerror_error_t **error )
{
	libfsxfs_directory_entry_t *safe_directory_entry    = NULL;
	libfsxfs_directory_entry_t *sub_directory_entry     = NULL;
	libfsxfs_inode_t *inode                             = NULL;
	libfsxfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsxfs_file_entry_get_sub_file_entry_by_index";
	int result                                          = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsxfs_internal_file_entry_t *) file_entry;

	if( sub_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub file entry.",
		 function );

		return( -1 );
	}
	if( *sub_file_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid sub file entry value already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( internal_file_entry->directory == NULL )
	{
		if( libfsxfs_internal_file_entry_get_directory(
		     internal_file_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve directory for inode: %" PRIu32 ".",
			 function,
			 internal_file_entry->inode_number );

			result = -1;
		}
	}
	if( result != -1 )
	{
		if( libfsxfs_directory_get_entry_by_index(
		     internal_file_entry->directory,
		     sub_file_entry_index,
		     &sub_directory_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub directory entry: %d.",
			 function,
			 sub_file_entry_index );

			result = -1;
		}
		else if( libfsxfs_file_system_get_inode_by_number(
		          internal_file_entry->file_system,
		          internal_file_entry->io_handle,
		          internal_file_entry->file_io_handle,
		          sub_directory_entry->inode_number,
		          &inode,
		          error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to retrieve sub directory entry: %d inode: %" PRIu64 "\n",
			 function,
			 sub_file_entry_index,
			 sub_directory_entry->inode_number );

			result = -1;
		}
		else if( libfsxfs_directory_entry_clone(
		          &safe_directory_entry,
		          sub_directory_entry,
		          error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to clone sub directory entry: %d.",
			 function,
			 sub_file_entry_index );

			libfsxfs_inode_free(
			 &inode,
			 NULL );

			result = -1;
		}
		/* libfsxfs_file_entry_initialize takes over management of sub_directory_entry
		 */
		else if( libfsxfs_file_entry_initialize(
		          sub_file_entry,
		          internal_file_entry->io_handle,
		          internal_file_entry->file_io_handle,
		          internal_file_entry->file_system,
		          sub_directory_entry->inode_number,
		          inode,
		          safe_directory_entry,
		          error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create file entry.",
			 function );

			libfsxfs_inode_free(
			 &inode,
			 NULL );

			libfsxfs_directory_entry_free(
			 &safe_directory_entry,
			 NULL );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the sub file entry for an UTF-8 encoded name
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
int libfsxfs_file_entry_get_sub_file_entry_by_utf8_name(
     libfsxfs_file_entry_t *file_entry,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsxfs_file_entry_t **sub_file_entry,
     libcerror_error_t **error )
{
	libfsxfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsxfs_file_entry_get_sub_file_entry_by_utf8_name";
	int result                                          = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsxfs_internal_file_entry_t *) file_entry;

	if( sub_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub file entry.",
		 function );

		return( -1 );
	}
	if( *sub_file_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid sub file entry value already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	/* TODO implement */
	result = -1;

#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the sub file entry for an UTF-16 encoded name
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
int libfsxfs_file_entry_get_sub_file_entry_by_utf16_name(
     libfsxfs_file_entry_t *file_entry,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsxfs_file_entry_t **sub_file_entry,
     libcerror_error_t **error )
{
	libfsxfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsxfs_file_entry_get_sub_file_entry_by_utf16_name";
	int result                                          = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsxfs_internal_file_entry_t *) file_entry;

	if( sub_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub file entry.",
		 function );

		return( -1 );
	}
	if( *sub_file_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid sub file entry value already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	/* TODO implement */
	result = -1;

#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Reads data at the current offset
 * Returns the number of bytes read or -1 on error
 */
ssize_t libfsxfs_file_entry_read_buffer(
         libfsxfs_file_entry_t *file_entry,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error )
{
	libfsxfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsxfs_file_entry_read_buffer";
	ssize_t read_count                                  = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsxfs_internal_file_entry_t *) file_entry;

	if( internal_file_entry->inode == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file entry - missing inode.",
		 function );

		return( -1 );
	}
	if( ( internal_file_entry->inode->file_mode & 0xf000 ) != LIBFSXFS_FILE_TYPE_REGULAR_FILE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid file entry - invalid inode - unsupported file mode not a regular file.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	read_count = libfdata_stream_read_buffer(
	              internal_file_entry->data_stream,
	              (intptr_t *) internal_file_entry->file_io_handle,
	              buffer,
	              buffer_size,
	              0,
	              error );

	if( read_count < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read from data stream.",
		 function );

		read_count = -1;
	}
#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( read_count );
}

/* Reads data at a specific offset
 * Returns the number of bytes read or -1 on error
 */
ssize_t libfsxfs_file_entry_read_buffer_at_offset(
         libfsxfs_file_entry_t *file_entry,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error )
{
	libfsxfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsxfs_file_entry_read_buffer_at_offset";
	ssize_t read_count                                  = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsxfs_internal_file_entry_t *) file_entry;

	if( internal_file_entry->inode == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file entry - missing inode.",
		 function );

		return( -1 );
	}
	if( ( internal_file_entry->inode->file_mode & 0xf000 ) != LIBFSXFS_FILE_TYPE_REGULAR_FILE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid file entry - invalid inode - unsupported file mode not a regular file.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	read_count = libfdata_stream_read_buffer_at_offset(
	              internal_file_entry->data_stream,
	              (intptr_t *) internal_file_entry->file_io_handle,
	              buffer,
	              buffer_size,
	              offset,
	              0,
	              error );

	if( read_count < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read from data stream.",
		 function );

		read_count = -1;
	}
#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( read_count );
}

/* Seeks a certain offset in the data
 * Returns the offset if seek is successful or -1 on error
 */
off64_t libfsxfs_file_entry_seek_offset(
         libfsxfs_file_entry_t *file_entry,
         off64_t offset,
         int whence,
         libcerror_error_t **error )
{
	libfsxfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                                = "libfsxfs_file_entry_seek_offset";

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsxfs_internal_file_entry_t *) file_entry;

	if( internal_file_entry->inode == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file entry - missing inode.",
		 function );

		return( -1 );
	}
	if( ( internal_file_entry->inode->file_mode & 0xf000 ) != LIBFSXFS_FILE_TYPE_REGULAR_FILE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid file entry - invalid inode - unsupported file mode not a regular file.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	offset = libfdata_stream_seek_offset(
	          internal_file_entry->data_stream,
	          offset,
	          whence,
	          error );

	if( offset == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset in data stream.",
		 function );

		offset = -1;
	}
#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( offset );
}

/* Retrieves the current offset of the data
 * Returns the offset if successful or -1 on error
 */
int libfsxfs_file_entry_get_offset(
     libfsxfs_file_entry_t *file_entry,
     off64_t *offset,
     libcerror_error_t **error )
{
	libfsxfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsxfs_file_entry_get_offset";
	int result                                          = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsxfs_internal_file_entry_t *) file_entry;

	if( internal_file_entry->inode == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file entry - missing inode.",
		 function );

		return( -1 );
	}
	if( ( internal_file_entry->inode->file_mode & 0xf000 ) != LIBFSXFS_FILE_TYPE_REGULAR_FILE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid file entry - invalid inode - unsupported file mode not a regular file.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfdata_stream_get_offset(
	     internal_file_entry->data_stream,
	     offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve offset from data stream.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the size of the data
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_file_entry_get_size(
     libfsxfs_file_entry_t *file_entry,
     size64_t *size,
     libcerror_error_t **error )
{
	libfsxfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsxfs_file_entry_get_size";

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsxfs_internal_file_entry_t *) file_entry;

	if( size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid size.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*size = internal_file_entry->data_size;

#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Retrieves the number of extents of the data
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_file_entry_get_number_of_extents(
     libfsxfs_file_entry_t *file_entry,
     int *number_of_extents,
     libcerror_error_t **error )
{
	libfsxfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsxfs_file_entry_get_number_of_extents";
	int result                                           = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsxfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfsxfs_inode_get_number_of_data_extents(
	     internal_file_entry->inode,
	     number_of_extents,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of data extents from inode.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves a specific extent of the data
 * Returns 1 if successful or -1 on error
 */
int libfsxfs_file_entry_get_extent_by_index(
     libfsxfs_file_entry_t *file_entry,
     int extent_index,
     off64_t *extent_offset,
     size64_t *extent_size,
     uint32_t *extent_flags,
     libcerror_error_t **error )
{
	libfsxfs_extent_t *extent                           = NULL;
	libfsxfs_internal_file_entry_t *internal_file_entry = NULL;
	static char *function                               = "libfsxfs_file_entry_get_extent_by_index";
	int result                                          = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	internal_file_entry = (libfsxfs_internal_file_entry_t *) file_entry;

#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfsxfs_inode_get_data_extent_by_index(
	     internal_file_entry->inode,
	     extent_index,
	     &extent,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data extent: %d from inode.",
		 function,
		 extent_index );

		result = -1;
	}
	if( result == 1 )
	{
		if( libfsxfs_extent_get_values(
		     extent,
		     internal_file_entry->io_handle,
		     extent_offset,
		     extent_size,
		     extent_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve extent: %d values.",
			 function,
			 extent_index );

			result = -1;
		}
	}
#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file_entry->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

