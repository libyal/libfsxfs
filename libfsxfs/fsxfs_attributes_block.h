/*
 * (Extended) attributes block structures of the X File System (XFS)
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

#if !defined( _FSXFS_ATTRIBUTES_BLOCK_H )
#define _FSXFS_ATTRIBUTES_BLOCK_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fsxfs_attributes_branch_block_header_v2 fsxfs_attributes_branch_block_header_v2_t;

struct fsxfs_attributes_branch_block_header_v2
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

typedef struct fsxfs_attributes_branch_block_header_v3 fsxfs_attributes_branch_block_header_v3_t;

struct fsxfs_attributes_branch_block_header_v3
{
	/* Number of entries
	 * Consists of 2 bytes
	 */
	uint8_t number_of_entries[ 2 ];

	/* Node level size
	 * Consists of 2 bytes
	 */
	uint8_t node_level[ 2 ];

	/* Unknown (padding)
	 * Consists of 4 bytes
	 */
	uint8_t unknown1[ 4 ];
};

typedef struct fsxfs_attributes_branch_block_entry fsxfs_attributes_branch_block_entry_t;

struct fsxfs_attributes_branch_block_entry
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

typedef struct fsxfs_attributes_leaf_block_header_v2 fsxfs_attributes_leaf_block_header_v2_t;

struct fsxfs_attributes_leaf_block_header_v2
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

typedef struct fsxfs_attributes_leaf_block_header_v3 fsxfs_attributes_leaf_block_header_v3_t;

struct fsxfs_attributes_leaf_block_header_v3
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

	/* Unknown (padding)
	 * Consists of 4 bytes
	 */
	uint8_t unknown3[ 4 ];
};

typedef struct fsxfs_attributes_leaf_block_entry fsxfs_attributes_leaf_block_entry_t;

struct fsxfs_attributes_leaf_block_entry
{
	/* Name hash
	 * Consists of 4 bytes
	 */
	uint8_t name_hash[ 4 ];

	/* Values offset
	 * Consists of 2 bytes
	 */
	uint8_t values_offset[ 2 ];

	/* Flags
	 * Consists of 1 byte
	 */
	uint8_t flags;

	/* Unknown (padding)
	 * Consists of 1 byte
	 */
	uint8_t unknown1;
};

typedef struct fsxfs_attributes_block_values_local fsxfs_attributes_block_values_local_t;

struct fsxfs_attributes_block_values_local
{
	/* Value data size
	 * Consists of 2 bytes
	 */
	uint8_t value_data_size[ 2 ];

	/* Name size
	 * Consists of 1 byte
	 */
	uint8_t name_size;
};

typedef struct fsxfs_attributes_block_values_remote fsxfs_attributes_block_values_remote_t;

struct fsxfs_attributes_block_values_remote
{
	/* Value data block number
	 * Consists of 4 bytes
	 */
	uint8_t value_data_block_number[ 4 ];

	/* Value data size
	 * Consists of 4 bytes
	 */
	uint8_t value_data_size[ 4 ];

	/* Name size
	 * Consists of 1 byte
	 */
	uint8_t name_size;
};

typedef struct fsxfs_attributes_remote_value_data_block_header_v3 fsxfs_attributes_remote_value_data_block_header_v3_t;

struct fsxfs_attributes_remote_value_data_block_header_v3
{
	/* Signature
	 * Consists of 4 bytes
	 */
	uint8_t signature[ 4 ];

	/* Value data offset
	 * Consists of 4 bytes
	 */
	uint8_t value_data_offset[ 4 ];

	/* Value data size
	 * Consists of 4 bytes
	 */
	uint8_t value_data_size[ 4 ];

	/* Checksum
	 * Consists of 4 bytes
	 */
	uint8_t checksum[ 4 ];

	/* Block type identifier
	 * Consists of 16 bytes
	 */
	uint8_t block_type_identifier[ 16 ];

	/* Owner inode number
	 * Consists of 8 bytes
	 */
	uint8_t owner_inode_number[ 8 ];

	/* Block number
	 * Consists of 8 bytes
	 */
	uint8_t block_number[ 8 ];

	/* Log sequence number
	 * Consists of 8 bytes
	 */
	uint8_t log_sequence_number[ 8 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSXFS_ATTRIBUTES_BLOCK_H ) */

