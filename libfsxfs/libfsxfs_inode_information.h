/*
 * Inode information functions
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

#if !defined( _LIBFSXFS_INODE_INFORMATION_H )
#define _LIBFSXFS_INODE_INFORMATION_H

#include <common.h>
#include <types.h>

#include "libfsxfs_io_handle.h"
#include "libfsxfs_libbfio.h"
#include "libfsxfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfsxfs_inode_information libfsxfs_inode_information_t;

struct libfsxfs_inode_information
{
	/* Inode B+ tree root block number
	 */
	uint32_t inode_btree_root_block_number;

	/* Inode B+ tree depth
	 */
	uint32_t inode_btree_depth;

	/* Last allocated chunk
	 */
	uint32_t last_allocated_chunk;
};

int libfsxfs_inode_information_initialize(
     libfsxfs_inode_information_t **inode_information,
     libcerror_error_t **error );

int libfsxfs_inode_information_free(
     libfsxfs_inode_information_t **inode_information,
     libcerror_error_t **error );

int libfsxfs_inode_information_read_data(
     libfsxfs_inode_information_t *inode_information,
     libfsxfs_io_handle_t *io_handle,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsxfs_inode_information_read_file_io_handle(
     libfsxfs_inode_information_t *inode_information,
     libfsxfs_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSXFS_INODE_INFORMATION_H ) */

