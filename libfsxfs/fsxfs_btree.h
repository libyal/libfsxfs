/*
 * B+ tree structures of the X File System (XFS)
 *
 * Copyright (C) 2020, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _FSXFS_BTREE_H )
#define _FSXFS_BTREE_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fsxfs_btree_header fsxfs_btree_header_t;

struct fsxfs_btree_header
{
	/* Signature
	 * Consists of 4 bytes
	 */
	uint8_t signature[ 4 ];

	/* Level
	 * Consists of 2 bytes
	 */
	uint8_t level[ 2 ];

	/* Number of records
	 * Consists of 2 bytes
	 */
	uint8_t number_of_records[ 2 ];

	/* Previous B+ tree block number
	 * Consists of 4 bytes
	 */
	uint8_t previous_btree_block_number[ 4 ];

	/* Next B+ tree block number
	 * Consists of 4 bytes
	 */
	uint8_t next_btree_block_number[ 4 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FSXFS_BTREE_H ) */

