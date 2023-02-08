/*
 * Python object wrapper of libfsxfs_volume_t
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

#if !defined( _PYFSXFS_VOLUME_H )
#define _PYFSXFS_VOLUME_H

#include <common.h>
#include <types.h>

#include "pyfsxfs_libbfio.h"
#include "pyfsxfs_libfsxfs.h"
#include "pyfsxfs_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyfsxfs_volume pyfsxfs_volume_t;

struct pyfsxfs_volume
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libfsxfs volume
	 */
	libfsxfs_volume_t *volume;

	/* The libbfio file IO handle
	 */
	libbfio_handle_t *file_io_handle;
};

extern PyMethodDef pyfsxfs_volume_object_methods[];
extern PyTypeObject pyfsxfs_volume_type_object;

int pyfsxfs_volume_init(
     pyfsxfs_volume_t *pyfsxfs_volume );

void pyfsxfs_volume_free(
      pyfsxfs_volume_t *pyfsxfs_volume );

PyObject *pyfsxfs_volume_signal_abort(
           pyfsxfs_volume_t *pyfsxfs_volume,
           PyObject *arguments );

PyObject *pyfsxfs_volume_open(
           pyfsxfs_volume_t *pyfsxfs_volume,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsxfs_volume_open_file_object(
           pyfsxfs_volume_t *pyfsxfs_volume,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsxfs_volume_close(
           pyfsxfs_volume_t *pyfsxfs_volume,
           PyObject *arguments );

PyObject *pyfsxfs_volume_get_label(
           pyfsxfs_volume_t *pyfsxfs_volume,
           PyObject *arguments );

PyObject *pyfsxfs_volume_get_file_entry_by_inode(
           pyfsxfs_volume_t *pyfsxfs_volume,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfsxfs_volume_get_root_directory(
           pyfsxfs_volume_t *pyfsxfs_volume,
           PyObject *arguments );

PyObject *pyfsxfs_volume_get_file_entry_by_path(
           pyfsxfs_volume_t *pyfsxfs_volume,
           PyObject *arguments,
           PyObject *keywords );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYFSXFS_VOLUME_H ) */

