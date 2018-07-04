find_path(Antlr4Runtime_INCLUDE_DIR
	NAMES
		antlr4-runtime.h
    HINTS
        /usr/include
        /usr/local/include
    PATH_SUFFIXES
        antlr4-runtime
    )

find_library(Antlr4Runtime_LIBRARY
	NAME antlr4-runtime
	PATHS
		/usr/include
		/usr/local/include
	)


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
	Antlr4Runtime
	DEFAULT_MSG
	Antlr4Runtime_INCLUDE_DIR
	Antlr4Runtime_LIBRARY)


if (Antlr4Runtime_FOUND)
	set(Antlr4Runtime_LIBRARIES ${Antlr4Runtime_LIBRARY})
	set(Antlr4Runtime_INCLUDE_DIRS ${Antlr4Runtime_INCLUDE_DIR})
endif()

mark_as_advanced(Antlr4Runtime_INCLUDE_DIR Antlr4Runtime_LIBRARY)

