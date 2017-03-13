FIND_PATH(GLFW3_INCLUDE_DIRS "GLFW/glfw3.h" "GLFW/glfw3native.h"
  PATHS 
  $ENV{GLFW_HOME}/include 
  ${GLFW_HOME}/include 
  "/usr/include"
  "usr/local/include")

# Search for the library
FIND_LIBRARY(GLFW3_LIBRARIES NAMES glfw3dll
  PATHS 
  $ENV{GLFW_HOME}/built/lib/
  $ENV{GLFW_HOME}/lib-mingw-w64
  $ENV{GLFW_HOME}/lib-vc2012
  $ENV{GLFW_HOME}/lib-vc2013
  $ENV{GLFW_HOME}/lib-vc2015
  ${GLFW_HOME}/lib-vc2012
  ${GLFW_HOME}/lib-vc2013
  ${GLFW_HOME}/lib-vc2015
  ${GLFW_HOME}/built/lib/
  "/usr/lib"
  "/usr/local/include")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLFW3 DEFAULT_MSG
                                  GLFW3_LIBRARIES GLFW3_INCLUDE_DIRS)

mark_as_advanced(GLFW3_LIBRARIES GLFW3_INCLUDE_DIRS)						