/*
 * Inode B+ tree functions
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

#if !defined( _LIBFSXFS_INODE_BTREE_H )
#define _LIBFSXFS_INODE_BTREE_H

#include <common.h>
#include <types.h>

#include "libfsxfs_inode_btree_record.h"
#include "libfsxfs_inode_information.h"
#include "libfsxfs_io_handle.h"
#include "libfsxfs_libbfio.h"
#include "libfsxfs_libcdata.h"
#include "libfsxfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsxfs_inode_btree libfsxfs_inode_btree_t;

struct libfsxfs_inode_btree
{
	/* Inode information array
	 */
	libcdata_array_t *inode_information_array;
};

int libfsxfs_inode_btree_initialize(
     libfsxfs_inode_btree_t **inode_btree,
     libcerror_error_t **error );

int libfsxfs_inode_btree_free(
     libfsxfs_inode_btree_t **inode_btree,
     libcerror_error_t **error );

int libfsxfs_inode_btree_read_inode_information(
     libfsxfs_inode_btree_t *inode_btree,
     libfsxfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error );

int libfsxfs_inode_btree_get_inode_from_branch_node(
     libfsxfs_inode_btree_t *inode_btree,
     libfsxfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint64_t allocation_group_block_number,
     uint16_t number_of_records,
     const uint8_t *records_data,
     size_t records_data_size,
     uint64_t relative_inode_number,
     int recursion_depth,
     libcerror_error_t **error );

int libfsxfs_inode_btree_get_inode_from_leaf_node(
     libfsxfs_inode_btree_t *inode_btree,
     uint16_t number_of_records,
     const uint8_t *records_data,
     size_t records_data_size,
     uint64_t relative_inode_number,
     libcerror_error_t **error );

int libfsxfs_inode_btree_get_inode_from_node(
     libfsxfs_inode_btree_t *inode_btree,
     libfsxfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint64_t allocation_group_block_number,
     uint64_t relative_block_number,
     uint64_t relative_inode_number,
     int recursion_depth,
     libcerror_error_t **error );

int libfsxfs_inode_btree_get_inode_by_number(
     libfsxfs_inode_btree_t *inode_btree,
     libfsxfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint64_t absolute_inode_number,
     off64_t *file_offset,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSXFS_INODE_BTREE_H ) */

