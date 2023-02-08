/*
 * Superblock functions
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

#if !defined( _LIBFSXFS_SUPERBLOCK_H )
#define _LIBFSXFS_SUPERBLOCK_H

#include <common.h>
#include <types.h>

#include "libfsxfs_libbfio.h"
#include "libfsxfs_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

extern const char *fsxfs_superblock_signature;

typedef struct libfsxfs_superblock libfsxfs_superblock_t;

struct libfsxfs_superblock
{
	/* Block size
	 */
	uint32_t block_size;

	/* Number of blocks
	 */
	uint64_t number_of_blocks;

	/* Journal block number
	 */
	uint64_t journal_block_number;

	/* Root directory inode number
	 */
	uint64_t root_directory_inode_number;

	/* Allocation group size
	 */
	uint32_t allocation_group_size;

	/* Number of allocation groups
	 */
	uint32_t number_of_allocation_groups;

	/* Format version
	 */
	uint8_t format_version;

	/* Feature flags
	 */
	uint16_t feature_flags;

	/* Sector size
	 */
	uint16_t sector_size;

	/* Inode size
	 */
	uint16_t inode_size;

	/* Directory block size
	 */
	uint32_t directory_block_size;

	/* Volume label
	 */
	uint8_t volume_label[ 12 ];

	/* Secondary feature flags
	 */
	uint32_t secondary_feature_flags;

	/* Number of bits used for the relative block number
	 */
	uint8_t number_of_relative_block_number_bits;

	/* Number of bits used for the relative inode number
	 */
	uint8_t number_of_relative_inode_number_bits;
};

int libfsxfs_superblock_initialize(
     libfsxfs_superblock_t **superblock,
     libcerror_error_t **error );

int libfsxfs_superblock_free(
     libfsxfs_superblock_t **superblock,
     libcerror_error_t **error );

int libfsxfs_superblock_read_data(
     libfsxfs_superblock_t *superblock,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfsxfs_superblock_read_file_io_handle(
     libfsxfs_superblock_t *superblock,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error );

int libfsxfs_superblock_get_utf8_volume_label_size(
     libfsxfs_superblock_t *superblock,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libfsxfs_superblock_get_utf8_volume_label(
     libfsxfs_superblock_t *superblock,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libfsxfs_superblock_get_utf16_volume_label_size(
     libfsxfs_superblock_t *superblock,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libfsxfs_superblock_get_utf16_volume_label(
     libfsxfs_superblock_t *superblock,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFSXFS_SUPERBLOCK_H ) */

