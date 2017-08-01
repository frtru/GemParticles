FIND_PATH(GLFW3_INCLUDE_DIRS "GLFW/glfw3.h" "GLFW/glfw3native.h"
  PATHS 
  $ENV{GLFW_HOME}/include 
  ${GLFW_HOME}/include 
  "/usr/include"
  "/usr/local/include")

# Search for the library
if(WIN32 OR WINDOWS)
FIND_LIBRARY(GLFW3_GLFW3_LIBRARY NAMES glfw3dll
  PATHS 
  $ENV{GLFW_HOME}/built/lib/
  $ENV{GLFW_HOME}/lib
  $ENV{GLFW_HOME}/lib-mingw-w64
  $ENV{GLFW_HOME}/lib-vc2012
  $ENV{GLFW_HOME}/lib-vc2013
  $ENV{GLFW_HOME}/lib-vc2015
  ${GLFW_HOME}/lib
  ${GLFW_HOME}/lib-vc2012
  ${GLFW_HOME}/lib-vc2013
  ${GLFW_HOME}/lib-vc2015
  ${GLFW_HOME}/built/lib
)
endif()

if(UNIX)
    find_package(Threads REQUIRED)
    find_package(X11 REQUIRED)
    if(NOT X11_Xrandr_FOUND)
        message(FATAL_ERROR "Xrandr library not found - required for GLFW")
    endif()
    if(NOT X11_xf86vmode_FOUND)
        message(FATAL_ERROR "xf86vmode library not found - required for GLFW")
    endif()
    if(NOT X11_Xcursor_FOUND)
        message(FATAL_ERROR "Xcursor library not found - required for GLFW")
    endif()
    if(NOT X11_Xinerama_FOUND)
        message(FATAL_ERROR "Xinerama library not found - required for GLFW")
    endif()
    if(NOT X11_Xi_FOUND)
        message(FATAL_ERROR "Xi library not found - required for GLFW")
    endif()

    list(APPEND GLFW_X11_LIBRARIES "${X11_Xrandr_LIB}" "${X11_Xxf86vm_LIB}" "${X11_Xcursor_LIB}" "${X11_Xinerama_LIB}" "${X11_Xi_LIB}" "${X11_LIBRARIES}" "${CMAKE_THREAD_LIBS_INIT}" -lrt -ldl)

    FIND_LIBRARY(GLFW3_GLFW3_LIBRARY NAMES glfw3 libglfw3
      PATHS
     "/usr/local/lib"
     "/usr/local/lib64"
     "/usr/lib"
     "/usr/lib64"
    )
endif(UNIX)

set(GLFW3_LIBRARIES ${GLFW3_GLFW3_LIBRARY}
        ${GLFW_X11_LIBRARIES})

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLFW3 DEFAULT_MSG
                                  GLFW3_LIBRARIES GLFW3_INCLUDE_DIRS)

mark_as_advanced(GLFW3_LIBRARIES GLFW3_INCLUDE_DIRS)						
