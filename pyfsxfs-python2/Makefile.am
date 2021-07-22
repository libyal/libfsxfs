# Note that we cannot use: AUTOMAKE_OPTIONS = subdir-objects
# subdir-objects will compile the source files to a single version of Python.
# Since subdir-objects is being deprecated we copy the source files instead.
am__installdirs = "$(DESTDIR)$(pyexecdir2)"
pyexecdir = $(pyexecdir2)

if HAVE_PYTHON2
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
	@LIBFGUID_CPPFLAGS@ \
	@LIBFSXFS_DLL_IMPORT@

am_pyfsxfs_la_rpath = -rpath $(pyexecdir2)

BUILT_SOURCES = \
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

pyexec_LTLIBRARIES = pyfsxfs.la

nodist_pyfsxfs_la_SOURCES = $(BUILT_SOURCES)

pyfsxfs_la_LIBADD = \
	@LIBCERROR_LIBADD@ \
	../libfsxfs/libfsxfs.la \
	@LIBCDATA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBFGUID_LIBADD@

pyfsxfs_la_CPPFLAGS = $(PYTHON2_CPPFLAGS)
pyfsxfs_la_LDFLAGS  = -module -avoid-version $(PYTHON2_LDFLAGS)

$(BUILT_SOURCES):
	/bin/cp -f $(top_srcdir)/pyfsxfs/$@ $@

endif

MAINTAINERCLEANFILES = \
	Makefile.in

clean-local:
	/bin/rm -f pyfsxfs*.[ch]

distclean: clean
	/bin/rm -f Makefile

