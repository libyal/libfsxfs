/*
 * Python object wrapper of libfsxfs_extended_attribute_t
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

#if !defined( _PYFSXFS_EXTENDED_ATTRIBUTE_H )
#define _PYFSXFS_EXTENDED_ATTRIBUTE_H

#include <common.h>
#include <types.h>

#include "pyfsxfs_libfsxfs.h"
#include "pyfsxfs_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyfsxfs_extended_attribute pyfsxfs_extended_attribute_t;

struct pyfsxfs_extended_attribute
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libfsxfs extended attribute
	 */
	libfsxfs_extended_attribute_t *extended_attribute;

	/* The parent object
	 */
	PyObject *parent_object;
};

extern PyMethodDef pyfsxfs_extended_attribute_object_methods[];
extern PyTypeObject pyfsxfs_extended_attribute_type_object;

PyObject *pyfsxfs_extended_attribute_new(
           libfsxfs_extended_attribute_t *extended_attribute,
           PyObject *parent_object );

int pyfsxfs_extended_attribute_init(
     pyfsxfs_extended_attribute_t *pyfsxfs_extended_attribute );

void pyfsxfs_extended_attribute_free(
      pyfsxfs_extended_attribute_t *pyfsxfs_extended_attribute );

PyObject *pyfsxfs_extended_attribute_get_name(
           pyfsxfs_extended_attribute_t *pyfsxfs_extended_attribute,
           PyObject *arguments );

PyObject *pyfsxfs_extended_attribute_read_buffer(
           pyfsxfs_extended_attribute_t *pyfsxfs_extended_attribute,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsxfs_extended_attribute_read_buffer_at_offset(
           pyfsxfs_extended_attribute_t *pyfsxfs_extended_attribute,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsxfs_extended_attribute_seek_offset(
           pyfsxfs_extended_attribute_t *pyfsxfs_extended_attribute,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsxfs_extended_attribute_get_offset(
           pyfsxfs_extended_attribute_t *pyfsxfs_extended_attribute,
           PyObject *arguments );

PyObject *pyfsxfs_extended_attribute_get_size(
           pyfsxfs_extended_attribute_t *pyfsxfs_extended_attribute,
           PyObject *arguments );

PyObject *pyfsxfs_extended_attribute_get_number_of_extents(
           pyfsxfs_extended_attribute_t *pyfsxfs_extended_attribute,
           PyObject *arguments );

PyObject *pyfsxfs_extended_attribute_get_extent_by_index(
           pyfsxfs_extended_attribute_t *pyfsxfs_extended_attribute,
           int extent_index );

PyObject *pyfsxfs_extended_attribute_get_extent(
           pyfsxfs_extended_attribute_t *pyfsxfs_extended_attribute,
           PyObject *arguments,
           PyObject *keywords );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYFSXFS_EXTENDED_ATTRIBUTE_H ) */

