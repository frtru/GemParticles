
if(WIN32 OR WINDOWS)
find_path(GLEW_INCLUDE_DIRS "GL/glew.h"
	PATHS $ENV{GLEW_HOME}/include 
	"/usr/include"
    "/usr/local/include")
endif()

if(UNIX)
find_path(GLEW_INCLUDE_DIRS "gl/glew.h"
	PATHS "/usr/include"
    "/usr/local/include")
endif()

find_library(GLEW_LIBRARIES 
	NAMES glew32 libglew32
	PATHS $ENV{GLEW_HOME}/built/lib
	"/usr/lib"
	"/usr/local/lib")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLEW DEFAULT_MSG
                                  GLEW_LIBRARIES GLEW_INCLUDE_DIRS)

mark_as_advanced(GLEW_LIBRARIES GLEW_INCLUDE_DIRS)