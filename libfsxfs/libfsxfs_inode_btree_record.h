/*
 * Inode B+ tree record functions
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

#if !defined( _LIBFSXFS_INODE_BTREE_RECORD_H )
#define _LIBFSXFS_INODE_BTREE_RECORD_H

#include <common.h>
#include <types.h>

#include "libfsxfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsxfs_inode_btree_record libfsxfs_inode_btree_record_t;

struct libfsxfs_inode_btree_record
{
	/* Inode number
	 */
	uint32_t inode_number;

	/* Number of unused inodes
	 */
	uint32_t number_of_unused_inodes;

	/* Chunk allocation bitmap
	 */
	uint64_t chunk_allocation_bitmap;
};

int libfsxfs_inode_btree_record_initialize(
     libfsxfs_inode_btree_record_t **inode_btree_record,
     libcerror_error_t **error );

int libfsxfs_inode_btree_record_free(
     libfsxfs_inode_btree_record_t **inode_btree_record,
     libcerror_error_t **error );

int libfsxfs_inode_btree_record_read_data(
     libfsxfs_inode_btree_record_t *inode_btree_record,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSXFS_INODE_BTREE_RECORD_H ) */

