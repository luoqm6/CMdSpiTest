cmake_minimum_required (VERSION 3.5.1)
project (CTP)

# The version number.
set (Tutorial_VERSION_MAJOR 1)
set (Tutorial_VERSION_MINOR 0)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
	"${PROJECT_SOURCE_DIR}/config.h.in"
	"${PROJECT_BINARY_DIR}/config.h"
)


# add the binary tree to the search path for include files
# so that we will find config.h
include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_BINARY_DIR}/CTP_API")


# add the executable
link_libraries(${PROJECT_BINARY_DIR}/CTP_API/thostmduserapi.so)
link_libraries(${PROJECT_BINARY_DIR}/CTP_API/thosttraderapi.so)
add_executable (CTP main.cpp)






