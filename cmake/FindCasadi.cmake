find_path(CASADI_INCLUDE_DIR
	NAMES
		casadi/casadi.hpp
    HINTS
        /usr/include
        /usr/local/include
    )

find_library(CASADI_LIBRARY
	NAME casadi
	PATHS
		/usr/include
		/usr/local/include
	)


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  Casadi
	DEFAULT_MSG
	CASADI_INCLUDE_DIR
	CASADI_LIBRARY)


if (CASADI_FOUND)
	set(CASADI_LIBRARIES ${CASADI_LIBRARY})
	set(CASADI_INCLUDE_DIRS ${CASADI_INCLUDE_DIR})
endif()

mark_as_advanced(CASADI_INCLUDE_DIR CASADI_LIBRARY)

