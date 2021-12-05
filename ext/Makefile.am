AUTOMAKE_OPTIONS = subdir-objects

noinst_LTLIBRARIES = libfmt.la

noinst_HEADERS = \
	fmt/include/fmt/chrono.h \
	fmt/include/fmt/color.h \
	fmt/include/fmt/compile.h \
	fmt/include/fmt/core.h \
	fmt/include/fmt/format-inl.h \
	fmt/include/fmt/format.h \
	fmt/include/fmt/locale.h \
	fmt/include/fmt/os.h \
	fmt/include/fmt/ostream.h \
	fmt/include/fmt/posix.h \
	fmt/include/fmt/printf.h \
	fmt/include/fmt/ranges.h

libfmt_la_CPPFLAGS = -Ifmt/include
libfmt_la_SOURCES = \
	fmt/src/format.cc \
	fmt/src/os.cc
