/*
 * Extent functions
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

#if !defined( _LIBFSXFS_EXTENT_H )
#define _LIBFSXFS_EXTENT_H

#include <common.h>
#include <types.h>

#include "libfsxfs_io_handle.h"
#include "libfsxfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsxfs_extent libfsxfs_extent_t;

struct libfsxfs_extent
{
	/* Logical block number
	 */
	uint64_t logical_block_number;

	/* Physical block number
	 */
	uint64_t physical_block_number;

	/* Number of blocks
	 */
	uint32_t number_of_blocks;

	/* The range flags
	 */
	uint32_t range_flags;
};

int libfsxfs_extent_initialize(
     libfsxfs_extent_t **extent,
     libcerror_error_t **error );

int libfsxfs_extent_free(
     libfsxfs_extent_t **extent,
     libcerror_error_t **error );

int libfsxfs_extent_read_data(
     libfsxfs_extent_t *extent,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsxfs_extent_get_values(
     libfsxfs_extent_t *extent,
     libfsxfs_io_handle_t *io_handle,
     off64_t *extent_offset,
     size64_t *extent_size,
     uint32_t *extent_flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSXFS_EXTENT_H ) */

