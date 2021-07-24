/*
 * (Extended) attributes block functions
 *
 * Copyright (C) 2020-2021, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBFSXFS_ATTRIBUTES_BLOCK_H )
#define _LIBFSXFS_ATTRIBUTES_BLOCK_H

#include <common.h>
#include <types.h>

#include "libfsxfs_attributes_block_header.h"
#include "libfsxfs_io_handle.h"
#include "libfsxfs_libbfio.h"
#include "libfsxfs_libcdata.h"
#include "libfsxfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsxfs_attributes_block libfsxfs_attributes_block_t;

struct libfsxfs_attributes_block
{
	/* Data
	 */
	uint8_t *data;

	/* Data size
	 */
	size_t data_size;

	/* The header
	 */
	libfsxfs_attributes_block_header_t *header;
};

int libfsxfs_attributes_block_initialize(
     libfsxfs_attributes_block_t **attributes_block,
     size_t block_size,
     libcerror_error_t **error );

int libfsxfs_attributes_block_free(
     libfsxfs_attributes_block_t **attributes_block,
     libcerror_error_t **error );

int libfsxfs_attributes_block_read_data(
     libfsxfs_attributes_block_t *attributes_block,
     libfsxfs_io_handle_t *io_handle,
     const uint8_t *data,
     size_t data_size,
     libcdata_array_t *entries_array,
     libcerror_error_t **error );

int libfsxfs_attributes_block_read_file_io_handle(
     libfsxfs_attributes_block_t *attributes_block,
     libfsxfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcdata_array_t *entries_array,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSXFS_ATTRIBUTES_BLOCK_H ) */

