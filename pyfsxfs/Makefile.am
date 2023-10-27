if HAVE_PYTHON
AM_CFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFSXFS_DLL_IMPORT@

pyexec_LTLIBRARIES = pyfsxfs.la

pyfsxfs_la_SOURCES = \
	pyfsxfs.c pyfsxfs.h \
	pyfsxfs_datetime.c pyfsxfs_datetime.h \
	pyfsxfs_error.c pyfsxfs_error.h \
	pyfsxfs_extended_attribute.c pyfsxfs_extended_attribute.h \
	pyfsxfs_extended_attributes.c pyfsxfs_extended_attributes.h \
	pyfsxfs_file_entries.c pyfsxfs_file_entries.h \
	pyfsxfs_file_entry.c pyfsxfs_file_entry.h \
	pyfsxfs_file_object_io_handle.c pyfsxfs_file_object_io_handle.h \
	pyfsxfs_integer.c pyfsxfs_integer.h \
	pyfsxfs_libbfio.h \
	pyfsxfs_libcerror.h \
	pyfsxfs_libfsxfs.h \
	pyfsxfs_python.h \
	pyfsxfs_unused.h \
	pyfsxfs_volume.c pyfsxfs_volume.h

pyfsxfs_la_LIBADD = \
	@LIBCERROR_LIBADD@ \
	../libfsxfs/libfsxfs.la \
	@LIBCDATA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBBFIO_LIBADD@

pyfsxfs_la_CPPFLAGS = $(PYTHON_CPPFLAGS)
pyfsxfs_la_LDFLAGS  = -module -avoid-version $(PYTHON_LDFLAGS)

endif

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	-rm -f setup.py
	-rm -f Makefile

