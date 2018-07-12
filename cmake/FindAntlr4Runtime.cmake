find_path(ANTLR4RUNTIME_INCLUDE_DIR
	NAMES
		antlr4-runtime.h
    HINTS
        /usr/include
        /usr/local/include
    PATH_SUFFIXES
        antlr4-runtime
    )

find_library(ANTLR4RUNTIME_LIBRARY
	NAME antlr4-runtime
	PATHS
		/usr/include
		/usr/local/include
	)


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
	Antlr4Runtime
	DEFAULT_MSG
	ANTLR4RUNTIME_INCLUDE_DIR
	ANTLR4RUNTIME_LIBRARY)


if (ANTLR4RUNTIME_FOUND)
	set(ANTLR4RUNTIME_LIBRARIES ${ANTLR4RUNTIME_LIBRARY})
	set(ANTLR4RUNTIME_INCLUDE_DIRS ${ANTLR4RUNTIME_INCLUDE_DIR})
endif()

mark_as_advanced(ANTLR4RUNTIME_INCLUDE_DIR ANTLR4RUNTIME_LIBRARY)

