/*
 * File entry functions
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

#if !defined( _LIBFSXFS_FILE_ENTRY_H )
#define _LIBFSXFS_FILE_ENTRY_H

#include <common.h>
#include <types.h>

#include "libfsxfs_extern.h"
#include "libfsxfs_inode.h"
#include "libfsxfs_libbfio.h"
#include "libfsxfs_libcerror.h"
#include "libfsxfs_libcthreads.h"
#include "libfsxfs_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsxfs_internal_file_entry libfsxfs_internal_file_entry_t;

struct libfsxfs_internal_file_entry
{
	/* The file IO handle
	 */
	libbfio_handle_t *file_io_handle;

	/* The inode
	 */
	libfsxfs_inode_t *inode;

#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	/* The read/write lock
	 */
	libcthreads_read_write_lock_t *read_write_lock;
#endif
};

int libfsxfs_file_entry_initialize(
     libfsxfs_file_entry_t **file_entry,
     libbfio_handle_t *file_io_handle,
     libfsxfs_inode_t *inode,
     libcerror_error_t **error );

LIBFSXFS_EXTERN \
int libfsxfs_file_entry_free(
     libfsxfs_file_entry_t **file_entry,
     libcerror_error_t **error );

LIBFSXFS_EXTERN \
int libfsxfs_file_entry_get_creation_time(
     libfsxfs_file_entry_t *file_entry,
     int64_t *posix_time,
     libcerror_error_t **error );

LIBFSXFS_EXTERN \
int libfsxfs_file_entry_get_modification_time(
     libfsxfs_file_entry_t *file_entry,
     int64_t *posix_time,
     libcerror_error_t **error );

LIBFSXFS_EXTERN \
int libfsxfs_file_entry_get_access_time(
     libfsxfs_file_entry_t *file_entry,
     int64_t *posix_time,
     libcerror_error_t **error );

LIBFSXFS_EXTERN \
int libfsxfs_file_entry_get_inode_change_time(
     libfsxfs_file_entry_t *file_entry,
     int64_t *posix_time,
     libcerror_error_t **error );

LIBFSXFS_EXTERN \
int libfsxfs_file_entry_get_owner_identifier(
     libfsxfs_file_entry_t *file_entry,
     uint32_t *owner_identifier,
     libcerror_error_t **error );

LIBFSXFS_EXTERN \
int libfsxfs_file_entry_get_group_identifier(
     libfsxfs_file_entry_t *file_entry,
     uint32_t *group_identifier,
     libcerror_error_t **error );

LIBFSXFS_EXTERN \
int libfsxfs_file_entry_get_file_mode(
     libfsxfs_file_entry_t *file_entry,
     uint16_t *file_mode,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSXFS_FILE_ENTRY_H ) */

