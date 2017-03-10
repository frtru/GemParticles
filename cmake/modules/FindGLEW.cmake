find_path(GLEW_INCLUDE_DIRS "GL/glew.h"
	PATHS $ENV{GLEW_HOME}/include 
	"/usr/include"
    "/usr/local/include")

find_library(GLEW_LIBRARIES 
	NAMES glew32 libglew32
	PATHS $ENV{GLEW_HOME}/built/lib
	"/usr/lib"
	"/usr/local/lib")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLEW DEFAULT_MSG
                                  GLEW_LIBRARIES GLEW_INCLUDE_DIRS)