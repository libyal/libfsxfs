/*
 * Python object definition of the sequence and iterator object of extended attributes
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

#if !defined( _PYFSXFS_EXTENDED_ATTRIBUTES_H )
#define _PYFSXFS_EXTENDED_ATTRIBUTES_H

#include <common.h>
#include <types.h>

#include "pyfsxfs_libfsxfs.h"
#include "pyfsxfs_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyfsxfs_extended_attributes pyfsxfs_extended_attributes_t;

struct pyfsxfs_extended_attributes
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The parent object
	 */
	PyObject *parent_object;

	/* The get item by index callback function
	 */
	PyObject* (*get_item_by_index)(
	             PyObject *parent_object,
	             int index );

	/* The current index
	 */
	int current_index;

	/* The number of items
	 */
	int number_of_items;
};

extern PyTypeObject pyfsxfs_extended_attributes_type_object;

PyObject *pyfsxfs_extended_attributes_new(
           PyObject *parent_object,
           PyObject* (*get_item_by_index)(
                        PyObject *parent_object,
                        int index ),
           int number_of_items );

int pyfsxfs_extended_attributes_init(
     pyfsxfs_extended_attributes_t *sequence_object );

void pyfsxfs_extended_attributes_free(
      pyfsxfs_extended_attributes_t *sequence_object );

Py_ssize_t pyfsxfs_extended_attributes_len(
            pyfsxfs_extended_attributes_t *sequence_object );

PyObject *pyfsxfs_extended_attributes_getitem(
           pyfsxfs_extended_attributes_t *sequence_object,
           Py_ssize_t item_index );

PyObject *pyfsxfs_extended_attributes_iter(
           pyfsxfs_extended_attributes_t *sequence_object );

PyObject *pyfsxfs_extended_attributes_iternext(
           pyfsxfs_extended_attributes_t *sequence_object );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYFSXFS_EXTENDED_ATTRIBUTES_H ) */

