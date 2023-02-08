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

#if !defined( _LIBFSXFS_FILE_SYSTEM_H )
#define _LIBFSXFS_FILE_SYSTEM_H

#include <common.h>
#include <types.h>

#include "libfsxfs_directory_entry.h"
#include "libfsxfs_inode.h"
#include "libfsxfs_inode_btree.h"
#include "libfsxfs_libbfio.h"
#include "libfsxfs_libcerror.h"
#include "libfsxfs_libcthreads.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsxfs_file_system libfsxfs_file_system_t;

struct libfsxfs_file_system
{
	/* The inode B+ tree
	 */
	libfsxfs_inode_btree_t *inode_btree;

	/* The root directory inode number
	 */
	uint64_t root_directory_inode_number;

#if defined( HAVE_LIBFSXFS_MULTI_THREAD_SUPPORT )
	/* The read/write lock
	 */
	libcthreads_read_write_lock_t *read_write_lock;
#endif
};

int libfsxfs_file_system_initialize(
     libfsxfs_file_system_t **file_system,
     uint64_t root_directory_inode_number,
     libcerror_error_t **error );

int libfsxfs_file_system_free(
     libfsxfs_file_system_t **file_system,
     libcerror_error_t **error );

int libfsxfs_file_system_read_inode_information(
     libfsxfs_file_system_t *file_system,
     libfsxfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error );

int libfsxfs_file_system_get_inode_by_number(
     libfsxfs_file_system_t *file_system,
     libfsxfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint64_t inode_number,
     libfsxfs_inode_t **inode,
     libcerror_error_t **error );

int libfsxfs_file_system_get_directory_entry_by_utf8_path(
     libfsxfs_file_system_t *file_system,
     libfsxfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     uint64_t *inode_number,
     libfsxfs_inode_t **inode,
     libfsxfs_directory_entry_t **directory_entry,
     libcerror_error_t **error );

int libfsxfs_file_system_get_directory_entry_by_utf16_path(
     libfsxfs_file_system_t *file_system,
     libfsxfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     uint64_t *inode_number,
     libfsxfs_inode_t **inode,
     libfsxfs_directory_entry_t **directory_entry,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSXFS_FILE_SYSTEM_H ) */

