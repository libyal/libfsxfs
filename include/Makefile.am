include_HEADERS = \
	libfsxfs.h

pkginclude_HEADERS = \
	libfsxfs/codepage.h \
	libfsxfs/definitions.h \
	libfsxfs/error.h \
	libfsxfs/extern.h \
	libfsxfs/features.h \
	libfsxfs/types.h

EXTRA_DIST = \
	libfsxfs.h.in \
	libfsxfs/definitions.h.in \
	libfsxfs/features.h.in \
	libfsxfs/types.h.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	-rm -f libfsxfs.h
	-rm -f libfsxfs/definitions.h
	-rm -f libfsxfs/features.h
	-rm -f libfsxfs/types.h
	-rm -f Makefile

