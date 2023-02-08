/*
 * Extent B+ tree functions
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

#if !defined( _LIBFSXFS_EXTENT_BTREE_H )
#define _LIBFSXFS_EXTENT_BTREE_H

#include <common.h>
#include <types.h>

#include "libfsxfs_io_handle.h"
#include "libfsxfs_libbfio.h"
#include "libfsxfs_libcdata.h"
#include "libfsxfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsxfs_extent_btree libfsxfs_extent_btree_t;

struct libfsxfs_extent_btree
{
	/* B+ tree maximum depth
	 */
	uint32_t maximum_depth;
};

int libfsxfs_extent_btree_initialize(
     libfsxfs_extent_btree_t **extent_btree,
     libcerror_error_t **error );

int libfsxfs_extent_btree_free(
     libfsxfs_extent_btree_t **extent_btree,
     libcerror_error_t **error );

int libfsxfs_extent_btree_get_extents_from_root_node(
     libfsxfs_extent_btree_t *extent_btree,
     libfsxfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint64_t number_of_blocks,
     const uint8_t *data,
     size_t data_size,
     libcdata_array_t *extents_array,
     uint8_t add_sparse_extents,
     libcerror_error_t **error );

int libfsxfs_extent_btree_get_extents_from_branch_node(
     libfsxfs_extent_btree_t *extent_btree,
     libfsxfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint16_t number_of_records,
     const uint8_t *records_data,
     size_t records_data_size,
     libcdata_array_t *extents_array,
     uint8_t add_sparse_extents,
     int recursion_depth,
     libcerror_error_t **error );

int libfsxfs_extent_btree_get_extents_from_node(
     libfsxfs_extent_btree_t *extent_btree,
     libfsxfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint64_t block_number,
     libcdata_array_t *extents_array,
     uint8_t add_sparse_extents,
     int recursion_depth,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSXFS_EXTENT_BTREE_H ) */

