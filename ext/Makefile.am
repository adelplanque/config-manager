AUTOMAKE_OPTIONS = subdir-objects

noinst_LTLIBRARIES = libfmt.la libjinja2cpp.la libsubprocess.la

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
	CLI11/include/CLI/TypeTools.hpp \
	CLI11/include/CLI/Validators.hpp \
	CLI11/include/CLI/Version.hpp \
	Jinja2Cpp/include/jinja2cpp/binding/rapid_json.h \
	Jinja2Cpp/include/jinja2cpp/config.h \
	Jinja2Cpp/include/jinja2cpp/error_info.h \
	Jinja2Cpp/include/jinja2cpp/filesystem_handler.h \
	Jinja2Cpp/include/jinja2cpp/generic_list.h \
	Jinja2Cpp/include/jinja2cpp/reflected_value.h \
	Jinja2Cpp/include/jinja2cpp/string_helpers.h \
	Jinja2Cpp/include/jinja2cpp/template.h \
	Jinja2Cpp/include/jinja2cpp/template_env.h \
	Jinja2Cpp/include/jinja2cpp/user_callable.h \
	Jinja2Cpp/include/jinja2cpp/value.h \
	Jinja2Cpp/include/jinja2cpp/value_ptr.hpp \
	Jinja2Cpp/src/ast_visitor.h \
	Jinja2Cpp/src/error_handling.h \
	Jinja2Cpp/src/expression_evaluator.h \
	Jinja2Cpp/src/expression_parser.h \
	Jinja2Cpp/src/filters.h \
	Jinja2Cpp/src/function_base.h \
	Jinja2Cpp/src/generic_adapters.h \
	Jinja2Cpp/src/helpers.h \
	Jinja2Cpp/src/internal_value.h \
	Jinja2Cpp/src/lexer.h \
	Jinja2Cpp/src/lexertk.h \
	Jinja2Cpp/src/out_stream.h \
	Jinja2Cpp/src/rapid_json_serializer.h \
	Jinja2Cpp/src/render_context.h \
	Jinja2Cpp/src/renderer.h \
	Jinja2Cpp/src/robin_hood.h \
	Jinja2Cpp/src/statements.h \
	Jinja2Cpp/src/string_functions.h \
	Jinja2Cpp/src/template_impl.h \
	Jinja2Cpp/src/template_parser.h \
	Jinja2Cpp/src/testers.h \
	Jinja2Cpp/src/value_helpers.h \
	Jinja2Cpp/src/value_visitors.h \
	Jinja2Cpp/thirdparty/json/rapid/include/rapidjson/allocators.h \
	Jinja2Cpp/thirdparty/json/rapid/include/rapidjson/document.h \
	Jinja2Cpp/thirdparty/json/rapid/include/rapidjson/encodedstream.h \
	Jinja2Cpp/thirdparty/json/rapid/include/rapidjson/encodings.h \
	Jinja2Cpp/thirdparty/json/rapid/include/rapidjson/error/en.h \
	Jinja2Cpp/thirdparty/json/rapid/include/rapidjson/error/error.h \
	Jinja2Cpp/thirdparty/json/rapid/include/rapidjson/internal/biginteger.h \
	Jinja2Cpp/thirdparty/json/rapid/include/rapidjson/internal/clzll.h \
	Jinja2Cpp/thirdparty/json/rapid/include/rapidjson/internal/diyfp.h \
	Jinja2Cpp/thirdparty/json/rapid/include/rapidjson/internal/dtoa.h \
	Jinja2Cpp/thirdparty/json/rapid/include/rapidjson/internal/ieee754.h \
	Jinja2Cpp/thirdparty/json/rapid/include/rapidjson/internal/itoa.h \
	Jinja2Cpp/thirdparty/json/rapid/include/rapidjson/internal/meta.h \
	Jinja2Cpp/thirdparty/json/rapid/include/rapidjson/internal/pow10.h \
	Jinja2Cpp/thirdparty/json/rapid/include/rapidjson/internal/stack.h \
	Jinja2Cpp/thirdparty/json/rapid/include/rapidjson/internal/strfunc.h \
	Jinja2Cpp/thirdparty/json/rapid/include/rapidjson/internal/strtod.h \
	Jinja2Cpp/thirdparty/json/rapid/include/rapidjson/internal/swap.h \
	Jinja2Cpp/thirdparty/json/rapid/include/rapidjson/memorystream.h \
	Jinja2Cpp/thirdparty/json/rapid/include/rapidjson/prettywriter.h \
	Jinja2Cpp/thirdparty/json/rapid/include/rapidjson/rapidjson.h \
	Jinja2Cpp/thirdparty/json/rapid/include/rapidjson/reader.h \
	Jinja2Cpp/thirdparty/json/rapid/include/rapidjson/stream.h \
	Jinja2Cpp/thirdparty/json/rapid/include/rapidjson/stringbuffer.h \
	Jinja2Cpp/thirdparty/json/rapid/include/rapidjson/writer.h \
	Jinja2Cpp/thirdparty/nonstd/expected-lite/include/nonstd/expected.hpp \
	Jinja2Cpp/thirdparty/nonstd/optional-lite/include/nonstd/optional.hpp \
	Jinja2Cpp/thirdparty/nonstd/string-view-lite/include/nonstd/string_view.hpp \
	Jinja2Cpp/thirdparty/nonstd/variant-lite/include/nonstd/variant.hpp \
	fmt/include/fmt/args.h \
	fmt/include/fmt/core.h \
	fmt/include/fmt/format-inl.h \
	fmt/include/fmt/format.h \
	fmt/include/fmt/os.h \
	fmt/include/fmt/xchar.h \
	fmt/src/fmt.cc \
	subprocess/src/cpp/subprocess.hpp \
	subprocess/src/cpp/subprocess/PipeVar.hpp \
	subprocess/src/cpp/subprocess/ProcessBuilder.hpp \
	subprocess/src/cpp/subprocess/basic_types.hpp \
	subprocess/src/cpp/subprocess/environ.hpp \
	subprocess/src/cpp/subprocess/pipe.hpp \
	subprocess/src/cpp/subprocess/shell_utils.hpp \
	subprocess/src/cpp/subprocess/utf8_to_utf16.hpp

