/*
 * Directory functions
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

#if !defined( _LIBFSXFS_DIRECTORY_H )
#define _LIBFSXFS_DIRECTORY_H

#include <common.h>
#include <types.h>

#include "libfsxfs_directory_entry.h"
#include "libfsxfs_inode.h"
#include "libfsxfs_io_handle.h"
#include "libfsxfs_libbfio.h"
#include "libfsxfs_libcdata.h"
#include "libfsxfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsxfs_directory libfsxfs_directory_t;

struct libfsxfs_directory
{
	/* Entries array
	 */
	libcdata_array_t *entries_array;
};

int libfsxfs_directory_initialize(
     libfsxfs_directory_t **directory,
     libcerror_error_t **error );

int libfsxfs_directory_free(
     libfsxfs_directory_t **directory,
     libcerror_error_t **error );

int libfsxfs_directory_read_file_io_handle(
     libfsxfs_directory_t *directory,
     libfsxfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfsxfs_inode_t *inode,
     libcerror_error_t **error );

int libfsxfs_directory_get_number_of_entries(
     libfsxfs_directory_t *directory,
     int *number_of_entries,
     libcerror_error_t **error );

int libfsxfs_directory_get_entry_by_index(
     libfsxfs_directory_t *directory,
     int entry_index,
     libfsxfs_directory_entry_t **directory_entry,
     libcerror_error_t **error );

int libfsxfs_directory_get_entry_by_utf8_name(
     libfsxfs_directory_t *directory,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfsxfs_directory_entry_t **directory_entry,
     libcerror_error_t **error );

int libfsxfs_directory_get_entry_by_utf16_name(
     libfsxfs_directory_t *directory,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfsxfs_directory_entry_t **directory_entry,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSXFS_DIRECTORY_H ) */

