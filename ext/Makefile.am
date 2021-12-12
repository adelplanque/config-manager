AUTOMAKE_OPTIONS = subdir-objects

noinst_LTLIBRARIES = libfmt.la

noinst_HEADERS = \
	CLI11/include/CLI/App.hpp \
	CLI11/include/CLI/CLI.hpp \
	CLI11/include/CLI/Config.hpp \
	CLI11/include/CLI/ConfigFwd.hpp \
	CLI11/include/CLI/Error.hpp \
	CLI11/include/CLI/Formatter.hpp \
	CLI11/include/CLI/FormatterFwd.hpp \
	CLI11/include/CLI/Macros.hpp \
	CLI11/include/CLI/Option.hpp \
	CLI11/include/CLI/Split.hpp \
	CLI11/include/CLI/StringTools.hpp \
	CLI11/include/CLI/Timer.hpp \
	CLI11/include/CLI/TypeTools.hpp \
	CLI11/include/CLI/Validators.hpp \
	CLI11/include/CLI/Version.hpp \
	fmt/include/fmt/args.h \
	fmt/include/fmt/chrono.h \
	fmt/include/fmt/color.h \
	fmt/include/fmt/compile.h \
	fmt/include/fmt/core.h \
	fmt/include/fmt/format-inl.h \
	fmt/include/fmt/format.h \
	fmt/include/fmt/locale.h \
	fmt/include/fmt/os.h \
	fmt/include/fmt/ostream.h \
	fmt/include/fmt/printf.h \
	fmt/include/fmt/ranges.h \
	fmt/include/fmt/xchar.h

libfmt_la_CPPFLAGS = -Ifmt/include
libfmt_la_SOURCES = \
	fmt/src/format.cc \
	fmt/src/os.cc

extra_DIST = \
	CLI11/LICENSE \
	CLI11/README.md \
	fmt/LICENSE.rst \
	fmt/README.rst