libfmt_la_CPPFLAGS = -I$(srcdir)/fmt/include
libfmt_la_SOURCES = \
	fmt/src/format.cc \
	fmt/src/os.cc

libjinja2cpp_la_SOURCES = \
	Jinja2Cpp/src/error_info.cpp \
	Jinja2Cpp/src/expression_evaluator.cpp \
	Jinja2Cpp/src/expression_parser.cpp \
	Jinja2Cpp/src/filesystem_handler.cpp \
	Jinja2Cpp/src/filters.cpp \
	Jinja2Cpp/src/internal_value.cpp \
	Jinja2Cpp/src/lexer.cpp \
	Jinja2Cpp/src/rapid_json_serializer.cpp \
	Jinja2Cpp/src/serialize_filters.cpp \
	Jinja2Cpp/src/statements.cpp \
	Jinja2Cpp/src/string_converter_filter.cpp \
	Jinja2Cpp/src/string_functions.cpp \
	Jinja2Cpp/src/template.cpp \
	Jinja2Cpp/src/template_env.cpp \
	Jinja2Cpp/src/template_parser.cpp \
	Jinja2Cpp/src/testers.cpp \
	Jinja2Cpp/src/value.cpp

libjinja2cpp_la_CPPFLAGS = \
	-I$(srcdir)/Jinja2Cpp/include \
	-I$(srcdir)/Jinja2Cpp/thirdparty/nonstd/string-view-lite/include \
	-I$(srcdir)/Jinja2Cpp/thirdparty/nonstd/optional-lite/include \
	-I$(srcdir)/Jinja2Cpp/thirdparty/nonstd/variant-lite/include \
	-I$(srcdir)/Jinja2Cpp/thirdparty/nonstd/expected-lite/include \
	-I$(srcdir)/Jinja2Cpp/thirdparty/json/rapid/include \
	-I$(srcdir)/fmt/include

libsubprocess_la_SOURCES = \
	subprocess/src/cpp/subprocess/CowData.cpp \
	subprocess/src/cpp/subprocess/PipeVar.cpp \
	subprocess/src/cpp/subprocess/ProcessBuilder.cpp \
	subprocess/src/cpp/subprocess/ProcessBuilder_posix.cpp \
	subprocess/src/cpp/subprocess/ProcessBuilder_windows.cpp \
	subprocess/src/cpp/subprocess/environ.cpp \
	subprocess/src/cpp/subprocess/pipe.cpp \
	subprocess/src/cpp/subprocess/shell_utils.cpp \
	subprocess/src/cpp/subprocess/utf8_to_utf16.cpp

EXTRA_DIST = \
	CLI11/LICENSE \
	CLI11/README.md \
	Jinja2Cpp/LICENSE \
	Jinja2Cpp/README.md \
	Jinja2Cpp/thirdparty/json/rapid/license.txt \
	Jinja2Cpp/thirdparty/json/rapid/readme.md \
	Jinja2Cpp/thirdparty/nonstd/expected-lite/LICENSE.txt \
	Jinja2Cpp/thirdparty/nonstd/expected-lite/LICENSE.txt \
	Jinja2Cpp/thirdparty/nonstd/expected-lite/README.md \
	Jinja2Cpp/thirdparty/nonstd/expected-lite/README.md \
	Jinja2Cpp/thirdparty/nonstd/string-view-lite/LICENSE.txt \
	Jinja2Cpp/thirdparty/nonstd/string-view-lite/README.md \
	Jinja2Cpp/thirdparty/nonstd/variant-lite/LICENSE.txt \
	Jinja2Cpp/thirdparty/nonstd/variant-lite/README.md \
	fmt/LICENSE.rst \
	fmt/README.rst \
	subprocess/LICENSE \
	subprocess/README.md
