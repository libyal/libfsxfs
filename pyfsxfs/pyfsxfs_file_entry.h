/*
 * Python object wrapper of libfsxfs_file_entry_t
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

#if !defined( _PYFSXFS_FILE_ENTRY_H )
#define _PYFSXFS_FILE_ENTRY_H

#include <common.h>
#include <types.h>

#include "pyfsxfs_libfsxfs.h"
#include "pyfsxfs_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyfsxfs_file_entry pyfsxfs_file_entry_t;

struct pyfsxfs_file_entry
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libfsxfs file entry
	 */
	libfsxfs_file_entry_t *file_entry;

	/* The parent object
	 */
	PyObject *parent_object;
};

extern PyMethodDef pyfsxfs_file_entry_object_methods[];
extern PyTypeObject pyfsxfs_file_entry_type_object;

PyObject *pyfsxfs_file_entry_new(
           libfsxfs_file_entry_t *file_entry,
           PyObject *parent_object );

int pyfsxfs_file_entry_init(
     pyfsxfs_file_entry_t *pyfsxfs_file_entry );

void pyfsxfs_file_entry_free(
      pyfsxfs_file_entry_t *pyfsxfs_file_entry );

PyObject *pyfsxfs_file_entry_get_inode_number(
           pyfsxfs_file_entry_t *pyfsxfs_file_entry,
           PyObject *arguments );

PyObject *pyfsxfs_file_entry_get_creation_time(
           pyfsxfs_file_entry_t *pyfsxfs_file_entry,
           PyObject *arguments );

PyObject *pyfsxfs_file_entry_get_creation_time_as_integer(
           pyfsxfs_file_entry_t *pyfsxfs_file_entry,
           PyObject *arguments );

PyObject *pyfsxfs_file_entry_get_modification_time(
           pyfsxfs_file_entry_t *pyfsxfs_file_entry,
           PyObject *arguments );

PyObject *pyfsxfs_file_entry_get_modification_time_as_integer(
           pyfsxfs_file_entry_t *pyfsxfs_file_entry,
           PyObject *arguments );

PyObject *pyfsxfs_file_entry_get_inode_change_time(
           pyfsxfs_file_entry_t *pyfsxfs_file_entry,
           PyObject *arguments );

PyObject *pyfsxfs_file_entry_get_inode_change_time_as_integer(
           pyfsxfs_file_entry_t *pyfsxfs_file_entry,
           PyObject *arguments );

PyObject *pyfsxfs_file_entry_get_access_time(
           pyfsxfs_file_entry_t *pyfsxfs_file_entry,
           PyObject *arguments );

PyObject *pyfsxfs_file_entry_get_access_time_as_integer(
           pyfsxfs_file_entry_t *pyfsxfs_file_entry,
           PyObject *arguments );

PyObject *pyfsxfs_file_entry_get_file_mode(
           pyfsxfs_file_entry_t *pyfsxfs_file_entry,
           PyObject *arguments );

PyObject *pyfsxfs_file_entry_get_number_of_links(
           pyfsxfs_file_entry_t *pyfsxfs_file_entry,
           PyObject *arguments );

PyObject *pyfsxfs_file_entry_get_owner_identifier(
           pyfsxfs_file_entry_t *pyfsxfs_file_entry,
           PyObject *arguments );

PyObject *pyfsxfs_file_entry_get_group_identifier(
           pyfsxfs_file_entry_t *pyfsxfs_file_entry,
           PyObject *arguments );

PyObject *pyfsxfs_file_entry_get_device_number(
           pyfsxfs_file_entry_t *pyfsxfs_file_entry,
           PyObject *arguments );

PyObject *pyfsxfs_file_entry_get_name(
           pyfsxfs_file_entry_t *pyfsxfs_file_entry,
           PyObject *arguments );

PyObject *pyfsxfs_file_entry_get_symbolic_link_target(
           pyfsxfs_file_entry_t *pyfsxfs_file_entry,
           PyObject *arguments );

PyObject *pyfsxfs_file_entry_get_number_of_extended_attributes(
           pyfsxfs_file_entry_t *pyfsxfs_file_entry,
           PyObject *arguments );

PyObject *pyfsxfs_file_entry_get_extended_attribute_by_index(
           PyObject *pyfsxfs_file_entry,
           int extended_attribute_index );

PyObject *pyfsxfs_file_entry_get_extended_attribute(
           pyfsxfs_file_entry_t *pyfsxfs_file_entry,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsxfs_file_entry_get_extended_attributes(
           pyfsxfs_file_entry_t *pyfsxfs_file_entry,
           PyObject *arguments );

PyObject *pyfsxfs_file_entry_has_extended_attribute_by_name(
           pyfsxfs_file_entry_t *pyfsxfs_file_entry,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsxfs_file_entry_get_extended_attribute_by_name(
           pyfsxfs_file_entry_t *pyfsxfs_file_entry,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsxfs_file_entry_get_number_of_sub_file_entries(
           pyfsxfs_file_entry_t *pyfsxfs_file_entry,
           PyObject *arguments );

PyObject *pyfsxfs_file_entry_get_sub_file_entry_by_index(
           PyObject *pyfsxfs_file_entry,
           int sub_file_entry_index );

PyObject *pyfsxfs_file_entry_get_sub_file_entry(
           pyfsxfs_file_entry_t *pyfsxfs_file_entry,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsxfs_file_entry_get_sub_file_entries(
           pyfsxfs_file_entry_t *pyfsxfs_file_entry,
           PyObject *arguments );

PyObject *pyfsxfs_file_entry_get_sub_file_entry_by_name(
           pyfsxfs_file_entry_t *pyfsxfs_file_entry,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsxfs_file_entry_read_buffer(
           pyfsxfs_file_entry_t *pyfsxfs_file_entry,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsxfs_file_entry_read_buffer_at_offset(
           pyfsxfs_file_entry_t *pyfsxfs_file_entry,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsxfs_file_entry_seek_offset(
           pyfsxfs_file_entry_t *pyfsxfs_file_entry,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsxfs_file_entry_get_offset(
           pyfsxfs_file_entry_t *pyfsxfs_file_entry,
           PyObject *arguments );

PyObject *pyfsxfs_file_entry_get_size(
           pyfsxfs_file_entry_t *pyfsxfs_file_entry,
           PyObject *arguments );

PyObject *pyfsxfs_file_entry_get_number_of_extents(
           pyfsxfs_file_entry_t *pyfsxfs_file_entry,
           PyObject *arguments );

PyObject *pyfsxfs_file_entry_get_extent_by_index(
           pyfsxfs_file_entry_t *pyfsxfs_file_entry,
           int extent_index );

PyObject *pyfsxfs_file_entry_get_extent(
           pyfsxfs_file_entry_t *pyfsxfs_file_entry,
           PyObject *arguments,
           PyObject *keywords );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYFSXFS_FILE_ENTRY_H ) */

