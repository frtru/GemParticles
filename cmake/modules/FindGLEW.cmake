
if(WIN32 OR WINDOWS)
find_path(GLEW_INCLUDE_DIRS "GL/glew.h"
	PATHS $ENV{GLEW_HOME}/include 
	"/usr/include"
    "/usr/local/include")

find_library(GLEW_LIBRARIES 
	NAMES glew32 libglew32
	PATHS $ENV{GLEW_HOME}/built/lib
	$ENV{GLEW_HOME}/lib/Release/Win32
	"/usr/lib"
	"/usr/local/lib")
endif()

if(UNIX)
find_path(GLEW_INCLUDE_DIRS "GL/glew.h"
	PATHS "/usr/include"
    "/usr/local/include")


find_library(GLEW_LIBRARIES 
	NAMES glew GLEW
	PATHS
	"/usr/lib"
	"/usr/lib64"
	"/usr/local/lib"
	"/usr/local/lib64")
endif()

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLEW DEFAULT_MSG
                                  GLEW_LIBRARIES GLEW_INCLUDE_DIRS)

mark_as_advanced(GLEW_LIBRARIES GLEW_INCLUDE_DIRS)
