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
	/bin/rm -f libfsxfs.h
	/bin/rm -f libfsxfs/definitions.h
	/bin/rm -f libfsxfs/features.h
	/bin/rm -f libfsxfs/types.h
	/bin/rm -f Makefile

