/*
 * Short-form directory table functions
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

#if !defined( _LIBFSXFS_DIRECTORY_TABLE_H )
#define _LIBFSXFS_DIRECTORY_TABLE_H

#include <common.h>
#include <types.h>

#include "libfsxfs_directory_table_header.h"
#include "libfsxfs_io_handle.h"
#include "libfsxfs_libcdata.h"
#include "libfsxfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsxfs_directory_table libfsxfs_directory_table_t;

struct libfsxfs_directory_table
{
	/* The header
	 */
	libfsxfs_directory_table_header_t *header;
};

int libfsxfs_directory_table_initialize(
     libfsxfs_directory_table_t **directory_table,
     libcerror_error_t **error );

int libfsxfs_directory_table_free(
     libfsxfs_directory_table_t **directory_table,
     libcerror_error_t **error );

int libfsxfs_directory_table_read_data(
     libfsxfs_directory_table_t *directory_table,
     libfsxfs_io_handle_t *io_handle,
     const uint8_t *data,
     size_t data_size,
     libcdata_array_t *entries_array,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSXFS_DIRECTORY_TABLE_H ) */

