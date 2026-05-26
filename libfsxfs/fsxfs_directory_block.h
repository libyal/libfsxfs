/*
 * Directory block structures of the X File System (XFS)
 *
 * Copyright (C) 2020-2026, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _FSXFS_DIRECTORY_BLOCK_H )
#define _FSXFS_DIRECTORY_BLOCK_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fsxfs_directory_branch_block_header_v1 fsxfs_directory_branch_block_header_v1_t;

struct fsxfs_directory_branch_block_header_v1
{
	/* Number of entries
	 * Consists of 2 bytes
	 */
	uint8_t number_of_entries[ 2 ];

	/* Node level size
	 * Consists of 2 bytes
	 */
	uint8_t node_level[ 2 ];
};

typedef struct fsxfs_directory_branch_block_entry_v1 fsxfs_directory_branch_block_entry_v1_t;

struct fsxfs_directory_branch_block_entry_v1
{
	/* Name hash
	 * Consists of 4 bytes
	 */
	uint8_t name_hash[ 4 ];

	/* Sub block number
	 * Consists of 4 bytes
	 */
	uint8_t sub_block_number[ 4 ];
};

typedef struct fsxfs_directory_leaf_block_header_v1 fsxfs_directory_leaf_block_header_v1_t;

struct fsxfs_directory_leaf_block_header_v1
{
	/* Number of entries
	 * Consists of 2 bytes
	 */
	uint8_t number_of_entries[ 2 ];

	/* Used data size
	 * Consists of 2 bytes
	 */
	uint8_t used_data_size[ 2 ];

	/* Used data offset
	 * Consists of 2 bytes
	 */
	uint8_t used_data_offset[ 2 ];

	/* Block compaction flag
	 * Consists of 1 byte
	 */
	uint8_t block_compaction_flag;

	/* Unknown (padding)
	 * Consists of 1 byte
	 */
	uint8_t unknown2;

	/* Free regions
	 * Consists of 12 bytes
	 */
	uint8_t free_regions[ 12 ];
};

typedef struct fsxfs_directory_leaf_block_entry_v1 fsxfs_directory_leaf_block_entry_v1_t;

struct fsxfs_directory_leaf_block_entry_v1
{
	/* Name hash
	 * Consists of 4 bytes
	 */
	uint8_t name_hash[ 4 ];

	/* Values offset
	 * Consists of 2 bytes
	 */
	uint8_t values_offset[ 2 ];

	/* Name size
	 * Consists of 1 byte
	 */
	uint8_t name_size;

	/* Unknown (padding)
	 * Consists of 1 byte
	 */
	uint8_t unknown1;
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSXFS_DIRECTORY_BLOCK_H ) */

