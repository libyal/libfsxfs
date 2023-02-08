/*
 * Block directory structures of the X File System (XFS)
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

#if !defined( _FSXFS_BLOCK_DIRECTORY_H )
#define _FSXFS_BLOCK_DIRECTORY_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fsxfs_block_directory_header_v2 fsxfs_block_directory_header_v2_t;

struct fsxfs_block_directory_header_v2
{
	/* Signature
	 * Consists of 4 bytes
	 */
	uint8_t signature[ 4 ];

	/* Free regions
	 * Consists of 12 bytes
	 */
	uint8_t free_regions[ 12 ];
};

typedef struct fsxfs_block_directory_header_v3 fsxfs_block_directory_header_v3_t;

struct fsxfs_block_directory_header_v3
{
	/* Signature
	 * Consists of 4 bytes
	 */
	uint8_t signature[ 4 ];

	/* Checksum
	 * Consists of 4 bytes
	 */
	uint8_t checksum[ 4 ];

	/* Block number
	 * Consists of 8 bytes
	 */
	uint8_t block_number[ 8 ];

	/* Log sequence number
	 * Consists of 8 bytes
	 */
	uint8_t log_sequence_number[ 8 ];

	/* Block type identifier
	 * Consists of 16 bytes
	 */
	uint8_t block_type_identifier[ 16 ];

	/* Owner inode number
	 * Consists of 8 bytes
	 */
	uint8_t owner_inode_number[ 8 ];

	/* Free regions
	 * Consists of 12 bytes
	 */
	uint8_t free_regions[ 12 ];

	/* Unknown (padding)
	 * Consists of 4 bytes
	 */
	uint8_t unknown1[ 4 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSXFS_BLOCK_DIRECTORY_H ) */

